//#define BOOST_RESULT_OF_USE_DECLTYPE 1
#include <CGAL/Epick_d.h>
#include <typeinfo>
#include <CGAL/NewKernel_d/Cartesian_base.h>
#include <CGAL/NewKernel_d/Cartesian_static_filters.h>
#include <CGAL/NewKernel_d/Cartesian_filter_NT.h>
#include <CGAL/NewKernel_d/Cartesian_filter_K.h>
#include <CGAL/NewKernel_d/Lazy_cartesian.h>
#include <CGAL/NewKernel_d/Wrapper/Cartesian_wrap.h>
#include <CGAL/NewKernel_d/Kernel_d_interface.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Interval_nt.h>
#include <CGAL/use.h>
#include <iostream>

//typedef CGAL::Cartesian_base_d<double,CGAL::Dimension_tag<2> > K0;
//typedef CGAL::Cartesian_base_d<CGAL::Interval_nt_advanced,CGAL::Dimension_tag<2> > KA;
struct KA : CGAL::Cartesian_static_filters<CGAL::Dimension_tag<2>, CGAL::Cartesian_base_d<CGAL::Interval_nt_advanced,CGAL::Dimension_tag<2>, KA>, KA> {};
typedef CGAL::Cartesian_base_d<CGAL::Gmpq,CGAL::Dimension_tag<2> > KE;

struct RC: public
CGAL::Cartesian_static_filters<CGAL::Dimension_tag<2>, // Yes, it is silly to put it there.
 CGAL::Cartesian_refcount<
  CGAL::Cartesian_LA_base_d<double,CGAL::Dimension_tag<2> >
 >, RC
>
{
	RC(){}
	RC(int){}
};

struct K0 : RC {};


#if 0
typedef K0 K2;
#elif 1
typedef CGAL::Cartesian_filter_NT<K0> K2;
#elif 0
typedef CGAL::Cartesian_filter_K<K0,KA,KE> K2;
#elif 1
typedef CGAL::Lazy_cartesian<KE,KA,CGAL::KernelD_converter<KE,KA> > K2;
#endif

#if 0
typedef K2 KK;
#elif 1
typedef CGAL::Cartesian_wrap<K2> KK;
#elif 1
typedef CGAL::Cartesian_wrap<K2> K3;
typedef CGAL::Cartesian_wrap<K3> KK;
#endif

#if 1
#define Kinit(f) =k.f()
#else
#define Kinit(f)
#endif

