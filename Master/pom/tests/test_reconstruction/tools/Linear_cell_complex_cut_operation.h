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
////////////////////////////////////////////////////////////////////////////////
#ifndef CGAL_LINEAR_CELL_COMPLEX_CUT_OPERATION_H
#define CGAL_LINEAR_CELL_COMPLEX_CUT_OPERATION_H

#include <CGAL/Linear_cell_complex_constructors.h>
#include <CGAL/Timer.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>
#include <iostream>
#include "lcc_creations.h"
#include "typedefs.h"

template<typename LCC>
class Cut_by_plane
{
public:
  typedef LCC               LCC_3;
  typedef typename LCC_3::Dart_handle Dart_handle;
  typedef typename LCC_3::Point       Point;
  typedef typename LCC_3::FT          FT;
  typedef typename LCC_3::Traits      K;
  typedef typename LCC_3::size_type   size_type;

  typedef typename K::Plane_3     Plane;
  typedef typename K::Segment_3   Segment;
  typedef typename K::Intersect_3 Intersect_3;
  typedef typename K::Direction_3 Direction;
  typedef typename K::Vector_3    Vector_3;

  static void cut(LCC_3& lcc, const Plane& plane)
  {
    // std::cout<<"BEFORE: ";
    // lcc.display_characteristics(std::cout);
    // std::cout<<std::endl;

    size_type vertex_mark = lcc.get_new_mark();
    insert_points(lcc, plane, vertex_mark);
    // lcc.display_characteristics(std::cout);

    size_type edge_mark = lcc.get_new_mark();
    insert_edges(lcc, vertex_mark, edge_mark);
    // lcc.display_characteristics(std::cout);

    insert_faces(lcc, edge_mark);

    // std::cout<<"AFTER: ";
    // lcc.display_characteristics(std::cout);
    // std::cout<<std::endl;
  }

protected:
  // Insert all intersection points between lcc and plane.
  // All darts of vertices on plane are marked by vertex_mark (new vertices
  // results of the intersection, and old vertices on the plane).
  static void insert_points(LCC_3& lcc, const Plane& plane, size_type vertex_mark)
  {
    std::size_t nb_inserts=0;
    std::vector<std::pair<Dart_handle, Point>> toinsert;
    size_type edge_mark=lcc.get_new_mark();

    // On itère sur les arrêtes, on récupère leurs points en itérant sur beta 1
    for (typename LCC_3::Dart_range::iterator current=lcc.darts().begin(),
         currentend=lcc.darts().end(); current!=currentend; ++current)
    {
      if(!lcc.is_marked(current, edge_mark))
      {
        lcc.template mark_cell<1>(current, edge_mark); // Mark edge
        const Point &pt1=lcc.point(current);
        const Point &pt2=lcc.point(lcc.beta(current, 1));
        Segment seg(pt1, pt2);
        auto result = intersection(seg, plane);
        if (result) // Here we have a non empty intersection
        {
          if (/*const Segment *s = */boost::get<Segment>(&*result))
          { // Here intersection is an existing edge: both extremities belong
            // to the plane
            if (!lcc.is_marked(current, vertex_mark))
            { lcc.template mark_cell<0>(current, vertex_mark); }
            if (!lcc.is_marked(lcc.beta(current, 2), vertex_mark))
            { lcc.template mark_cell<0>(lcc.beta(current, 2), vertex_mark); }
          }
          else
          {
            const Point *p = boost::get<Point>(&*result);
            if (*p==pt1) // Intersection is source of the current edge
            {
              if (!lcc.is_marked(current, vertex_mark))
              { lcc.template mark_cell<0>(current, vertex_mark); }
            }
            else if (*p==pt2)  // Intersection is target of the current edge
            { // Maybe this test can be avoid since it will be done for the
              // source of the next dart, but it is painless.
              if (!lcc.is_marked(lcc.beta(current, 2), vertex_mark))
              { lcc.template mark_cell<0>(lcc.beta(current, 2), vertex_mark); }
            }
            else
            { // We need to insert a new point on the current edge
              toinsert.push_back(std::make_pair(current, *p));
            }
          }
        }
      }
    }

    assert(lcc.is_whole_map_marked(edge_mark));
    lcc.free_mark(edge_mark);

    lcc.negate_mark(vertex_mark);
    for (auto dh: toinsert)
    {
      lcc.template insert_point_in_cell<1>(dh.first, dh.second);
      ++nb_inserts;
    }
    lcc.negate_mark(vertex_mark);
    // Here all darts of vertices on the plane are marked (both new and old)

    //std::cout<<"#darts marked after insert points: "
    //         <<lcc.number_of_marked_darts(vertex_mark)<<std::endl;

    // std::cout<<"#inserted vertices: "<<nb_inserts<<std::endl;
  }

