// Date creation : 07/02/2022
// Author : Judith Millet

#include "typedefs.h"
#include "Linear_cell_complex_cut_operation.h"
#include "lcc_triangulate_faces.h"

////////////////////////////////////////////////////////////////////////////////

struct PrimitiveCompare
{
  bool operator() (const Primitive& lhs, const Primitive& rhs) const
  {
    return &*(lhs.id())<&*(rhs.id());
  }
};

////////////////////////////////////////////////////////////////////////////////
[[ noreturn ]] void usage(int /*argc*/, char** argv)
{
  // Name
  std::cout<<"Name"<<std::endl;
  std::cout<<"        "<<argv[0]<<" - test reconstruction of a building from a point cloud.";
  std::cout<<std::endl<<std::endl;
  // Synopsis
  std::cout<<"SYNOPSIS"<<std::endl;
  std::cout<<"        "<<argv[0]<<" ../data/file"
           <<" [--help|-h|-?] "
           <<"[-epsilon E] [-nbPlanes N]"
           <<std::endl<<std::endl;
  // Description
  std::cout<<"DESCRIPTION"<<std::endl;
  std::cout<<"        "<<"test reconstruction of a building from a point cloud, using RANSAC, an AABB Tree and LCC.";
  std::cout<<std::endl<<std::endl;
  // Options
  std::cout<<"        --help, -h, -?"<<std::endl
           <<"                display this help and exit."
           <<std::endl<<std::endl;
  std::cout<<"        -epsilon E"
           <<std::endl
           <<"                number of points for a surface (170 by default)."
           <<std::endl<<std::endl;
  std::cout<<"        -nbPlanes N"
           <<std::endl
           <<"                number of planes to insert before segmentation (5 by default)."
           <<std::endl<<std::endl;
  exit(EXIT_FAILURE);
}
////////////////////////////////////////////////////////////////////////////////
[[ noreturn ]] void error_command_line(int argc, char** argv, const char* msg)
{
  std::cout<<"ERROR: "<<msg<<std::endl;
  usage(argc, argv);
}
////////////////////////////////////////////////////////////////////////////////
void process_command_line(int argc, char** argv, unsigned int& epsilon, unsigned int& nbPlanes)
{
  epsilon = 170;
  nbPlanes = 5;
  bool helprequired=false;
  std::string arg;
  for (int i=1; i<argc; ++i)
  {
    arg=std::string(argv[i]);
    if (arg==std::string("-h") || arg==std::string("--help") || arg==std::string("-?"))
    { helprequired=true; }
    else if(arg=="-epsilon")
    {
      if (argc-1-i<1)
      { error_command_line(argc, argv, "Error: we need a number after -epsilon option."); }
      epsilon=std::stod(std::string(argv[++i]));
    }
    else if(arg=="-nbPlanes")
    {
      if (argc-1-i<1)
      { error_command_line(argc, argv, "Error: we need a number after -nbPlanes option."); }
      nbPlanes=std::stod(std::string(argv[++i]));
    }
  }
  if (helprequired) { usage(argc, argv); }
}

////////////////////////////////////////////////////////////////////////////////
Efficient_ransac::Plane_range ransac_plans_detection(Pwn_vector points) {
  // Instantiate shape detection engine.
    Efficient_ransac ransac;
    // Provide input data.
    ransac.set_input(points);
    // Register planar shapes via template method.
    ransac.add_shape_factory<Plane>();

    // Set parameters for shape detection.
    Efficient_ransac::Parameters parameters;
    // Set probability to miss the largest primitive at each iteration.
    parameters.probability = 0.05;
    // Detect shapes with at least 200 points.
    parameters.min_points = 200;
    // Set maximum Euclidean distance between a point and a shape.
    parameters.epsilon = 0.002;
    // Set maximum Euclidean distance between points to be clustered.
    parameters.cluster_epsilon = 0.01;
    // Set maximum normal deviation.
    // 0.9 < dot(surface_normal, point_normal);
    parameters.normal_threshold = 0.9;

    // Detect shapes.
    ransac.detect(parameters);
    // Detect registered shapes with default parameters.
    // ransac.detect();

    // Print number of detected shapes and unassigned points.
    std::cout << ransac.shapes().end() - ransac.shapes().begin() << " detected shapes, "
    << ransac.number_of_unassigned_points() << " unassigned points." << std::endl;

    // Efficient_ransac::planes() provides
    // an iterator range to the detected planes.
    return ransac.planes();
}

void max_boundaries_box(Pwn_vector points, double &sxmin, double & symin, double &szmin, 
                                          double &sxmax, double & symax, double &szmax) {
    // std::cout<<"coordinates box"<<std::endl;
    for(long unsigned int i=0; i<points.size(); i++) {
      if(sxmin > points[i].first.x()) sxmin = points[i].first.x();
      if(symin > points[i].first.y()) symin = points[i].first.y();
      if(szmin > points[i].first.z()) szmin = points[i].first.z();
      if(sxmax < points[i].first.x()) sxmax = points[i].first.x();
      if(symax < points[i].first.y()) symax = points[i].first.y();
      if(szmax < points[i].first.z()) szmax = points[i].first.z();
    }
    sxmax = sxmax-sxmin;
    symax = symax-symin;
    szmax = szmax-szmin;
    // std::cout << "MIN x : " << sxmin << " y : " << symin << " z : " << szmin << std::endl;
    // std::cout << "MAX x : " << sxmax << " y : " << symax << " z : " << szmax << std::endl;
}

