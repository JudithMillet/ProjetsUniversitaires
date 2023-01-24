// Date creation : 07/02/2022
// Author : Judith Millet

#include "Linear_cell_complex_cut_operation.h"
#include "lcc_triangulate_faces.h"
#include "typedefs.h"
#include <CGAL/Kernel/global_functions_3.h>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
struct PrimitiveCompare {
  bool operator()(const Primitive &lhs, const Primitive &rhs) const {
    return &*(lhs.id()) < &*(rhs.id());
  }
};

// bool PlaneCompare (const Plane& p1, const Plane& p2)
// {
//     return (p1.plane_normal() == p2.plane_normal() &&
//     compare_distance(p1.d(), p2.d(), 1)); // add threshold to d()
// };
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] void usage(int /*argc*/, char **argv) {
  // Name
  std::cout << "Name" << std::endl;
  std::cout << "        " << argv[0]
            << " - test reconstruction of a building from a point cloud.";
  std::cout << std::endl << std::endl;
  // Synopsis
  std::cout << "SYNOPSIS" << std::endl;
  std::cout << "        " << argv[0] << " ../data/file"
            << " [--help|-h|-?] "
            << " [-optimal_epsilon] [-epsilon E] [-normal N] [-distance D] "
               "[-ransac_param] [-preprocess] [-less]"
            << std::endl
            << std::endl;
  // Description
  std::cout << "DESCRIPTION" << std::endl;
  std::cout << "        "
            << "test reconstruction of a building from a point cloud, using "
               "RANSAC, an AABB Tree and LCC.";
  std::cout << std::endl << std::endl;
  // Options
  std::cout << "        --help, -h, -?" << std::endl
            << "                display this help and exit." << std::endl
            << std::endl;
  std::cout
      << "        -optimal_epsilon" << std::endl
      << "                epsilon calculated with surface points density (false by default)."
      << std::endl
      << std::endl;
  std::cout
      << "        -epsilon E" << std::endl
      << "                number of points for a surface (170 by default) "
      << std::endl
      << "                or coverage when used with -optimal_epsilon option ."
      << std::endl
      << std::endl;
  std::cout << "        -normal N" << std::endl
            << "                normal threshold between two similar planes "
               "(50 by default)."
            << std::endl
            << std::endl;
  std::cout << "        -distance D" << std::endl
            << "                distance threshold between two similar planes "
               "(0.6 by default)."
            << std::endl
            << std::endl;
  std::cout << "        -ransac_param" << std::endl
            << "                use parameters for Ransac (false by default)."
            << std::endl
            << std::endl;
  std::cout << "        -preprocess" << std::endl
            << "                use preprocessing (false by default)."
            << std::endl
            << std::endl;
  std::cout << "        -less" << std::endl
            << "                use less points - 1/3 (false by default)."
            << std::endl
            << std::endl;
  exit(EXIT_FAILURE);
}
////////////////////////////////////////////////////////////////////////////////
[[noreturn]] void error_command_line(int argc, char **argv, const char *msg) {
  std::cout << "ERROR: " << msg << std::endl;
  usage(argc, argv);
}
////////////////////////////////////////////////////////////////////////////////
void process_command_line(int argc, char **argv, unsigned int &epsilon,
                          unsigned int &normal_threshold,
                          double &distance_threshold, bool &optimal_epsilon,
                          bool &ransac_param, bool &preprocess,
                          bool &less_points) {
  epsilon = 170;
  normal_threshold = 50;
  distance_threshold = 0.6;
  optimal_epsilon = false;
  ransac_param = false;
  preprocess = false;
  less_points = false;
  bool helprequired = false;

  std::string arg;
  for (int i = 1; i < argc; ++i) {
    arg = std::string(argv[i]);
    if (arg == std::string("-h") || arg == std::string("--help") ||
        arg == std::string("-?")) {
      helprequired = true;
    } else if (arg == "-epsilon") {
      if (argc - 1 - i < 1) {
        error_command_line(argc, argv,
                           "Error: we need a number after -epsilon option.");
      }
      epsilon = std::stod(std::string(argv[++i]));
    } else if (arg == "-normal") {
      if (argc - 1 - i < 1) {
        error_command_line(argc, argv,
                           "Error: we need a number after -normal option.");
      }
      normal_threshold = std::stod(std::string(argv[++i]));
    } else if (arg == "-distance") {
      if (argc - 1 - i < 1) {
        error_command_line(argc, argv,
                           "Error: we need a number after -distance option.");
      }
      distance_threshold = std::stod(std::string(argv[++i]));
    } else if (arg == "-optimal_epsilon") {
      optimal_epsilon = true;
    } else if (arg == "-ransac_param") {
      ransac_param = true;
    } else if (arg == "-preprocess") {
      preprocess = true;
    } else if (arg == "-less") {
      less_points = true;
    }
  }
  if (helprequired) {
    usage(argc, argv);
  }
}
////////////////////////////////////////////////////////////////////////////////
Efficient_ransac::Plane_range ransac_plans_detection(Pwn_vector points,
                                                     bool ransac_param) {
  // Instantiate shape detection engine.
  Efficient_ransac ransac;

  // Provide input data.
  ransac.set_input(points);
  // Register planar shapes via template method.
  ransac.add_shape_factory<Plane>();

  if (ransac_param) {
    // Set parameters for shape detection.
    Efficient_ransac::Parameters parameters;

    // Set probability to miss the largest primitive at each iteration.
    parameters.probability = 0.05;
    // Detect shapes with at least 250 points.
    parameters.min_points = 400;
    // Set maximum Euclidean distance between a point and a shape.
    parameters.epsilon = 0.007;
    // Set maximum Euclidean distance between points to be clustered.
    parameters.cluster_epsilon = 0.01;
    // Set maximum normal deviation.
    // 0.9 < dot(surface_normal, point_normal);
    parameters.normal_threshold = 0.9;

    // Detect shapes.
    ransac.detect(parameters);
  } else {
    // Detect registered shapes with default parameters.
    ransac.detect();
  }

  // Print number of detected shapes and unassigned points.
  std::cout << ransac.shapes().end() - ransac.shapes().begin()
            << " detected shapes, " << ransac.number_of_unassigned_points()
            << " unassigned points." << std::endl;

  // Efficient_ransac::planes() provides
  // an iterator range to the detected planes.
  return ransac.planes();
}
////////////////////////////////////////////////////////////////////////////////
void max_boundaries_box(Pwn_vector points, double &sxmin, double &symin,
                        double &szmin, double &sxmax, double &symax,
                        double &szmax) {

  for (long unsigned int i = 0; i < points.size(); i++) {
    if (sxmin > points[i].first.x())
      sxmin = points[i].first.x();
    if (symin > points[i].first.y())
      symin = points[i].first.y();
    if (szmin > points[i].first.z())
      szmin = points[i].first.z();
    if (sxmax < points[i].first.x())
      sxmax = points[i].first.x();
    if (symax < points[i].first.y())
      symax = points[i].first.y();
    if (szmax < points[i].first.z())
      szmax = points[i].first.z();
  }

  sxmax = sxmax - sxmin;
  symax = symax - symin;
  szmax = szmax - szmin;
}
////////////////////////////////////////////////////////////////////////////////
bool compare_angle(unsigned int val, unsigned int threshold) {
  return (val <= threshold || val >= 360 - threshold ||
          (180 - threshold <= val && val <= 180 + threshold));
}