  // Insert all intersection edges between lcc and plane.
  // @input: All darts of vertices on plane are marked by vertex_mark.
  // @output: all darts of edges on plane are marked by edge_mark.
  //          all darts are unmarked for vertex_mark and this mark is freed.
  static void insert_edges(LCC_3& lcc, int vertex_mark, int edge_mark)
  {
    std::size_t nb_inserts=0;
    int face_mark=lcc.get_new_mark();
    lcc.negate_mark(face_mark);

    lcc.unmark_all(edge_mark);
    lcc.negate_mark(edge_mark); // To mark automatically new darts

    for (typename LCC_3::Dart_range::iterator face=lcc.darts().begin();
         face!=lcc.darts().end(); ++face)
    {
      if (lcc.is_marked(face, face_mark)) // We consider each face once
      { // We are going to insert all intersection edges in the current face
        std::vector<Dart_handle> edges;
        Dart_handle it = face;
        do
        {
          if (lcc.is_marked(it, vertex_mark))
          {
            edges.push_back(it);
            lcc.unmark(it, vertex_mark);
          }

          lcc.unmark(it, face_mark);
          if (!lcc.template is_free<3>(it))
          {
            lcc.unmark(lcc.template beta<3>(it), face_mark);
            lcc.unmark(lcc.template beta<3>(it), vertex_mark);
          }

          it=lcc.beta(it, 1);
        }
        while(it!=face);

        //std::cout<<"#darts marked in the current face: "<<edges.size()<<std::endl;

        if (edges.size()>=2)
        {
          if(edges.size()==2)
          {
            if(lcc.beta(edges[0], 1)==edges[1])
            {
              // std::cout<<"Edge already exists (case 1)."<<std::endl;
              lcc.template mark_cell<1>(edges[0], edge_mark);
            }
            else
            {
              //  std::cout<<"Insert new edge (case 1)."<<std::endl;
              lcc.insert_cell_1_in_cell_2(edges[0], edges[1]);
              ++nb_inserts;
            }
          }
          else
          {
            // We compute the minimum vertex for an arbitrary direction
            // (intersections of a plane and a face are all on a same line)
            Direction base(Vector_3(lcc.point(edges[0]), lcc.point(edges[1])));
            Dart_handle min=edges[0];
            for (std::size_t i=1; i<edges.size(); ++i)
            {
              Direction direction(Vector_3(lcc.point(min), lcc.point(edges[i])));
              if(direction!=base)
              { min=edges[i]; }
            }

            // intersection_line will containg all darts of intersection edges,
            // with the distance between their source and the minimum vertex.
            std::vector<std::pair<FT, Dart_handle>> intersection_line;
            for(std::size_t i=0; i<edges.size(); ++i)
            {
              std::pair<FT, Dart_handle> point_val;
              point_val.first =
                  Vector_3(lcc.point(min), lcc.point(edges[i])).squared_length();
              point_val.second=edges[i];
              intersection_line.push_back(point_val);
            }

            // Sort all darts of intersection_line according to their distance with
            // the minimum vertex
            std::sort(intersection_line.begin(), intersection_line.end(),
                      [](const std::pair<FT, Dart_handle> & a,
                      const std::pair<FT, Dart_handle> & b) -> bool
            { return a.first < b.first; });
            for (std::size_t i=0; i<intersection_line.size()-1; i+=2)
            {
              if(lcc.beta(intersection_line[i].second, 1)==
                 lcc.beta(intersection_line[i+1].second, 1))
              {
                // std::cout<<"Edge already exists (case 2)."<<std::endl;
                lcc.template mark_cell<1>(intersection_line[i].second, edge_mark);
              }
              else
              {
                //  std::cout<<"Insert new edge (case 1)."<<std::endl;
                lcc.insert_cell_1_in_cell_2(intersection_line[i].second,
                                            intersection_line[i+1].second);
                ++nb_inserts;
              }
            }
          }
        }
      }
    }
    assert(lcc.is_whole_map_unmarked(face_mark));
    lcc.free_mark(face_mark);

    assert(lcc.is_whole_map_unmarked(vertex_mark));
    lcc.free_mark(vertex_mark);

    lcc.negate_mark(edge_mark);
    /* std::cout<<"#darts marked after edge insertions: "
            <<lcc.number_of_marked_darts(edge_mark)<<std::endl; */

    // std::cout<<"#inserted edges: "<<nb_inserts<<std::endl;
  }