template<class Ker>
void test2(){
  typedef Ker K1;
  typedef typename K1::Point_d P;
  typedef typename K1::Cartesian_const_iterator_d CI;
  typedef typename K1::Hyperplane_d H;
  typedef typename K1::Sphere_d Sp;
  typedef typename K1::Vector_d V;
  typedef typename K1::Segment_d S;
  typedef typename K1::Aff_transformation_d AT;
  typedef typename K1::Direction_d D;
  typedef typename K1::Line_d L;
  typedef typename K1::Ray_d R;
  typedef typename K1::Iso_box_d IB;
  typedef typename K1::Flat_orientation_d FO;

  //typedef K1::Construct_point CP;
  typedef typename K1::Construct_point_d CP;
  typedef typename K1::Construct_vector_d CV;
  typedef typename K1::Construct_segment_d CS;
  typedef typename K1::Construct_sphere_d CSp;
  typedef typename CGAL::Get_functor<K1, CGAL::Segment_extremity_tag>::type CSE;
  typedef typename K1::Construct_cartesian_const_iterator_d CCI;
  typedef typename K1::Linear_base_d LB;
  typedef typename K1::Orientation_d PO;
  typedef typename K1::Side_of_oriented_sphere_d SOS;
  typedef typename K1::Side_of_bounded_sphere_d SBS;
  typedef typename K1::Compute_coordinate_d CC;
  typedef typename K1::Construct_flat_orientation_d CFO;
  typedef typename K1::In_flat_orientation_d IFO;
  typedef typename K1::In_flat_side_of_oriented_sphere_d IFSOS;
  typedef typename K1::Contained_in_affine_hull_d CAH;
  typedef typename K1::Compare_lexicographically_d CL;
  typedef typename K1::Value_at_d VA;
  typedef typename K1::Construct_hyperplane_d CH;
  typedef typename K1::Center_of_sphere_d COS;
  typedef typename K1::Affine_rank_d AR;
  typedef typename K1::Linear_rank_d LR;
  typedef typename K1::Has_on_positive_side_d HOPS;
  typedef typename K1::Less_coordinate_d LC;
  typedef typename K1::Less_lexicographically_d LL;
  typedef typename K1::Less_or_equal_lexicographically_d LEL;
  typedef typename K1::Midpoint_d M;
  typedef typename K1::Oriented_side_d OS;
  typedef typename K1::Orthogonal_vector_d OV;
  typedef typename K1::Point_dimension_d PD;
  typedef typename K1::Point_of_sphere_d PS;
  typedef typename K1::Point_to_vector_d PV;
  typedef typename K1::Vector_to_point_d VP;
  typedef typename K1::Barycentric_coordinates_d BC;
  typedef typename K1::Construct_direction_d CD;
  typedef typename K1::Construct_line_d CLi;
  typedef typename K1::Construct_ray_d CR;
  typedef typename K1::Construct_iso_box_d CIB;
  typedef typename K1::Construct_aff_transformation_d CAT;
  typedef typename K1::Position_on_line_d PoL;
  typedef typename K1::Equal_d E;
  typedef typename K1::Squared_distance_d SD;

  CGAL_USE_TYPE(AT);
  CGAL_USE_TYPE(D);
  CGAL_USE_TYPE(L);
  CGAL_USE_TYPE(R);
  CGAL_USE_TYPE(IB);

  Ker k
#if 0
    (2)
#endif
    ;
  CP cp Kinit(construct_point_d_object);
  CV cv Kinit(construct_vector_d_object);
  CCI ci Kinit(construct_cartesian_const_iterator_d_object);
  CC cc Kinit(compute_coordinate_d_object);
  PO po Kinit(orientation_d_object);
  CS cs Kinit(construct_segment_d_object);
  CSp csp Kinit(construct_sphere_d_object);
  VA va Kinit(value_at_d_object);
  CH ch Kinit(construct_hyperplane_d_object);
  CSE cse (k);
  SOS sos Kinit(side_of_oriented_sphere_d_object);
  SBS sbs Kinit(side_of_bounded_sphere_d_object);
  CFO cfo Kinit(construct_flat_orientation_d_object);
  IFO ifo Kinit(in_flat_orientation_d_object);
  IFSOS ifsos Kinit(in_flat_side_of_oriented_sphere_d_object);
  CAH cah Kinit(contained_in_affine_hull_d_object);
  LB lb Kinit(linear_base_d_object);
  COS cos Kinit(center_of_sphere_d_object);
  LR lr Kinit(linear_rank_d_object);
  AR ar Kinit(affine_rank_d_object);
  HOPS hops Kinit(has_on_positive_side_d_object);
  LC lc Kinit(less_coordinate_d_object);
  CL cl Kinit(compare_lexicographically_d_object);
  LL ll Kinit(less_lexicographically_d_object);
  LEL lel Kinit(less_or_equal_lexicographically_d_object);
  M m Kinit(midpoint_d_object);
  OS os Kinit(oriented_side_d_object);
  OV ov Kinit(orthogonal_vector_d_object);
  PD pd Kinit(point_dimension_d_object);
  PS ps Kinit(point_of_sphere_d_object);
  PV pv Kinit(point_to_vector_d_object);
  VP vp Kinit(vector_to_point_d_object);
  BC bc Kinit(barycentric_coordinates_d_object);
  CD cd Kinit(construct_direction_d_object);
  CLi cli Kinit(construct_line_d_object);
  CR cr Kinit(construct_ray_d_object);
  CIB cib Kinit(construct_iso_box_d_object);
  CAT cat Kinit(construct_aff_transformation_d_object);
  PoL pol Kinit(position_on_line_d_object);
  E ed Kinit(equal_d_object);
  SD sd Kinit(squared_distance_d_object);

  CGAL_USE(bc);
  CGAL_USE(pol);
  CGAL_USE(cat);
  CGAL_USE(cd);
  CGAL_USE(cli);
  CGAL_USE(cr);
  CGAL_USE(cib);
  P a=cp(3,4);
  assert(pd(a)==2);
  assert(pv(a)[1]==4);
  P b=vp(cv(5,6,7));
  assert(fabs(b[0]-5./7)<.0001);
  assert(lc(b,a,1));
  assert(!lc(a,b,0));
  int rr[]={3,5,2};
  int* r=rr;
  P c=cp(r,r+2);
  assert(!ll(a,a));
  assert(lel(a,a));
  assert(cl(a,a)==CGAL::EQUAL);
  assert(ll(a,c));
  assert(!lel(c,a));
  assert(cl(a,c)==CGAL::SMALLER);
  assert(ll(b,c));
  assert(cl(c,b)==CGAL::LARGER);
  assert(fabs(m(a,c)[0]-3)<.0001);
  assert(fabs(m(a,c)[1]-4.5)<.0001);
  P d=cp(r,r+3,CGAL::Homogeneous_tag());
  S s=cs(c,d);
  std::cout << cc(a,1) << std::endl;
  std::cout << cc(b,1) << std::endl;
  std::cout << cc(cse(s,0),1) << std::endl;
  std::cout << cc(cse(s,1),1) << std::endl;
  for(CI i=ci(a);i!=ci(a,0);++i)
    std::cout << *i << ' ';
  std::cout << '\n';
  P tab[]={a,b,c,d};
  assert(po (&tab[0],tab+3) == CGAL::CLOCKWISE);
  std::cout << sos(tab+1,tab+4,a) << ' ';
  std::cout << sbs(tab+1,tab+4,a) << std::endl;
  P tabp[]={P(0,0),P(1,0),P(0,1)};
  P tabn[]={P(0,0),P(0,1),P(1,0)};
  assert(po(tabp+0,tabp+3)==CGAL::POSITIVE);
  assert(po(tabn+0,tabn+3)==CGAL::NEGATIVE);
  assert(sos(tabp+0,tabp+3,P(3,3))==CGAL::ON_NEGATIVE_SIDE);
  assert(sos(tabn+0,tabn+3,P(3,3))==CGAL::ON_POSITIVE_SIDE);
  assert(sbs(tabp+0,tabp+3,P(3,3))==CGAL::ON_UNBOUNDED_SIDE);
  assert(sbs(tabn+0,tabn+3,P(3,3))==CGAL::ON_UNBOUNDED_SIDE);
  assert(sos(tabp+0,tabp+3,P(.5,.5))==CGAL::ON_POSITIVE_SIDE);
  assert(sos(tabn+0,tabn+3,P(.5,.5))==CGAL::ON_NEGATIVE_SIDE);
  assert(sbs(tabp+0,tabp+3,P(.5,.5))==CGAL::ON_BOUNDED_SIDE);
  assert(sbs(tabn+0,tabn+3,P(.5,.5))==CGAL::ON_BOUNDED_SIDE);
  P x1=cp(0,1);
  P x2=cp(-1,-1);
  P x3=cp(1,-1);
  P x4=cp(0,0);
  P x5=cp(0,-1);
  P tab2[]={x1,x2,x3,x4};
  assert(po(tab2+0,tab2+3)==CGAL::COUNTERCLOCKWISE);
  assert(sos(tab2+0,tab2+3,x4)==CGAL::ON_POSITIVE_SIDE);
  assert(sbs(tab2+0,tab2+3,x4)==CGAL::ON_BOUNDED_SIDE);
  V y1=cv(1,-1);
  V y2=cv(3,-3);
  V tab3[]={y1,y2};
  std::vector<V> v;
  std::back_insert_iterator<std::vector<V> > bii(v);
  lb(tab3+0,tab3+2,bii);
  assert(v.size()==1);
  assert(lr(tab3+0,tab3+2)==1);
  H h=ch(tab2+1,tab2+3);
  assert(fabs(va(h,x2)-1)<.0001);
  assert(fabs(va(h,x3)-1)<.0001);
  assert(fabs(va(h,x1)+1)<.0001);
  H h2=ch(tab2+1,tab2+3,x1,CGAL::ON_POSITIVE_SIDE);
  assert(hops(h2,x1));
  assert(os(h2,x1)==CGAL::ON_POSITIVE_SIDE);
  H h3=ch(tab2+1,tab2+3,x1,CGAL::ON_NEGATIVE_SIDE);
  assert(!hops(h3,x1));
  V hv=ov(h); CGAL_USE(hv);
#if 1
  // Doesn't compile with Lazy yet.
  FO fo=cfo(tab2+1,tab2+3);
  assert(ifo(fo,tab2+1,tab2+3)==CGAL::POSITIVE);
  assert(ifsos(fo,tab2+1,tab2+3,x5)==CGAL::ON_POSITIVE_SIDE);
  P tab_h[]={P(0,42),P(1,42),P(4,42),P(2,42),P(3,42)};
  assert(cah(tab_h+0,tab_h+2,tab_h[4]));
  P py2=cp(3,-3);
  assert(!cah(tab_h+0,tab_h+2,py2));
  FO fo_hp = cfo (tab_h+0, tab_h+2);
  FO fo_hn = cfo (tab_h+2, tab_h+4);
  assert(ifo(fo_hp, tab_h+1, tab_h+3)==CGAL::POSITIVE);
  assert(ifo(fo_hn, tab_h+1, tab_h+3)==CGAL::NEGATIVE);
  assert(ifo(fo_hn, tab_h+2, tab_h+4)==CGAL::POSITIVE);
  assert(ifo(fo_hp, tab_h+2, tab_h+4)==CGAL::NEGATIVE);
  assert(ifsos(fo_hp, tab_h+1, tab_h+3, tab_h[3])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_hn, tab_h+1, tab_h+3, tab_h[3])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_hp, tab_h+0, tab_h+2, tab_h[2])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_hn, tab_h+0, tab_h+2, tab_h[2])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_hp, tab_h+2, tab_h+4, tab_h[1])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_hn, tab_h+2, tab_h+4, tab_h[1])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_hp, tab_h+2, tab_h+4, tab_h[4])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_hn, tab_h+2, tab_h+4, tab_h[4])==CGAL::ON_POSITIVE_SIDE);
  P tab_v[]={P(42,0),P(42,1),P(42,4),P(42,2),P(42,3)};
  assert(ar(tab_v+0,tab_v+5)==1);
  // FIXME: Triangulation says cah is only for independent range, but not Kernel_d
  // assert(cah(tab_v+0,tab_v+4,tab_v[4]));
  assert(cah(tab_v+0,tab_v+2,tab_v[4]));
  FO fo_vp = cfo (tab_v+0, tab_v+2);
  FO fo_vn = cfo (tab_v+2, tab_v+4);
  assert(ifo(fo_vp, tab_v+1, tab_v+3)==CGAL::POSITIVE);
  assert(ifo(fo_vn, tab_v+1, tab_v+3)==CGAL::NEGATIVE);
  assert(ifo(fo_vn, tab_v+2, tab_v+4)==CGAL::POSITIVE);
  assert(ifo(fo_vp, tab_v+2, tab_v+4)==CGAL::NEGATIVE);
  assert(ifsos(fo_vp, tab_v+1, tab_v+3, tab_v[3])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_vn, tab_v+1, tab_v+3, tab_v[3])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_vp, tab_v+0, tab_v+2, tab_v[2])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_vn, tab_v+0, tab_v+2, tab_v[2])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_vp, tab_v+2, tab_v+4, tab_v[1])==CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fo_vn, tab_v+2, tab_v+4, tab_v[1])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_vp, tab_v+2, tab_v+4, tab_v[4])==CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fo_vn, tab_v+2, tab_v+4, tab_v[4])==CGAL::ON_POSITIVE_SIDE);