void segmentation(LCC &lcc, Pwn_vector points, unsigned int epsilon) {
    std::map<Primitive, Pair, PrimitiveCompare> m;
    std::list<Triangle> triangles;

    for (auto it=lcc.one_dart_per_cell<2>().begin(); it!=lcc.one_dart_per_cell<2>().end(); ++it) {
      triangles.push_back(Triangle(lcc.point(it), lcc.point(lcc.beta(it,1)), lcc.point(lcc.beta(it,1,1))));
      Primitive pKey = std::prev(triangles.end());
      Pair pVal = std::make_pair(it,0);

      m[pKey] = pVal;
    }

    Tree tree(triangles.begin(),triangles.end());

    for (long unsigned int i=0; i<points.size(); i++) {
      Point_and_primitive_id pp = tree.closest_point_and_primitive(points[i].first);
      Primitive closest_primitive = pp.second;
      m[closest_primitive].second += 1; // point added to the total point of a surface
    }

    // recalculer epsilon

    for (const auto &KeyVal : m) {
      Primitive key = KeyVal.first;
      // std::cout<<m[key].second<<std::endl;
      if(m[key].second < epsilon && !lcc.is_free(m[key].first, 3)) {
        std::cout<<m[key].second<<std::endl;
        lcc.remove_cell<2>(m[key].first);
      }
    }

}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    std::cout << "Processing shape detection" << std::endl;

    std::cout << "Efficient RANSAC" << std::endl;
    // const std::string filename = argv[argc-1];
    const std::string filename = "/home/judith/Documents/M1/POM/pom-s2/data/point_set_3.xyz";// argv[1]; 
    // Points with normals.
    Pwn_vector points;

    CGAL::Timer t;
    t.start();
    // Load point set from a file.
    if (!CGAL::IO::read_points(filename, std::back_inserter(points),
            CGAL::parameters::point_map(Point_map()).normal_map(Normal_map()))) {
        std::cerr << "Error: cannot read file !" << std::endl;
        return EXIT_FAILURE;
    }  else {
        std::cout << " Done. " << points.size() << " points. Time: "
            << t.time() << " sec." << std::endl;
    }


    // Shape detection
    Efficient_ransac::Plane_range planes = ransac_plans_detection(points);
    
    // Plane construction
    LCC lcc;
    double sxmin, symin, szmin, sxmax, symax, szmax;
    unsigned int epsilon;
    unsigned int nbPlanes;

    process_command_line(argc, argv, epsilon, nbPlanes);

    sxmin = symin = szmin = 10;
    sxmax = symax = szmax = -10;
    max_boundaries_box(points, sxmin, symin, szmin, sxmax, symax, szmax);

    CGAL::Real_timer lcc_timer;
    lcc_timer.start();

    make_hexa(lcc, typename LCC::Point(sxmin-0.5,symin-0.5,szmin-0.5), sxmax+0.5, symax+0.5, szmax+0.5);

    Plane *p;
    unsigned int nb = 0;
    for (auto it=planes.begin(); it!=planes.end(); ++it) {
      p = dynamic_cast<Plane *>(it->get());
      if (p!=nullptr) {
        nb++;
        Cut_by_plane<LCC>::cut(lcc,
                                static_cast<typename LCC::Traits::Plane_3>(*p));
        if (nb % nbPlanes == 0 || nb == planes.size()) {
          // Triangulation
          triangulate_all_faces(lcc);
          
          // construction AABB tree
          // std::cout<<"segmentation"<<std::endl;
          segmentation(lcc, points, epsilon);
          CGAL::draw(lcc);
          // std::cout<<"NB "<<nb<<std::endl;
        }
      }
    }


    

    

    // how many points make a plane --> avg ? total/?


    // unsigned int total_points = 0;
    // for ( const auto &KeyVal : m ) {
    //   Primitive key = KeyVal.first;
    //   total_points += m[key].second;
    // }

    // unsigned int avg = total_points / m.size();

    // std::cout<<"SIZE = "<<m.size()<<std::endl;
    // std::cout<<total_points<<" & AVG = "<<avg<<std::endl;

    // Conclusion
    lcc_timer.stop();

    lcc.display_characteristics(std::cout)
        <<(lcc.is_valid()?", valid.":", NOT VALID.")<<std::endl;
    std::cout<<"[TIME]: to compute intersection of planes "<<lcc_timer.time()
            <<std::endl;

    CGAL::draw(lcc);

    return EXIT_SUCCESS;

}
