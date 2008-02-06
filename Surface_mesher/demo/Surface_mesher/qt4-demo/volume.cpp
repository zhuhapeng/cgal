#include <CGAL/basic.h>

#include  <algorithm> // std::sort
#include <boost/shared_ptr.hpp>

#include <CGAL/Bbox_3.h>
#include <CGAL/Timer.h>

#include "volume.h"
#include "viewer.h"
#include "isovalues_list.h"

#include <QApplication>
#include <QAction>
#include <QMainWindow>
#include <QStatusBar>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QTreeWidgetItem>

// surface mesher
#define CGAL_MESHES_NO_OUTPUT
#include <CGAL/Surface_mesh_vertex_base_3.h>
#include <CGAL/Surface_mesh_cell_base_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#include <CGAL/Surface_mesh_default_criteria_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/Surface_mesh_traits_generator_3.h>
typedef CGAL::Surface_mesh_vertex_base_3<Kernel> Vb;
typedef CGAL::Surface_mesh_cell_base_3<Kernel> Cb;
typedef CGAL::Triangulation_data_structure_3<Vb, Cb> Tds;
typedef CGAL::Delaunay_triangulation_3<Kernel, Tds> Tr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<Tr> C2t3;
typedef CGAL::Implicit_surface_3<Kernel, Binary_image> Surface_3;

#include <CGAL/Surface_mesher/Standard_criteria.h>
#include <CGAL/Surface_mesher/Vertices_on_the_same_psc_element_criterion.h>


struct Threshold : public std::unary_function<FT, unsigned char> {
  double isovalue;

  Threshold(double isovalue) : isovalue(isovalue) {}

  result_type operator()(FT value)
  {
    if(value >=  isovalue)
      return 1;
    else
      return 0;
  }
};

class Classify_from_isovalue_list :
  public std::unary_function<FT, unsigned char> 
{
  typedef std::pair<FT, result_type> Isovalue;
  typedef std::vector<Isovalue> Isovalues;
  boost::shared_ptr<Isovalues> isovalues;

  struct Sort_isovalues : std::binary_function<Isovalue, Isovalue, bool> 
  {
    bool operator()(const Isovalue& isoval1, const Isovalue& isoval2)
    {
      return isoval1.first < isoval2.first;
    }
  };
public:
  Classify_from_isovalue_list(Isovalues_list * list)
  {
    isovalues = boost::shared_ptr<Isovalues>(new Isovalues(list->numberOfIsoValues()));
    for(int i = 0, nbs = list->numberOfIsoValues(); i < nbs; ++i )
      (*isovalues)[i] = std::make_pair(list->isovalue(i), i);
    std::sort(isovalues->begin(), isovalues->end(), Sort_isovalues());
  }

  result_type operator()(FT value)
  {
    result_type result = 0;
//     std::cerr << "isovalues: ";
    for(int i = 1, end = isovalues->size(); i <= end; ++i)
    {
//       std::cerr << (*isovalues)[i-1] << ", ";
      if(value >= (*isovalues)[i-1].first &&
         i >= result)
      {
        result = i;
      }
    }
//     if(result>1)
//       std::cerr << "result = "  << (int)result << "/" << list->numberOfIsoValues() << std::endl;
//     else
//       std::cerr << std::endl;
    if(result>0)
      return (*isovalues)[result-1].second + 1;
    else
      return 0;
  }
};

class Generate_surface_identifiers :
  std::binary_function<Classify_from_isovalue_list::result_type,
                       Classify_from_isovalue_list::result_type,
                       const QTreeWidgetItem*>
{
  Isovalues_list* list;
public:
  Generate_surface_identifiers(Isovalues_list* list) : list(list) {};

  result_type operator()(const Classify_from_isovalue_list::result_type& a,
                         const Classify_from_isovalue_list::result_type& b)
  {
    return list->item(std::min(a, b));
  }
};

// class Classify_from_isovalue_list :
//   public std::unary_function<FT, const QTreeWidgetItem*> 
// {
//   typedef std::pair<FT, result_type> Isovalue;
//   typedef std::vector<Isovalue> Isovalues;
//   boost::shared_ptr<Isovalues> isovalues;

