#include "scalarfield.h"
#include <typeinfo>

ScalarField::ScalarField() {}

////////// CHARGEMENT D'UNE CARTE //////////

void ScalarField::loadScalarField(const char* filename) {

	Image img = read_image(filename);
	nx = img.width();
	ny = img.height();
	h.resize(nx * ny);

	a = vec2i(0, 0);
	b = vec2i(nx, ny);

	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			h[index(i, j)] = (nx/10) * img.sample(i, j).r;
		}
	}
	ex = (b.x - a.x) / (float)(nx - 1);
	ey = (b.y - a.y) / (float)(ny - 1);
}

////////// FONCTIONS UTILES //////////

vec2 ScalarField::gradient(const int i, const int j)const {
	if (inRange(i, j)) {
		float dhx = (height(i + 1, j) - height(i - 1, j)) / (2 * ex);
		float dhy = (height(i , j + 1) - height(i, j - 1)) / (2 * ey);
		return vec2(dhx, dhy);
	}
	return vec2(0,0);
}

int  ScalarField::index(const int i, const int j)const {
	return i * nx + j;
}

int ScalarField::index(const vec2i vec)const {
	return vec.x * nx + vec.y;
}

vec2i ScalarField::index(const int i)const {
	return vec2i(i % nx, i / nx);
}

bool ScalarField::inRange(const int i, const int j, int bound)const {
    return i > bound && j > bound && j < ny-1-bound && i < nx-1-bound;
}

float ScalarField::height(const int i, const int j)const {
	return h[index(i, j)];
}

float ScalarField::kLip() const {
	float max = 0;
	vec2 g;
	for(int i=0; i<nx; i++) {
		for(int j=0; j<ny; j++) {
			g = gradient(i, j);
			if (max < sqrt(g.x*g.x + g.y*g.y) )
				max = sqrt(g.x*g.x + g.y*g.y);
		}
	}
	return max;
}

float ScalarField::laplacian(int i, int j) const {
	float epsilon = 1;
	if (inRange(i, j))
		return (height(i-1,j) + height(i+1,j) + height(i,j-1) + height(i,j+1) - (4 * height(i, j)))/(epsilon*epsilon);
	return 0;
}

////////// OPERATEURS //////////

ScalarField ScalarField::operator+(const ScalarField & sf2)const {
	ScalarField sf(a, b, nx, ny);
	for(int i=0; i<nx; i++) {
		for(int j=0; j<ny; j++) {
			sf.h[index(i,j)] = height(i,j) + sf2.height(i,j);
		}
	}
	return sf;
}

ScalarField ScalarField::operator^(const double & d) const
{
	ScalarField sf(a, b, nx, ny);
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			sf.h[index(i, j)] = pow(height(i, j), d);
		}
	}
	return sf;
}

ScalarField ScalarField::operator*(const ScalarField & sf2)const {
	ScalarField sf(a,b,nx,ny);
	for(int i=0; i<nx; i++) {
		for(int j=0; j<ny; j++) {
			sf.h[index(i,j)] = height(i,j) * sf2.height(i,j);
		}
	}
	return sf;
}

ScalarField ScalarField::operator*(const double d)const {
	ScalarField sf(a, b, nx, ny);
	for(int i=0; i<nx; i++) {
		for(int j=0; j<ny; j++) {
			sf.h[index(i,j)] = height(i,j) * d;
		}
	}
	return sf;
}

////////// FONCTION DE DRAW //////////

// bornes a et b en param
Image ScalarField::draw() {
	Image image(nx, ny);

	float maxi = h[0];
	float mini = h[0];
	for(int k = 1; k < nx*ny ; k++) {
		maxi = std::max(maxi, h[k]);
		mini = std::min(mini, h[k]);
	}
	
	for (int i=0 ; i<nx ; i++) {
		for (int j=0 ; j<ny ; j++) {

			float col = sqrt(sqrt(height(i, j)/maxi)); //interpolation(0,1,interpolation01(mini,maxi,height(i,j)));
			image(i, j) = Color(col, col, col);
		}
	}
	return image;
}

////////// FONCTIONS D'INTERPOLATION //////////

float ScalarField::interpolation(float a, float b, float val)
{
	return a + ((b - a) * val);
}

float ScalarField::interpolation01(float a, float b, float val)
{

	return (val - a) / (b - a);
}