  // Insert all intersection faces between lcc and plane.
  // @input: All darts of edges on plane are marked by edge_mark.
  static void insert_faces(LCC_3& lcc, size_type edge_mark)
  {
    std::size_t nb_inserts=0;

    // First we unmark faces that already exist.
    for (typename LCC_3::Dart_range::iterator current=lcc.darts().begin(),
         currentend=lcc.darts().end(); current!=currentend; ++current)
    {
      if (lcc.is_marked(current, edge_mark))
      {
        Dart_handle it=lcc.beta(current, 1);
        bool all_marked=true;
        while (all_marked && it!=current)
        {
          if(!lcc.is_marked(it, edge_mark))
          { all_marked = false; }
          it=lcc.beta(it, 1);
        }

        if (all_marked)
        { // This face already exists in lcc: nothing to do.
          lcc.template unmark_cell<2>(it, edge_mark);
        }
      }
    }

    // Now we are sure that all marked edges belong to a face that needs to
    // be inserted.
    for (typename LCC_3::Dart_range::iterator current=lcc.darts().begin();
         current!=lcc.darts().end(); ++current)
    {
      if(lcc.is_marked(current, edge_mark))
      {
        std::vector<Dart_handle> newFace;
        Dart_handle iterator=current;
        do
        {
          iterator=lcc.beta(iterator, 1);
          while (!lcc.is_marked(iterator, edge_mark))
          { iterator=lcc.beta(iterator, 2, 1); }
          newFace.push_back(iterator);
          lcc.unmark(iterator, edge_mark);
          lcc.unmark(lcc.beta(iterator, 2), edge_mark);
        }
        while(iterator!=current);
        if(!newFace.empty())
        {
          lcc.insert_cell_2_in_cell_3(newFace.begin(), newFace.end());
          ++nb_inserts;
        }
      }
    }

    assert(lcc.is_whole_map_unmarked(edge_mark));
    lcc.free_mark(edge_mark);

    // std::cout<<"#inserted faces: "<<nb_inserts<<std::endl;
  }
};
////////////////////////////////////////////////////////////////////////////////
template<class LCC, typename T>
void plane_intersection(LCC& lcc, T& planes, std::vector<bool> used_planes)
{
  std::cout<<"Plane intersection"<<std::endl;
  Plane p;
  for (auto it=planes.begin(); it!=planes.end(); ++it) {
    unsigned int index = it - planes.begin();
    if(used_planes.at(index)) {
      p = *it;
      // if (p!=nullptr) {
        Cut_by_plane<LCC>::cut(lcc, p);
      // }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
template<class LCC, typename T>
void bbox_plane_intersection(LCC& lcc, T planes, std::vector<bool> used_planes,
                              double sxmin, double symin, double szmin,
                              double sxmax, double symax, double szmax)
{
  make_hexa(lcc, typename LCC::Point(sxmin-0.5,symin-0.5,szmin-0.5), sxmax+0.5, symax+0.5, szmax+0.5);
  plane_intersection(lcc, planes, used_planes);
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////