//   struct Sort_isovalues : std::binary_function<Isovalue, Isovalue, bool> 
//   {
//     bool operator()(const Isovalue& isoval1, const Isovalue& isoval2)
//     {
//       return isoval1.first < isoval2.first;
//     }
//   };
// public:
//   Classify_from_isovalue_list(Isovalues_list * list)
//   {
//     isovalues = boost::shared_ptr<Isovalues>(new Isovalues(list->numberOfIsoValues()));
//     for(int i = 0, nbs = list->numberOfIsoValues(); i < nbs; ++i )
//       (*isovalues)[i] = std::make_pair(list->isovalue(i), list->item(i));
//     std::sort(isovalues->begin(), isovalues->end(), Sort_isovalues());
//   }

//   result_type operator()(FT value)
//   {
//     int result = 0;
// //     std::cerr << "isovalues: ";
//     for(int i = 1, end = isovalues->size(); i <= end; ++i)
//     {
// //       std::cerr << (*isovalues)[i-1] << ", ";
//       if(value >= (*isovalues)[i-1].first &&
//          i >= result)
//       {
//         result = i;
//       }
//     }
//     if(result>1)
//       std::cerr << boost::format("result = %1%/%2%\n") % result % isovalues->size();
//     if(result>0)
//       return (*isovalues)[result-1].second;
//     else
//       return 0;
//   }
// };
Volume::Volume(QObject* parent) : 
  Surface(parent),
  m_sm_angle(30),
  m_sm_radius(0),
  m_sm_distance(0),
  m_relative_precision(0.0001),
  m_view_surface(false),
  m_view_mc(false),
  parent(qobject_cast<QMainWindow*>(parent)),
  m_inverse_normals(false),
  two_sides(false)
{
  QAction* marching_cube_action = parent->findChild<QAction*>("actionMarching_cubes");
  QAction* surface_mesher_action = parent->findChild<QAction*>("actionSurface_mesher");
  spinBox_radius_bound = parent->findChild<QDoubleSpinBox*>("spinBox_radius_bound");
  spinBox_distance_bound = parent->findChild<QDoubleSpinBox*>("spinBox_distance_bound");

  isovalues_list = parent->findChild<Isovalues_list*>("isovalues");
  Q_ASSERT_X(isovalues_list, "surface meshing", "cannot find widget \"isovalues\"");

 if(spinBox_distance_bound && spinBox_radius_bound)
  {
    connect(spinBox_radius_bound, SIGNAL(valueChanged(double)),
            this, SLOT(set_radius_bound(double)));
    connect(spinBox_distance_bound, SIGNAL(valueChanged(double)),
            this, SLOT(set_distance_bound(double)));
  }
  else
    CGAL_error_msg("Cannot find spinboxes");

  connect(marching_cube_action, SIGNAL(triggered()),
          this, SLOT(display_marchin_cube()));
  connect(surface_mesher_action, SIGNAL(triggered()),
          this, SLOT(display_surface_mesher_result()));

  QAction* inverse_normals = parent->findChild<QAction*>("actionInverse_normals");
  if(inverse_normals) {
    inverse_normals->setVisible(true);
    connect(inverse_normals, SIGNAL(toggled(bool)),
            this, SLOT(set_inverse_normals(bool)));
    m_inverse_normals = inverse_normals->isChecked();
  }
  else
    CGAL_error_msg("Cannot find action actionInverse_normals!");

  QAction* two_sides_action = parent->findChild<QAction*>("actionDisplay_front_and_back");
  if(two_sides_action) {
    two_sides_action->setVisible(true);
    connect(two_sides_action, SIGNAL(toggled(bool)),
            this, SLOT(set_two_sides(bool)));
    two_sides = two_sides_action->isChecked();
  }
  else
    CGAL_error_msg("Cannot find action actionDisplay_front_and_back!");

  QAction* draw_triangles_edges_action = parent->findChild<QAction*>("actionDraw_triangles_edges");
  if(draw_triangles_edges_action) {
    draw_triangles_edges_action->setVisible(true);
    connect(draw_triangles_edges_action, SIGNAL(toggled(bool)),
            this, SLOT(set_draw_triangles_edges(bool)));
    draw_triangles_edges = draw_triangles_edges_action->isChecked();
  }
  else
    CGAL_error_msg("Cannot find action actionDraw_triangles_edges!");

  QAction* use_gouraud_action = parent->findChild<QAction*>("actionUse_Gouraud_shading");
  if(use_gouraud_action) {
    use_gouraud_action->setVisible(true);
    connect(use_gouraud_action, SIGNAL(toggled(bool)),
            this, SLOT(set_use_gouraud(bool)));
    use_gouraud = use_gouraud_action->isChecked();
  }
  else
    CGAL_error_msg("Cannot find action actionUse_Gouraud_shading!");

  viewer = parent->findChild<Viewer*>("viewer");
  if(viewer)
    connect(this, SIGNAL(new_bounding_box(double, double, double, double, double, double)),
            viewer, SLOT(interpolateToFitBoundingBox(double, double, double, double, double, double)));
  else
    CGAL_error_msg("Cannot find the viewer!");
  connect(isovalues_list, SIGNAL(colors_changed()),
          viewer, SLOT(updateGL()));
  connect(isovalues_list, SIGNAL(isovalues_changed()),
          this, SLOT(changed_parameters()));
}

