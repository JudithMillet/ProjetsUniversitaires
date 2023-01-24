#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/Timer.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/property_map.h>
#include <CGAL/Polygonal_surface_reconstruction.h>
#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Combinatorial_map_operations.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/draw_linear_cell_complex.h>
#include <CGAL/Real_timer.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <map>
#include <utility>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>

// Type declarations.
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;

// Type declarations for Ransac
typedef std::pair<Kernel::Point_3, Kernel::Vector_3> Point_with_normal;
typedef std::vector<Point_with_normal> Pwn_vector;
typedef CGAL::First_of_pair_property_map<Point_with_normal> Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;
typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, Pwn_vector,
                                            Point_map, Normal_map> Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits> Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits> Plane;

// Type declarations for Polygonal Surface Reconstruction
typedef CGAL::Linear_cell_complex_for_combinatorial_map
<3,3,CGAL::Linear_cell_complex_traits<3, Kernel>> LCC;
typedef LCC::Dart_handle Dart_handle;


// AABB Tree
typedef Kernel::FT FT;
typedef Kernel::Ray_3 Ray;
typedef Kernel::Line_3 Line;
typedef Kernel::Triangle_3 Triangle;
typedef std::list<Triangle>::iterator Iterator; //primitive_id
typedef CGAL::AABB_triangle_primitive<Kernel, Iterator> Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive> AABB_triangle_traits;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;
typedef Tree::Point_and_primitive_id Point_and_primitive_id;
typedef std::pair<Dart_handle, unsigned int> Pair;