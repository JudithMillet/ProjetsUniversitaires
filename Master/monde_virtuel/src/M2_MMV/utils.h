#ifndef _UTILS
#define _UTILS

struct Utils_commande {
    char* filename;
    unsigned int smooth, road, erosion; 
};

// Utile pour le drain map
struct Neighborhood{
	int neighbors[8];
	int n = 0;
};

// Utile pour le drain map
struct Cell {
    float q;
    int i, j;

    bool operator<(const Cell& c) const {
        return q < c.q;
    }

    bool operator>(const Cell& c) const {
        return q > c.q;
    }
};


struct vec2i {
	int x, y;

	vec2i() {}

	vec2i(int x, int y) : x(x), y(y) {}

	vec2i operator+(const vec2i& v) const {
		return vec2i(x + v.x, y + v.y);
	}
};

// Masque M2 utile pour parcourir les 8 ou 16 voisins
struct M2 {
  const int nb = 16;
  const int nb_demi = 8;
  vec2i m2[16] = {vec2i(1, 0),   vec2i(1, 1),   vec2i(0, 1),  vec2i(-1, 1),
                  vec2i(-1, 0),  vec2i(-1, -1), vec2i(0, -1), vec2i(1, -1),
                  vec2i(1, 2),   vec2i(2, 1),   vec2i(2, -1), vec2i(1, -2),
                  vec2i(-1, -2), vec2i(-2, -1), vec2i(-2, 1), vec2i(-1, 2)};
};

#endif