void Volume::set_inverse_normals(const bool b) {
  m_inverse_normals = b;
  emit changed();
}

void Volume::set_two_sides(const bool b) {
  two_sides = b;
  emit changed();
}

void Volume::set_draw_triangles_edges(const bool b) {
  draw_triangles_edges = b;
  emit changed();
}

void Volume::set_use_gouraud(const bool b) {
  use_gouraud = b;
  emit changed();
}

void Volume::open(const QString& filename)
{
  fileinfo.setFile(filename);
  if(!fileinfo.isReadable())
  {
    QMessageBox::warning(parent, parent->windowTitle(),
                         QString(tr("Cannot read file <tt>%1</tt>!")).arg(filename));
    status_message(QString("Opening of file %1 failed!").arg(filename));
  }
  else
  {
    if(!m_image.read(filename.toStdString().c_str()))
    {
      QMessageBox::warning(parent, parent->windowTitle(),
                           QString(tr("Error with file <tt>%1</tt>:\nunknown file format!")).arg(filename));
      status_message(QString("Opening of file %1 failed!").arg(filename));
    }
    else
    {
      status_message(QString("File %1 successfully opened.").arg(filename));
      viewer->camera()->setSceneBoundingBox(qglviewer::Vec(0, 0, 0),
                                            qglviewer::Vec(m_image.xmax(),
                                                           m_image.ymax(),
                                                           m_image.zmax()));

      viewer->showEntireScene();
      isovalues_list->load_values(fileinfo.absoluteFilePath());
      emit changed();
    }
  }
}

void Volume::status_message(QString string)
{
  std::cerr << qPrintable(string) << std::endl;
  parent->statusBar()->showMessage(string, 20000);
}

void Volume::busy() const 
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void Volume::not_busy() const 
{
  QApplication::restoreOverrideCursor();
}