#endif
  P z0=cp( 0+2,5-3);
  P z1=cp(-5+2,0-3);
  P z2=cp( 3+2,4-3);
  P tabz[]={z0,z1,z2};
  Sp sp = csp(tabz+0,tabz+3);
  P cent0=cos(sp);
  P cent1=cos(tabz+0,tabz+3);
  assert(fabs(cent0[0]-2)<.0001);
  assert(fabs(cent0[1]+3)<.0001);
  assert(fabs(cent1[0]-2)<.0001);
  assert(fabs(cent1[1]+3)<.0001);
  assert(fabs(sp.squared_radius()-25)<.0001);
  P psp0=ps(sp,0);
  P psp1=ps(sp,1);
  P psp2=ps(sp,2);
  assert(!ed(psp0,psp1));
  assert(!ed(psp0,psp2));
  assert(!ed(psp2,psp1));
  assert(fabs(sd(cent0,psp0)-25)<.0001);
  assert(fabs(sd(cent0,psp1)-25)<.0001);
  assert(fabs(sd(cent0,psp2)-25)<.0001);
  Sp un1; CGAL_USE(un1);
  H un2; CGAL_USE(un2);
  S un3; CGAL_USE(un3);
  P un4; CGAL_USE(un4);
  V un5; CGAL_USE(un5);
  CI un6; CGAL_USE(un6);
  FO un7; CGAL_USE(un7);
  L un8; CGAL_USE(un8);
  R un9; CGAL_USE(un9);
  D un10; CGAL_USE(un10);
}

