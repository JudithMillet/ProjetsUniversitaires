
//! \file terrain.h MMV - TP1 - Génération/analyse de terrains

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <iostream>
#include <vector>
#include <vec.h>
#include "scalarfield.h"
#include "mesh.h"
#include <list>
#include "dijkstra.h"



class HeightField : public ScalarField
{
public:
    HeightField() {}
    
    bool inside(vec3 p) const;

    /// <summary>
    /// Return slop at position i,j
    /// </summary>
    /// <param name="i"></param>
    /// <param name="j"></param>
    /// <returns>Slope</returns>
    float slope(int i, int j) const;
    /// <summary>
    /// Return normal at position i,j
    /// </summary>
    /// <param name="i"></param>
    /// <param name="j"></param>
    /// <returns></returns>
    vec3 normal(int i, int j) const;
    /// <summary>
    /// Return point at position i,j
    /// </summary>
    /// <param name="i"></param>
    /// <param name="j"></param>
    /// <returns></returns>
    vec3 point(int i, int j) const;

    /// <summary>
    /// Create mesh of HeightField
    /// </summary>
    /// <returns>Mesh</returns>
    Mesh createMesh();

    /// <summary>
    /// Addition operator
    /// </summary>
    /// <param name="sf2">ScalarField</param>
    /// <returns>HeightField</returns>
    HeightField operator +(const ScalarField& sf2) const;
    /// <summary>
    /// Add ScalarField to actual HeightField
    /// </summary>
    /// <param name="sf2">ScalarField</param>
    /// <returns>HeightField&</returns>
    HeightField& operator +=(const ScalarField& sf2);

    // Lissage du terrain 
    /// <summary>
    /// Smooth HeightField
    /// </summary>
    void smooth();

    /// <summary>
    /// Calculate every slope at all index i,j
    /// </summary>
    /// <returns> ScalarField Map of slope</returns>
    ScalarField slopeMap();

    /// <summary>
    /// Calculate every laplacian at all index i,j
    /// </summary>
    /// <returns>ScalarField Map of laplacian</returns>
    ScalarField laplacianMap();

    /// <summary>
    /// Calculate drain map of a HeightField
    /// </summary>
    /// <returns> ScalarField Map of drain</returns>
    ScalarField drainMap();
    /// <summary>
    /// Set uplift to calculate erosion
    /// </summary>
    /// <returns>ScalarField</returns>
    ScalarField uplift();

    /// <summary>
    /// Calculate Erosion map with this
    /// </summary>
    /// <returns>ScalarField</returns>
    ScalarField erosionMap();

   


    // generate roads
    /// <summary>
    /// Compute adjacency list for each index i,j 
    /// </summary>
    /// <param name="adjacency_list">adjacency_list_t</param>
    /// <param name="road">unsigned int</param>
    void computeNeighbors(adjacency_list_t &adjacency_list, const unsigned int &road) const;

    // compare slopes
    /// <summary>
    /// Calculate weight through slope of each neighbbors
    /// </summary>
    /// <param name="initial_id">vec2i</param>
    /// <param name="neighbor_id">vec2i</param>
    /// <returns>weight_t</returns>
    weight_t computeCostThroughSlope(const vec2i &initial_id, const vec2i &neighbor_id) const;

    // check environment
    /// <summary>
    /// Calculate weight through environment of each neighbbors
    /// </summary>
    /// <param name="initial_id">vec2i</param>
    /// <param name="neighbor_id">vec2i</param>
    /// <returns>weight_t</returns>
    weight_t computeCostThroughEnvironment(const vec2i &initial_id, const vec2i &neighbor_id) const;
    

    /// <summary>
    /// Return the shortest path between stat and end point
    /// </summary>
    /// <param name="source">vertex_t start point</param>
    /// <param name="end">vertex_t end point</param>
    /// <param name="road">unsigned int road criteria</param>
    /// <returns>std::list<vertex_t></returns>
    std::list<vertex_t> createRoad(const vertex_t &source, const vertex_t &end, const unsigned int &road) const;

    /// <summary>
    /// Create an Image of the road as texture
    /// </summary>
    /// <param name="image">Image</param>
    /// <param name="path">std::list<vertex_t></param>
    void drawPath(Image &image, std::list<vertex_t> path);

protected:
};

#endif