void Volume::display_marchin_cube()
{
  if(m_surface_mc.empty())
  {
    isovalues_list->save_values(fileinfo.absoluteFilePath());
    unsigned int nx = m_image.xdim();
    unsigned int ny = m_image.ydim();
    unsigned int nz = m_image.zdim();
    if(nx * ny * nz == 0)
    {
      status_message("No volume loaded.");
      return;
    }

    CGAL::Timer timer;
    busy();
    status_message("Marching cubes...");

    timer.start();
    m_surface_mc.clear();

    for(int isovalue_id = 0; 
        isovalue_id < isovalues_list->numberOfIsoValues();
        ++isovalue_id)
    {
      status_message(QString("Marching cubes, isovalue #%1...").arg(isovalue_id));

      if(mc.ntrigs()!=0)
        mc.clean_all();
      mc.set_resolution(nx,ny,nz);
      mc.init_all();

      // set data
      for(unsigned int i=0;i<nx;i++)
        for(unsigned int j=0;j<ny;j++)
          for(unsigned int k=0;k<nz;k++)
          {
            const float& value = m_image.value(i,j,k);
            mc.set_data(value,i,j,k);
          }
      // compute scaling ratio
      const double xr = m_image.xmax() / nx;
      const double yr = m_image.ymax() / ny;
      const double zr = m_image.zmax() / nz;

      mc.run(isovalues_list->isovalue(isovalue_id), xr, yr, zr);

      std::vector<double> facets;
      mc.get_facets(facets);

      timer.stop();
      const unsigned int nbt = facets.size() / 9;
      for(unsigned int i=0;i<nbt;i++)
      {
        const Point a(facets[9*i],   facets[9*i+1], facets[9*i+2]);
        const Point b(facets[9*i+3], facets[9*i+4], facets[9*i+5]);
        const Point c(facets[9*i+6], facets[9*i+7], facets[9*i+8]);
        const Triangle_3 t(a,b,c);
        const Vector u = t[1] - t[0];
        const Vector v = t[2] - t[0];
        Vector n = CGAL::cross_product(u,v);
        n = n / std::sqrt(n*n);
        m_surface_mc.push_back(Facet(t,n,isovalues_list->item(isovalue_id)));
      }
      timer.start();
    }
    timer.stop();
    not_busy();

    mc.clean_temps();
    status_message(QString("Marching cubes...done (%2 facets in %1 s)").arg(timer.time()).arg(m_surface_mc.size()));
  }
  CGAL::Bbox_3 bbox(0,0,0,0,0,0);
  for(std::vector<Facet>::const_iterator
        it = m_surface_mc.begin(), end = m_surface_mc.end();
      it != end; ++it)
  {
    bbox = bbox + it->first.bbox();
  }

  m_view_mc = true;
  m_view_surface = false;
  emit changed();
  if(!m_surface_mc.empty())
  {
    emit new_bounding_box(bbox.xmin(),
                          bbox.ymin(),
                          bbox.zmin(),
                          bbox.xmax(),
                          bbox.ymax(),
                          bbox.zmax());
  }
}