template<class CP> struct Construct_point3_helper {
  CP const& cp;
  Construct_point3_helper(CP const& x) : cp(x) {}
  template<class T1,class T2,class T3>
  typename CP::result_type operator()(T1 const&t1, T2 const&t2, T3 const&t3)const{
    double tab[]={(double)t1,(double)t2,(double)t3};
    return cp(tab+0,tab+3);
  }
  template<class T1,class T2,class T3,class T4>
  typename CP::result_type operator()(T1 const&t1, T2 const&t2, T3 const&t3, T4 const&t4)const{
    double tab[]={(double)t1,(double)t2,(double)t3};
    return cp(tab+0,tab+3,t4);
  }
};

template<class Ker>
void test3(){
  typedef Ker K1;
  //typedef typename K1::FT FT;
  typedef typename K1::Point_d P;
  typedef typename K1::Cartesian_const_iterator_d CI;
  typedef typename K1::Vector_d V;
  typedef typename K1::Segment_d S;
  typedef typename K1::Flat_orientation_d FO;

  //typedef K1::Construct_point CP;
  typedef typename K1::Construct_point_d CP_;
  typedef typename K1::Construct_vector_d CV_;
  typedef typename K1::Construct_segment_d CS;
  typedef typename CGAL::Get_functor<K1, CGAL::Segment_extremity_tag>::type CSE;
  typedef typename K1::Construct_cartesian_const_iterator_d CCI;
  typedef typename K1::Orientation_d PO;
  typedef typename K1::Linearly_independent_d LI;
  typedef typename K1::Side_of_oriented_sphere_d SOS;
  typedef typename K1::Side_of_bounded_sphere_d SBS;
  typedef typename K1::Compute_coordinate_d CC;
  typedef typename K1::Construct_flat_orientation_d CFO;
  typedef typename K1::In_flat_orientation_d IFO;
  typedef typename K1::In_flat_side_of_oriented_sphere_d IFSOS;
  typedef typename K1::Contained_in_affine_hull_d CAH;
  typedef typename K1::Contained_in_linear_hull_d CLH;
  typedef typename K1::Contained_in_simplex_d CiS;
  typedef typename K1::Compare_lexicographically_d CL;
  typedef typename K1::Component_accessor_d CA;
  typedef typename K1::Equal_d E;
  typedef typename K1::Squared_distance_d SD;
  typedef typename K1::Point_dimension_d PD;
  typedef typename K1::Affinely_independent_d AI;

  Ker k
#if 1
    (3)
#endif
    ;
  CP_ cp_ Kinit(construct_point_d_object);
  CV_ cv_ Kinit(construct_vector_d_object);
  typename boost::mpl::if_<boost::is_same<typename Ker::Default_ambient_dimension,CGAL::Dynamic_dimension_tag>,Construct_point3_helper<CP_>,CP_>::type cp(cp_);
  typename boost::mpl::if_<boost::is_same<typename Ker::Default_ambient_dimension,CGAL::Dynamic_dimension_tag>,Construct_point3_helper<CV_>,CV_>::type cv(cv_);
  CCI ci Kinit(construct_cartesian_const_iterator_d_object);
  CC cc Kinit(compute_coordinate_d_object);
  CL cl Kinit(compare_lexicographically_d_object);
  PO po Kinit(orientation_d_object);
  CS cs Kinit(construct_segment_d_object);
  CSE cse (k);
  LI li Kinit(linearly_independent_d_object);
  SOS sos Kinit(side_of_oriented_sphere_d_object);
  SBS sbs Kinit(side_of_bounded_sphere_d_object);
  CFO cfo Kinit(construct_flat_orientation_d_object);
  IFO ifo Kinit(in_flat_orientation_d_object);
  IFSOS ifsos Kinit(in_flat_side_of_oriented_sphere_d_object);
  CAH cah Kinit(contained_in_affine_hull_d_object);
  CLH clh Kinit(contained_in_linear_hull_d_object);
  CiS cis Kinit(contained_in_simplex_d_object);
  CA ca Kinit(component_accessor_d_object);
  E ed Kinit(equal_d_object);
  SD sd Kinit(squared_distance_d_object);
  PD pd Kinit(point_dimension_d_object);
  AI ai Kinit(affinely_independent_d_object);
  P a; // Triangulation needs this :-(
  a=cp(2,3,4);
  assert(pd(a)==3);
  P b=cp(5,6,7,8);
  assert(ed(a,a));
  assert(!ed(a,b));
  assert(ca.dimension(a)==3);
  assert(ca.cartesian(a,1)==3);
  assert(ca.homogeneous(a,1)==3);
  assert(ca.homogeneous(a,3)==1);
  int rr[]={3,5,2,3};
  int* r=rr;
  P c=cp_(3,r,r+3);
  P d=cp_(r,r+4,CGAL::Homogeneous_tag());
  S s=cs(c,d);
  std::cout << cc(a,1) << std::endl;
  std::cout << cc(b,2) << std::endl;
  std::cout << cse(s,0)[1] << std::endl;
  std::cout << cc(cse(s,1),2) << std::endl;
  for(CI i=ci(a);i!=ci(a,0);++i)
    std::cout << *i << ' ';
  std::cout << '\n';
  P e=cp(-2,3,0);
  assert(fabs(sd(e,a)-32)<.0001);
  P tab[]={a,b,c,d,e};
  std::cout << po (&tab[0],tab+4) << ' ';
  std::cout << sos(&tab[0],tab+5) << ' ';
  std::cout << sbs(&tab[0],tab+5) << std::endl;
  FO fo=cfo(&tab[0],tab+3);
  std::cout << fo;
  P x[]={cp(2,2,3),cp(2,2,0),cp(1,2,1)};
  FO fo2=cfo(&x[0],x+3);
  std::cout << fo2;
  P y[]={cp(0,2,4),cp(3,1,2),cp(3,3,6),cp(0,4,8)};
  assert(!cis(x+0,x+3,y[0]));
  V yv[]={cv(0,2,4),cv(3,1,2),cv(3,3,6),cv(0,4,8)};
  assert( clh(yv+0,yv+1,yv[3]));
  assert( clh(yv+0,yv+2,yv[2]));
  assert(!clh(yv+0,yv+1,yv[2]));
  assert( li(yv+0,yv+2));
  assert(!li(yv+0,yv+3));
  assert( cah(y+0,y+3,y[3]));
  assert(!cah(y+0,y+2,y[2]));
  assert( ai(y+0,y+3));
  assert(!ai(y+0,y+4));
  FO fo3=cfo(&y[0],y+3);
  assert(fo3.rest.size()==1 && fo3.rest[0]!=3);
  std::cout << fo3;
  CGAL::Orientation base=ifo(fo3,&y[0],y+3);
  assert(ifo(fo3,y+1,y+4)==base);
  P yy[]={y[1],y[3],y[0],y[2]};
  assert(ifo(fo3,yy+0,yy+3)==base);
  assert(ifo(fo3,yy+1,yy+4)==base);
  std::cout << ifsos(fo3,y+0,y+3,y[3]) << ' ';
  std::cout << ifsos(fo3,y+1,y+4,y[0]) << ' ';
  std::cout << ifsos(fo3,yy+0,yy+3,yy[3]) << ' ';
  std::cout << ifsos(fo3,yy+1,yy+4,yy[0]) << '\n';
  P buf[]={cp(100,900,0),y[0],y[1],y[2],y[3]};
  std::cout << sos(buf+0,buf+5) << ' ';
  buf[1]=y[1];buf[2]=y[2];buf[3]=y[3];buf[4]=y[0];
  std::cout << sos(buf+0,buf+5) << ' ';
  buf[1]=yy[0];buf[2]=yy[1];buf[3]=yy[2];buf[4]=yy[3];
  std::cout << sos(buf+0,buf+5) << ' ';
  buf[1]=yy[1];buf[2]=yy[2];buf[3]=yy[3];buf[4]=yy[0];
  std::cout << sos(buf+0,buf+5) << '\n';
  assert(cah(y+0,y+3,y[3]));
  assert(!cah(y+0,y+3,buf[0]));
  assert(cl(a,a)==CGAL::EQUAL);
  assert(cl(a,b)==CGAL::LARGER);
  P x1=cp(0,1,-1);
  P x2=cp(-1,-1,-1);
  P x3=cp(1,-1,-1);
  P x4=cp(0,0,1);
  P x5=cp(0,0,0);
  P x6=cp(0,0,-1);
  P tab2[]={x1,x2,x3,x4,x5};
  assert(cis(tab2+0,tab2+4,x5));
  assert(po(tab2+0,tab2+4)==CGAL::POSITIVE);
  assert(sos(tab2+0,tab2+4,x5)==CGAL::ON_POSITIVE_SIDE);
  assert(sbs(tab2+0,tab2+4,x5)==CGAL::ON_BOUNDED_SIDE);
  FO fo4=cfo(tab2+0,tab2+3);
  assert(ifo(fo4,tab2+0,tab2+3)==CGAL::POSITIVE);
  assert(ifsos(fo4,tab2+0,tab2+3,x6)==CGAL::ON_POSITIVE_SIDE);
  P tx[]={cp(1,1,42),cp(3,1,42),cp(1,3,42),cp(3,3,42),cp(2,2,42),cp(4,5,42)};
  FO foxp=cfo(tx+0,tx+3);
  FO foxn=cfo(tx+1,tx+4);
  assert(ifo(foxp, tx+0, tx+3) == CGAL::POSITIVE);
  assert(ifo(foxn, tx+0, tx+3) == CGAL::NEGATIVE);
  assert(ifo(foxp, tx+1, tx+4) == CGAL::NEGATIVE);
  assert(ifo(foxn, tx+1, tx+4) == CGAL::POSITIVE);
  assert(ifsos(foxp, tx+0, tx+3, tx[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(foxn, tx+0, tx+3, tx[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(foxp, tx+0, tx+3, tx[4]) == CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(foxn, tx+0, tx+3, tx[4]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(foxp, tx+0, tx+3, tx[5]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(foxn, tx+0, tx+3, tx[5]) == CGAL::ON_POSITIVE_SIDE);
  P ty[]={cp(1,42,1),cp(3,42,1),cp(1,42,3),cp(3,42,3),cp(2,42,2),cp(4,42,5)};
  FO foyp=cfo(ty+0,ty+3);
  FO foyn=cfo(ty+1,ty+4);
  assert(ifo(foyp, ty+0, ty+3) == CGAL::POSITIVE);
  assert(ifo(foyn, ty+0, ty+3) == CGAL::NEGATIVE);
  assert(ifo(foyp, ty+1, ty+4) == CGAL::NEGATIVE);
  assert(ifo(foyn, ty+1, ty+4) == CGAL::POSITIVE);
  assert(ifsos(foyp, ty+0, ty+3, ty[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(foyn, ty+0, ty+3, ty[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(foyp, ty+0, ty+3, ty[4]) == CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(foyn, ty+0, ty+3, ty[4]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(foyp, ty+0, ty+3, ty[5]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(foyn, ty+0, ty+3, ty[5]) == CGAL::ON_POSITIVE_SIDE);
  P tz[]={cp(1,1,42),cp(3,1,42),cp(1,3,42),cp(3,3,42),cp(2,2,42),cp(4,5,42)};
  FO fozp=cfo(tz+0,tz+3);
  FO fozn=cfo(tz+1,tz+4);
  assert(ifo(fozp, tz+0, tz+3) == CGAL::POSITIVE);
  assert(ifo(fozn, tz+0, tz+3) == CGAL::NEGATIVE);
  assert(ifo(fozp, tz+1, tz+4) == CGAL::NEGATIVE);
  assert(ifo(fozn, tz+1, tz+4) == CGAL::POSITIVE);
  assert(ifsos(fozp, tz+0, tz+3, tz[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(fozn, tz+0, tz+3, tz[3]) == CGAL::ON_ORIENTED_BOUNDARY);
  assert(ifsos(fozp, tz+0, tz+3, tz[4]) == CGAL::ON_POSITIVE_SIDE);
  assert(ifsos(fozn, tz+0, tz+3, tz[4]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fozp, tz+0, tz+3, tz[5]) == CGAL::ON_NEGATIVE_SIDE);
  assert(ifsos(fozn, tz+0, tz+3, tz[5]) == CGAL::ON_POSITIVE_SIDE);
}
template struct CGAL::Epick_d<CGAL::Dimension_tag<2> >;
template struct CGAL::Epick_d<CGAL::Dimension_tag<3> >;
template struct CGAL::Epick_d<CGAL::Dynamic_dimension_tag>;
typedef CGAL::Epick_d<CGAL::Dimension_tag<2> > Ker2;
typedef CGAL::Epick_d<CGAL::Dimension_tag<3> > Ker3;
typedef CGAL::Epick_d<CGAL::Dynamic_dimension_tag> Kerd;
CGAL_static_assertion((boost::is_same<CGAL::Dimension_tag<2>,Ker2::Dimension>::value));
CGAL_static_assertion((boost::is_same<CGAL::Dimension_tag<3>,Ker3::Dimension>::value));
CGAL_static_assertion((boost::is_same<CGAL::Dynamic_dimension_tag,Kerd::Dimension>::value));
CGAL_static_assertion((boost::is_same<CGAL::Dimension_tag<2>,CGAL::Ambient_dimension<Ker2::Point_d>::type>::value));
CGAL_static_assertion((boost::is_same<CGAL::Dimension_tag<3>,CGAL::Ambient_dimension<Ker3::Point_d,Ker3>::type>::value));
int main(){
  //Broken with Linear_base_d (output iterator)
  //test2<CGAL::Kernel_d_interface<KK> >();
  test2<Ker2>();
  test3<Ker3>();
  test3<Kerd>();
}