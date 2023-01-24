// Copyright (c) 2021 CNRS and LIRIS' Establishments (France).
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
///////////////////////////////////////////////////////////////////////////////
#ifndef LCC_CREATIONS_H
#define LCC_CREATIONS_H
///////////////////////////////////////////////////////////////////////////////
template<class LCC>
typename LCC::Dart_handle make_xy_quadrangle(LCC& lcc,
                                             const typename LCC::Point& basepoint,
                                             typename LCC::FT lg1,
                                             typename LCC::FT lg2)
{
  return lcc.make_quadrangle(basepoint,
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(0,lg2,0)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,lg2,0)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,0,0)));
}
///////////////////////////////////////////////////////////////////////////////
template<class LCC>
typename LCC::Dart_handle make_hexa(LCC& lcc,
                                    const typename LCC::Point& basepoint,
                                    typename LCC::FT lg1,
                                    typename LCC::FT lg2,
                                    typename LCC::FT lg3)
{
  return lcc.make_hexahedron(basepoint,
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(0,lg2,0)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,lg2,0)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,0,0)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,0,lg3)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(0,0,lg3)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(0,lg2,lg3)),
                             typename LCC::Traits::Construct_translated_point()
                             (basepoint,typename LCC::Traits::Vector(lg1,lg2,lg3)));
}
///////////////////////////////////////////////////////////////////////////////
template<class LCC>
typename LCC::Dart_handle make_iso_cuboid(LCC& lcc,
                                          const typename LCC::Point& basepoint,
                                          typename LCC::FT lg)
{ return make_hexa(lcc, basepoint, lg, lg, lg); }
///////////////////////////////////////////////////////////////////////////////
// Create a mesh, given its basepoint, the size of the full mesh in x, y and z
// and the number of hexa in x, y, z.
template<class LCC>
typename LCC::Dart_handle make_mesh(LCC& lcc,
                                    const typename LCC::Point basepoint,
                                    typename LCC::FT sx,
                                    typename LCC::FT sy,
                                    typename LCC::FT sz,
                                    std::size_t nbx,
                                    std::size_t nby,
                                    std::size_t nbz)
{
  typename LCC::Dart_handle first=nullptr, prevx=nullptr;
  double dx=sx/nbx;
  double dy=sy/nby;
  double dz=sz/nbz;
  std::vector<typename LCC::Dart_handle> darts(nbx*nby*nbz, nullptr);
  for (std::size_t z=0; z<nbz; ++z)
  {
    for (std::size_t y=0; y<nby; ++y)
    {
      for (std::size_t x=0; x<nbx; ++x)
      {
        typename LCC::Dart_handle d = make_hexa
          (lcc,
           typename LCC::Point(basepoint.x()+x*dx,
                               basepoint.y()+y*dy,
                               basepoint.z()+z*dz), dx, dy, dz);
        darts[(z*nbx*nby)+(y*nbx)+x]=d;

        if(x>0)
        { // sew_3 avec hexa à gauche
          lcc.template sew<3>(d, prevx);
        }
        else
        { if(first==nullptr) { first=d; } }
        if(y>0)
        { // sew_3 avec hexa devant
          lcc.template sew<3>(lcc.template beta<2>(d),
                              lcc.template beta<1,1,2>(darts[(z*nbx*nby)+((y-1)*nbx)+x]));
        }
        if(z>0)
        { // sew_3 avec hexa dessous
          lcc.template sew<3>(lcc.template beta<0,2>(d),
                              lcc.template beta<1,2>(darts[((z-1)*nbx*nby)+(y*nbx)+x]));
        }

        prevx=lcc.template beta<2,1,1,2>(d);
      }
    }
  }
  return first;
}
///////////////////////////////////////////////////////////////////////////////
// Create a 2D XY grid, given its basepoint, the size of the full mesh in x, and y
// and the number of squares in x, y.
template<class LCC>
typename LCC::Dart_handle make_xy_grid(LCC& lcc,
                                       const typename LCC::Point basepoint,
                                       typename LCC::FT sx,
                                       typename LCC::FT sy,
                                       std::size_t nbx,
                                       std::size_t nby)
{
  typename LCC::Dart_handle first=nullptr, prevx=nullptr;
  double dx=sx/nbx;
  double dy=sy/nby;
  std::vector<typename LCC::Dart_handle> darts(nbx, nullptr);
  for (std::size_t y=0; y<nby; ++y)
  {
    for (std::size_t x=0; x<nbx; ++x)
    {
      typename LCC::Dart_handle d = make_xy_quadrangle
        (lcc,
         typename LCC::Point(basepoint.x()+x*dx,
                             basepoint.y()+y*dy,
                             basepoint.z()), dx, dy);

      if(x>0)
      { // sew_2 with left square
        lcc.template sew<2>(d, prevx);
      }
      else
      { if(first==nullptr) { first=d; } }
      if(y>0)
      { // sew_2 with square below
        lcc.template sew<2>(lcc.template beta<0>(d), darts[x]);
      }

      prevx=lcc.template beta<1,1>(d);
      darts[x]=lcc.template beta<1>(d);
    }
  }
  return first;
}
///////////////////////////////////////////////////////////////////////////////
#endif // LCC_CREATIONS_H