void Volume::display_surface_mesher_result()
{
  if(m_surface.empty() || // Either the surface is not computed.
     m_view_surface) // Or it is computed and displayed, and one want
                     // to recompute it.
  {
    isovalues_list->save_values(fileinfo.absoluteFilePath());

    unsigned int nx = m_image.xdim();
    unsigned int ny = m_image.ydim();
    unsigned int nz = m_image.zdim();
    if(nx * ny * nz == 0)
    {
      status_message("No volume loaded.");
      return;
    }

    m_surface.clear();
    CGAL::Timer timer;
    busy();

    status_message("Surface meshing...");

    timer.start();

    Tr tr;            // 3D-Delaunay triangulation
    C2t3 c2t3 (tr);   // 2D-complex in 3D-Delaunay triangulation

    Sphere bounding_sphere(m_image.center(),m_image.radius()*m_image.radius());

    // definition of the surface
    Surface_3 surface(m_image, bounding_sphere, m_relative_precision);
//     Threshold threshold(m_image.isovalue());
    Classify_from_isovalue_list classify(isovalues_list);
    Generate_surface_identifiers generate_ids(isovalues_list);

    std::vector<Point> seeds;
    search_for_connected_components(std::back_inserter(seeds), classify);

    // surface mesh traits class
    typedef CGAL::Surface_mesher::Implicit_surface_oracle_3<Kernel,
      Surface_3, 
      Classify_from_isovalue_list,
      Generate_surface_identifiers> Oracle;
    Oracle oracle(classify, generate_ids);

    for(std::vector<Point>::const_iterator it = seeds.begin(), end = seeds.end();
        it != end; ++it)
    {
      CGAL::Random_points_on_sphere_3<Point> random_points_on_sphere_3(2*m_image.radius());
      Oracle::Intersect_3 intersect = oracle.intersect_3_object();
      for(int i = 0; i < 20; ++i)
      {
        const Point test = *it + (*random_points_on_sphere_3++ - CGAL::ORIGIN);
        CGAL::Object o = intersect(surface, Segment_3(*it, test));
        if (const Point* intersection = CGAL::object_cast<Point>(&o))
          tr.insert(*intersection);
        else 
        {
          std::cerr << 
            boost::format("Error. Segment (%1%, %2%) does not intersect the surface! values=(%3%, %4%)\n")
            % *it % test
            % surface(*it) % surface(test);
        }
      }
    }

    std::cerr << boost::format("Number of initial points: %1%\n") % tr.number_of_vertices();

    // defining meshing criteria
    typedef CGAL::Surface_mesher::Refine_criterion<Tr> Criterion;
    CGAL::Surface_mesher::Curvature_size_criterion<Tr>
      curvature_size_criterion (m_sm_distance);
    CGAL::Surface_mesher::Uniform_size_criterion<Tr>
      uniform_size_criterion (m_sm_radius);
    CGAL::Surface_mesher::Aspect_ratio_criterion<Tr>
      aspect_ratio_criterion (m_sm_angle);
    CGAL::Surface_mesher::Vertices_on_the_same_psc_element_criterion<Tr, Surface_3>
      vertices_on_the_same_psc_element_criterion(surface);
    
    std::vector<Criterion*> criterion_vector;
    criterion_vector.push_back(&aspect_ratio_criterion);
    criterion_vector.push_back(&uniform_size_criterion);
    criterion_vector.push_back(&curvature_size_criterion);
    criterion_vector.push_back(&vertices_on_the_same_psc_element_criterion);

    CGAL::Surface_mesher::Standard_criteria<Criterion> criteria(criterion_vector);
    std::cerr << "Surface_mesher... angle=" << m_sm_angle << ", radius= " << m_sm_radius
              << ", distance=" << m_sm_distance << "\n";

    // meshing surface
    make_surface_mesh(c2t3, surface, oracle, criteria, CGAL::Manifold_tag(), 0);
    timer.stop();
    not_busy();

    // get output surface
    for(C2t3::Facet_iterator
          fit = c2t3.facets_begin(), end = c2t3.facets_end();
        fit != end; ++fit)
    {
      const Tr::Cell_handle& cell = fit->first;
      const int index = fit->second;
      const Triangle_3 t = 
        Triangle_3(cell->vertex(tr.vertex_triple_index(index, 0))->point(),
                   cell->vertex(tr.vertex_triple_index(index, 1))->point(),
                   cell->vertex(tr.vertex_triple_index(index, 2))->point());
      const Vector u = t[1] - t[0];
      const Vector v = t[2] - t[0];
      Vector n = CGAL::cross_product(u,v);
      n = n / std::sqrt(n*n);
      m_surface.push_back(Facet(t,n,cell->vertex(tr.vertex_triple_index(index, 0))->point().element_index()));
    }

    const unsigned int nbt = m_surface.size();
    status_message(QString("Surface meshing...done (%1 triangles, %2 s)").arg(nbt).arg(timer.time()));
  }

  CGAL::Bbox_3 bbox(0,0,0,0,0,0);
  for(std::vector<Facet>::const_iterator
        it = m_surface.begin(), end = m_surface.end();
      it != end; ++it)
  {
    bbox = bbox + it->first.bbox();
  }

  // toggle visualization
  m_view_mc = false;
  m_view_surface = true;

  emit changed();
  if(!m_surface.empty())
  {
    emit new_bounding_box(bbox.xmin(),
                          bbox.ymin(),
                          bbox.zmin(),
                          bbox.xmax(),
                          bbox.ymax(),
                          bbox.zmax());
  }
}

