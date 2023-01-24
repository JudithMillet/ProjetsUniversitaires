// Copyright (c) 2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of LCC-Demo.
//
// LCC-Demo is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Foobar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
//    along with LCC-Demo.  If not, see <https://www.gnu.org/licenses/>.
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#include <CGAL/Linear_cell_complex_for_combinatorial_map.h>
#include <CGAL/Combinatorial_map_operations.h>
#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/draw_linear_cell_complex.h>
#include <CGAL/Real_timer.h>
#include "Linear_cell_complex_cut_operation.h"

//typedef CGAL::Inexact_predicates_exact_constructions_kernel MyKernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel MyKernel;

typedef CGAL::Linear_cell_complex_for_combinatorial_map
<3,3,CGAL::Linear_cell_complex_traits<3, MyKernel>> LCC3;
////////////////////////////////////////////////////////////////////////////////
[[ noreturn ]] void usage(int /*argc*/, char** argv)
{
  // Name
  std::cout<<"Name"<<std::endl;
  std::cout<<"        "<<argv[0]<<" - test subdivision of an hexahedron by random planes.";
  std::cout<<std::endl<<std::endl;
  // Synopsis
  std::cout<<"SYNOPSIS"<<std::endl;
  std::cout<<"        "<<argv[0]<<" [--help|-h|-?] "
           <<"[-size X Y Z] [-nb N]"
           <<std::endl<<std::endl;
  // Description
  std::cout<<"DESCRIPTION"<<std::endl;
  std::cout<<"        "<<"test subdivision of an hexahedron by random planes.";
  std::cout<<std::endl<<std::endl;
  // Options
  std::cout<<"        --help, -h, -?"<<std::endl
           <<"                display this help and exit."
           <<std::endl<<std::endl;
  std::cout<<"        -size X Y Z"
           <<std::endl
           <<"                size of the initial hexahedron, in x y and z (5x5x5 by default)."
           <<std::endl<<std::endl;
  std::cout<<"        -nb N"
           <<std::endl
           <<"                number of planes (10 by default)."
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
void process_command_line(int argc, char** argv,
                          double& x, double& y, double& z,
                          std::size_t& nb)
{
  x=y=z=5;
  nb=10;
  bool helprequired=false;
  std::string arg;
  for (int i=1; i<argc; ++i)
  {
    arg=std::string(argv[i]);
    if (arg==std::string("-h") || arg==std::string("--help") || arg==std::string("-?"))
    { helprequired=true; }
    else if(arg=="-size")
    {
      if (argc-1-i<3)
      { error_command_line(argc, argv, "Error: we need 3 numbers after -size option."); }
      x=std::stod(std::string(argv[++i]));
      y=std::stod(std::string(argv[++i]));
      z=std::stod(std::string(argv[++i]));
    }
    else if(arg=="-nb")
    {
      if (argc-1-i<1)
      { error_command_line(argc, argv, "Error: we need 1 number after -nb option."); }
      nb=std::stoi(std::string(argv[++i]));
    }
  }
  if (helprequired) { usage(argc, argv); }
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  LCC3 lcc;
  double sx, sy, sz;
  std::size_t nb;
  process_command_line(argc, argv, sx, sy, sz, nb);

  CGAL::Real_timer lcc_timer;
  lcc_timer.start();
  bbox_random_plane_intersection(lcc, sx, sy, sz, nb);

  lcc_timer.stop();

  lcc.display_characteristics(std::cout)
      <<(lcc.is_valid()?", valid.":", NOT VALID.")<<std::endl;
  std::cout<<"[TIME]: to compute intersection of planes "<<lcc_timer.time()
           <<std::endl;

  CGAL::draw(lcc);
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
