#ifndef _SCALARFIELD_H
#define _SCALARFIELD_H

#include <vector>
#include <vec.h>
#include <string>
#include <image.h>
#include <image_io.h>
#include <algorithm>
#include "utils.h"

class ScalarField
{
	public:
		vec2i a, b;
		int nx, ny;
		float ex, ey;
		std::vector<float> h;

		ScalarField();
		ScalarField(vec2i a, vec2i b, int nx, int ny) :
			a(a), b(b), nx(nx), ny(ny) {h.resize(nx * ny);
		}
		
		/// <summary>
		/// Calculate gradient at index i,j
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>vec2</returns>
		vec2 gradient(const int i, const int j)const;
		/// <summary>
		/// return height at index i,j
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>float</returns>
		float height(const int i, const int j)const;
		/// <summary>
		/// Initialize a ScalarField with an image 
		/// </summary>
		/// <param name="filename"></param>
		void loadScalarField(const char* filename);
		
		/// <summary>
		/// return position in array
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>int</returns>
		int index(const int i, const int j)const;
		/// <summary>
		/// return position in array
		/// </summary>
		/// <param name="vec"></param>
		/// <returns>int</returns>
		int index(const vec2i vec)const;
		/// <summary>
		/// return Index i,j in vec of int
		/// </summary>
		/// <param name="i"></param>
		/// <returns>vec2i</returns>
		vec2i index(const int i)const;

		/// <summary>
		/// Test if an index is not at the border of the map
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <param name="bound"></param>
		/// <returns>bool</returns>
		bool inRange(const int i, const int j, int bound = 0)const;
		/// <summary>
		/// return lip constant
		/// </summary>
		/// <returns></returns>
		float kLip() const; // constante de lip
		/// <summary>
		/// return laplacian at index i,j
		/// </summary>
		/// <param name="i"></param>
		/// <param name="j"></param>
		/// <returns>float</returns>
		float laplacian(int i, int j) const;

		/// <summary>
		/// multiply every heigh by a scalar
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ScalarField operator *(const double) const;
		/// <summary>
		/// multiply scaflarfield by another scalar field
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ScalarField operator *(const ScalarField &) const;
    	/// <summary>
    	/// add scaflarfield by another scalar field
    	/// </summary>
    	/// <param name=""></param>
    	/// <returns></returns>
    	ScalarField operator +(const ScalarField &) const;
		/// <summary>
		/// Pow operator by a scalar
		/// </summary>
		/// <param name="d"></param>
		/// <returns></returns>
		ScalarField operator ^(const double & d) const;
		
		/// <summary>
		/// export scalrfield to an image 
		/// </summary>
		/// <returns></returns>
		Image draw();

		float interpolation(float a, float b, float val);
		float interpolation01(float a, float b, float val);
};




#endif