void Volume::draw()
{
  float	ambient[]  =   { 0.25f,
                         0.20725f,
                         0.20725f,
                         0.922f };
  float	diffuse[]  =   { 1.0f,
                         0.829f,
                         0.829f,
                         0.922f };

  float	specular[]  = {  0.296648f,
                         0.296648f,
                         0.296648f,
                         0.522f };

  float	emission[]  = {  0.3f,
                         0.3f,
                         0.3f,
                         1.0f };
  float shininess[] = {  11.264f };

  // apply
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  ::glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,  emission);

  ::glEnable(GL_LINE_SMOOTH);

  if(two_sides)
    ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  else
    ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); // default

  // draw surface mesh
  if(m_view_surface)
  {
    ::glEnable(GL_LIGHTING);
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    ::glColor3f(0.2f, 0.2f, 1.f);
    ::glEnable(GL_POLYGON_OFFSET_FILL);
    ::glPolygonOffset(3.0f,-3.0f);
    gl_draw_surface();

    if(draw_triangles_edges)
    {
      ::glDisable(GL_LIGHTING);
      ::glLineWidth(1.);
      ::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      ::glColor3ub(0,0,0);
      ::glDisable(GL_POLYGON_OFFSET_FILL);
      gl_draw_surface();
    }
  }

  // draw MC surface mesh
  if(m_view_mc)
  {
    ::glEnable(GL_LIGHTING);
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    ::glColor3f(0.2f, 0.2f, 1.f);
    ::glEnable(GL_POLYGON_OFFSET_FILL);
    ::glPolygonOffset(3.0f,-3.0f);
    gl_draw_surface_mc();

    if(draw_triangles_edges)
    {
      ::glDisable(GL_LIGHTING);
      ::glLineWidth(1.);
      ::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      ::glColor3ub(0,0,0);
      ::glDisable(GL_POLYGON_OFFSET_FILL);
      gl_draw_surface_mc();
    }
  }

  ::glDisable(GL_LIGHTING);
  m_image.gl_draw_bbox(3.0f,0,0,0);
}

void Volume::set_radius_bound(double d)
{ 
  m_sm_radius = FT(d);
  changed_parameters();
}

void Volume::set_distance_bound(double d)
{ 
  m_sm_distance = FT(d);
  changed_parameters();
}

void Volume::gl_draw_surface()
{
  gl_draw_surface(m_surface.begin(),
                  m_surface.end());
}

void Volume::gl_draw_surface_mc()
{
  if(use_gouraud)
    gl_draw_marchingcube();
  else
    gl_draw_surface(m_surface_mc.begin(),
                    m_surface_mc.end());
}

template <typename Iterator>
void Volume::gl_draw_surface(Iterator begin, Iterator end)
{
  ::glBegin(GL_TRIANGLES);
  unsigned int counter = 0;
  for(Iterator it = begin; it != end; ++it)
  {
    const Facet& f = *it;
    if(!isovalues_list->enabled(f.third))
      continue;

    const Vector& n = f.second;

    if(m_inverse_normals)
      ::glNormal3d(-n.x(),-n.y(),-n.z());
    else
      ::glNormal3d(n.x(),n.y(),n.z());

    const Triangle_3& t = f.first;
    const Point& a = t[0];
    const Point& b = t[1];
    const Point& c = t[2];

    viewer->qglColor(isovalues_list->color(f.third));

    ::glVertex3d(a.x(),a.y(),a.z());
    ::glVertex3d(b.x(),b.y(),b.z());
    ::glVertex3d(c.x(),c.y(),c.z());
    ++counter;
  }
  ::glEnd();
}

void Volume::changed_parameters()
{
  m_surface.clear();
  m_surface_mc.clear();
}

void Volume::gl_draw_one_marching_cube_vertex(int i)
{
  if(!m_inverse_normals)
    glArrayElement(i);
  else
  {
    const Vertex* const vertex = mc.vert(i);
    ::glNormal3d(-vertex->nx, -vertex->ny, -vertex->nz);
    ::glVertex3d(vertex->x, vertex->y, vertex->z);
  }
}

void Volume::gl_draw_marchingcube()
{
  ::glVertexPointer(3, GL_DOUBLE, sizeof(Vertex), mc.vertices());
  ::glNormalPointer(GL_DOUBLE, sizeof(Vertex), &(mc.vertices()->nx));
  ::glEnableClientState(GL_VERTEX_ARRAY);

  if(!m_inverse_normals)
    ::glEnableClientState(GL_NORMAL_ARRAY);
  const int size = mc.ntrigs();

  ::glBegin(GL_TRIANGLES);
  for(int i = 0; i < size; ++i)
  {
    const MC_Triangle* const trig = mc.trig(i);
    gl_draw_one_marching_cube_vertex(trig->v1);
    gl_draw_one_marching_cube_vertex(trig->v2);
    gl_draw_one_marching_cube_vertex(trig->v3);
  }
  ::glEnd();
}