bool compare_distance(float it_d, float it_tmp_d, float threshold) {
  // std::cout << "comp " << abs(it_d - it_tmp_d) << std::endl;
  return (abs(it_d - it_tmp_d) <= threshold);
}

Kernel::Vector_3 regularize_planes(Kernel::Vector_3 normal,
                                   unsigned int threshold,
                                   unsigned int &regu) {
  // almost vertical or horizontal normal --> rectify | ou _
  // + 2 almost same normals --> avg between both
  Kernel::Vector_3 horizontal = Kernel::Vector_3(0, 1, 0);
  float degree = CGAL::approximate_angle(normal, horizontal);

  if (compare_angle(abs(degree), threshold)) {
    regu++;
    return Kernel::Vector_3(0, 1, 0);
  } else if (compare_angle(abs(abs(degree) - 90), threshold)) {
    regu++;
    return Kernel::Vector_3(normal.x(), 0, normal.z());
  } else
    return normal;
}
////////////////////////////////////////////////////////////////////////////////
void regularize(std::vector<Plane> & planes_v, unsigned int threshold) {
  auto it = planes_v.begin();
  unsigned int regularize_cmpt = 0;
  while(it != planes_v.end()) {
    Kernel::Vector_3 normal = it->plane_normal();
    int index = it - planes_v.begin();
    planes_v.at(index).plane_normal() =
        regularize_planes(normal, threshold, regularize_cmpt);
    it = std::next(it);
  }
  std::cout << "Regularize planes : " << regularize_cmpt << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
// Similarity based on normal and coordinates
void similar_planes(std::vector<bool> &used_planes,
                    const std::vector<Plane> &planes_v,
                    unsigned int normal_threshold, double distance_threshold) {
  auto it = planes_v.begin();

  while (it != planes_v.end()) {
    auto it_tmp = std::next(it);
    Kernel::Vector_3 normal = it->plane_normal();

    float it_d = it->d();

    while (it_tmp != planes_v.end() &&
           it_tmp->d() < it_d + distance_threshold * 2) {
      float it_tmp_d = it_tmp->d();
      Kernel::Vector_3 normal_tmp = it_tmp->plane_normal();

      unsigned int degree = CGAL::approximate_angle(normal, normal_tmp);

      // compare normals && distances
      if (compare_angle(degree, normal_threshold) &&
          compare_distance(it_d, it_tmp_d, distance_threshold)) {
        // TODO take difference normal
        int index_tmp = it_tmp - planes_v.begin();
        used_planes.at(index_tmp) = false;
      }
      if (std::next(it_tmp) != planes_v.end()) {
        it_tmp = std::next(it_tmp);
      } else {
        it_tmp = planes_v.end();
        break;
      }
    }
    it = it_tmp;
  }
}
////////////////////////////////////////////////////////////////////////////////
void segmentation(LCC &lcc, Pwn_vector points, unsigned int epsilon,
                  bool optimal_epsilon, bool less_points) {
  std::map<Primitive, Pair, PrimitiveCompare> m;
  std::list<Triangle> triangles;
  std::vector<float> triangles_area;
  unsigned int avg_points = 0; // average points per plane
  unsigned int nb_planes = 0;

  CGAL::Timer t;
  t.start();
  auto begin = t.time();

  for (auto it = lcc.one_dart_per_cell<2>().begin();
       it != lcc.one_dart_per_cell<2>().end(); ++it) {
    Triangle t = Triangle(lcc.point(it), lcc.point(lcc.beta(it, 1)),
                          lcc.point(lcc.beta(it, 1, 1)));
    triangles.push_back(t);
    Primitive pKey = std::prev(triangles.end());
    Pair pVal = std::make_pair(it, 0);

    m[pKey] = pVal;

    // keep the triangle's area
    if (optimal_epsilon)
      triangles_area.push_back(100 * sqrt(t.squared_area()));
  }

  std::cout << "Fill map Time: " << t.time() - begin << " sec."
            << std::endl;

  begin = t.time();

  std::cout << "- create AABB tree" << std::endl;
  Tree tree(triangles.begin(), triangles.end());

  std::cout << "Init AABB Tree Time: " << t.time() - begin << " sec."
            << std::endl;

  begin = t.time();

  std::cout << "- count points" << std::endl;
  for (long unsigned int i = 0; i < points.size(); i++) {
    Point_and_primitive_id pp =
        tree.closest_point_and_primitive(points[i].first);
    Primitive closest_primitive = pp.second;
    // point added to the total points of a plane
    m[closest_primitive].second += 1; 

    if (less_points) {
      i += 2;
    }
  }

  std::cout << "Count points Time: " << t.time() - begin << " sec."
            << std::endl;

  begin = t.time();

  std::cout << "- Calculate average points per plane" << std::endl;
  for (const auto &keyVal : m) {
    Primitive key = keyVal.first;
    unsigned int val = m[key].second;
    if (val > 0) {
      avg_points += val;
      nb_planes++;
    }
  }
  avg_points = avg_points / nb_planes;
  std::cout << "  Avg points per plane = " << avg_points << std::endl;

  std::cout << "Average Time: " << t.time() - begin << " sec."
            << std::endl;

  begin = t.time();

  std::cout << "- Removing unnecessary faces" << std::endl;
  unsigned int cmpt = 0;

  if (optimal_epsilon) {
    for (const auto &keyVal : m) {
      Primitive key = keyVal.first;
      unsigned int val = m[key].second;
      if (triangles_area[cmpt] != 0 && val / triangles_area[cmpt] < epsilon) {
        lcc.remove_cell<2>(m[key].first);
      } else {
        // std::cout << val / triangles_area[cmpt] << std::endl;
      }
      cmpt++;
    }
  } else {
    for (const auto &keyVal : m) {
      Primitive key = keyVal.first;
      unsigned int val = m[key].second;
      if (val < epsilon) { // avg_points ?
        lcc.remove_cell<2>(m[key].first);
      }
    }
  }

  std::cout << "Remove Time: " << t.time() - begin << " sec."
            << std::endl;
  t.stop();
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  std::cout << "Processing shape detection" << std::endl;

  const std::string filename = argv[1];
  // Points with normals.
  Pwn_vector points;

  CGAL::Timer t;
  t.start();
  auto begin = t.time();
  // Load point set from a file.
  if (!CGAL::IO::read_points(
          filename, std::back_inserter(points),
          CGAL::parameters::point_map(Point_map()).normal_map(Normal_map()))) {
    std::cerr << "Error: cannot read file !" << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << " Done. " << points.size() << " points. Time: " << t.time()
              << " sec." << std::endl;
  }

  // Set parameters
  unsigned int epsilon;
  unsigned int normal_threshold;
  double distance_threshold;
  bool optimal_epsilon;
  bool ransac_param;
  bool preprocess;
  bool less_points;

  process_command_line(argc, argv, epsilon, normal_threshold,
                       distance_threshold, optimal_epsilon, ransac_param,
                       preprocess, less_points);

  // Shape detection
  std::cout << "----- Efficient RANSAC -----" << std::endl;
  Efficient_ransac::Plane_range planes =
      ransac_plans_detection(points, ransac_param);
  std::cout << "RANSAC Time: " << t.time() - begin << " sec." << std::endl;

  // Plane construction
  std::cout << "----- Plane detection -----" << std::endl;
  LCC lcc;
  double sxmin, symin, szmin, sxmax, symax, szmax;

  sxmin = symin = szmin = 10;
  sxmax = symax = szmax = -10;

  std::cout << "max_boundaries_box()" << std::endl;
  max_boundaries_box(points, sxmin, symin, szmin, sxmax, symax, szmax);

  CGAL::Real_timer lcc_timer;
  lcc_timer.start();

  // Organized planes vector based on distances from the origin
  std::vector<Plane> planes_v;

  begin = t.time();
  for (auto it = planes.begin(); it != planes.end(); ++it) {
    Plane *p = it->get();
    planes_v.push_back(*p);
  }
  std::cout << "Create planes_v Time: " << t.time() - begin << " sec."
            << std::endl;

  // planes selected to be treated
  std::vector<bool> used_planes(planes_v.size(), true);

  if (preprocess) {
    begin = t.time();

    // Regularize planes
    regularize(planes_v, 10);

    // erase same planes --> doesn't remove planes
    // std::cout << "before erase unique size = " << planes_v.size() <<
    // std::endl; auto last = std::unique(planes_v.begin(), planes_v.end(),
    // PlaneCompare); planes_v.erase(last, planes_v.end()); std::cout << "After
    // erase unique size = " << planes_v.size() << std::endl;

    // planes sorted before process
    std::sort(
        planes_v.begin(), planes_v.end(),
        [](const Plane &it1, const Plane &it2) { return (it1.d() < it2.d()); });

    // check similar planes
    std::cout << "before extract planes, size = " << planes_v.size()
              << std::endl;
    similar_planes(used_planes, planes_v, normal_threshold, distance_threshold);
    auto count = std::count(used_planes.begin(), used_planes.end(), true);
    std::cout << "After extract planes, size = " << count << std::endl;

    std::cout << "Preprocess Time: " << t.time() - begin << " sec."
              << std::endl;
  }

  std::cout << "bbox_plane_intersection()" << std::endl;
  begin = t.time();
  bbox_plane_intersection(lcc, planes_v, used_planes, sxmin, symin, szmin,
                          sxmax, symax, szmax);
  std::cout << "Fill bounding box Time: " << t.time() - begin << " sec."
            << std::endl;

  std::cout << "triangulate_all_faces()" << std::endl;
  begin = t.time();
  triangulate_all_faces(lcc);
  std::cout << "Triangulate LCC Time: " << t.time() - begin << " sec."
            << std::endl;

  std::cout << "segmentation()" << std::endl;
  begin = t.time();
  segmentation(lcc, points, epsilon, optimal_epsilon, less_points);
  std::cout << "Segmentation Time: " << t.time() - begin << " sec."
            << std::endl;

  // Conclusion
  lcc_timer.stop();
  t.stop();

  lcc.display_characteristics(std::cout)
      << (lcc.is_valid() ? ", valid." : ", NOT VALID.") << std::endl;
  std::cout << "[TIME]: to compute intersection of planes " << lcc_timer.time()
            << std::endl;
  std::cout << "[TIME]: TOTAL " << t.time()
            << std::endl;

  CGAL::draw(lcc);

  return EXIT_SUCCESS;
}
