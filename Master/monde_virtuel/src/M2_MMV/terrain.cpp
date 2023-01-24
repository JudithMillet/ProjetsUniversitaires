//! \file terrain.cpp MMV - TP1 - Génération/analyse de terrains
#include "terrain.h"
#include "scalarfield.h"


////////// FONCTIONS UTILES //////////

bool HeightField::inside(vec3 p) const {
    return false;
}

float HeightField::slope(int i, int j) const {
    vec2 g = gradient(i, j);
    return sqrt(g.x*g.x + g.y*g.y);
}

vec3 HeightField::normal(int i, int j) const {
    vec2 g = gradient(i, j);
    vec3 n = vec3(-g.x, 1, -g.y);
    return normalize(n); // = n / ||n||
}

vec3 HeightField::point(int i, int j) const {
    float z = height(i, j);
    float x = a.x + ((b.x - a.x) * i)/(nx - 1);
    float y = a.y + ((b.y - a.y) * j)/(ny - 1);
    return vec3(x, z, y);
}

////////// CREATION MAILLAGE //////////

Mesh HeightField::createMesh() {
    Mesh res = Mesh(GL_TRIANGLES);
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny ; j++) {
            res.normal(normal(i, j));
            res.texcoord((float)i / (float)nx, (float)j / (float)ny);
            res.vertex(point(i, j));
        }
    }
    for (int i = 0; i < nx - 1; i++)
    {
        for (int j = 0; j < ny - 1; j++)
        {
            int p0, p1, p2, p3;
            p0 = i * nx + j;
            p1 = (i + 1) * nx + j;
            p2 = i * nx + j + 1;
            p3 = (i + 1) * nx + j + 1;
            res.triangle(p0, p2, p3);
            res.triangle(p0, p3, p1);
        }
    }

    return res;
}

////////// OPERATEURS //////////

HeightField HeightField::operator+(const ScalarField& sf2) const
{
    HeightField sf;
    sf.a = sf2.a;
    sf.b = sf2.b;
    sf.nx = sf2.nx;
    sf.ny = sf2.ny;
    sf.h.resize(nx * ny);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            sf.h[index(i, j)] = height(i, j) + sf2.height(i, j);
        }
    }
    return sf;
}

HeightField& HeightField::operator+=(const ScalarField& sf2)
{   
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            h[index(i, j)] = height(i, j) + sf2.height(i, j);
        }
    }
    return *this;
}

////////// SMOOTH DU TERRAIN //////////

void HeightField::smooth() {
    for(int i = 0; i<nx; i++) {
        for(int j = 0; j<ny; j++) {
            float sum = 0;
            for (int m = -1; m < 2 ; m++) {
                for (int n = -1; n < 2 ; n++) {
                    vec2i neighbor = vec2i(i, j) + vec2i(m,n);
                    if (inRange(neighbor.x, neighbor.y))
                        sum += height(neighbor.x, neighbor.y);
                }
            }
            h[index(i,j)] = sum / 9;
        }
    }
}


////////// CARTES D'ANALYSE ////////// 

ScalarField HeightField::slopeMap() {
    ScalarField sf(a,b,nx,ny);
    for(int i = 0; i<nx; i++) {
        for(int j = 0; j<ny; j++) {
            sf.h[index(i,j)] = slope(i,j);
        }
    }
    return sf;
}


ScalarField HeightField::laplacianMap() {
    ScalarField sf(a,b,nx,ny);
    for(int i = 0; i<nx; i++) {
        for(int j = 0; j<ny; j++) {
            if (inRange(i,j))
                sf.h[index(i,j)] = laplacian(i,j);
            else
                sf.h[index(i,j)] = 0.0;
        }
    }
    return sf;
}



ScalarField HeightField::drainMap() {
    ScalarField A(a, b, nx, ny); 

    std::list<Cell> L = {};
    Cell c;

    for(int i=0 ; i<nx ; i++) {
        for(int j=0; j<ny ; j++) {
            A.h[index(i,j)] = 1.0;
            c.q = height(i,j);
            c.i = i;
            c.j = j;
            L.push_back(c);
        }
    }

    L.sort(std::greater<Cell>());
    M2 mask;
    for(Cell c : L) {
        if (inRange(c.i, c.j)) {
            Neighborhood neighbors;
            for(int k=0 ; k<mask.nb_demi ; k++) {
                vec2i neighbor = vec2i(c.i, c.j) + mask.m2[k];
                if(c.q > height(neighbor.x, neighbor.y)) {
                    neighbors.neighbors[neighbors.n] = k;
                    neighbors.n++;
                }
            }
            for(int k=0 ; k< neighbors.n ; k++) {
                vec2i neighbor = vec2i(c.i, c.j) + mask.m2[neighbors.neighbors[k]];
                A.h[index(neighbor.x, neighbor.y)] += (1.0/ neighbors.n) * A.height(c.i, c.j);
            }
        }
    }
    return A;
}

ScalarField HeightField::uplift()
{
    ScalarField sf(a, b, nx, ny);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {           
            sf.h[index(i, j)] = 1.0;
        }
    }
    return sf;
}

ScalarField HeightField::erosionMap()
{
    ScalarField res(a, b, nx, ny);
    ScalarField slope = slopeMap();
    ScalarField drain = drainMap();
    ScalarField laplacian = laplacianMap();
    ScalarField up = uplift();

    res = up + ((drain^(0.7)*-1.0) * slope^(1.5)) + laplacian;

    return res;
}


////////// CREATION LISTE D'ADJACENCE //////////

void HeightField::computeNeighbors(adjacency_list_t &adjacency_list, const unsigned int &road) const {
    adjacency_list.clear();
    adjacency_list.resize(nx*ny);
    M2 mask; 
    for(int i = 0; i<nx; i++) {
        for(int j = 0; j<ny; j++) {
            std::vector<neighbor> neighbors;
            
            for (int k = 0; k < mask.nb; k++) {
                vec2i n = vec2i(i, j) + mask.m2[k];
                if (inRange(n.x, n.y, 1)) {
                    // calculate weight
                    weight_t weight;
                    switch (road) {
                        case 1 :
                            weight = computeCostThroughSlope(vec2i(i, j), n);
                            break;
                        case 2 :
                            weight = computeCostThroughEnvironment(vec2i(i, j), n);
                            break;
                        default:
                            weight = computeCostThroughEnvironment(vec2i(i, j), n)
                                    + computeCostThroughSlope(vec2i(i, j), n);
                            break;
                    }
                    neighbors.push_back(neighbor(index(n.x,n.y), weight));
                }
            }
            adjacency_list.at(index(i,j)) = neighbors;
        }
    }
}



////////// CALCUL COUT ENTRE 2 POINTS //////////

unsigned int calculateDistance(const vec2i &i_id, const vec2i &n_id) {
    return std::max(abs(i_id.x - n_id.x), abs(i_id.y - n_id.y));
}

double calculDistance(const vec2i &i_id, const vec2i &n_id) {
    return std::sqrt(std::pow(n_id.x - i_id.x, 2) + std::pow(n_id.y - i_id.y, 2));
}


weight_t HeightField::computeCostThroughSlope(const vec2i &initial_id, const vec2i &neighbor_id) const {
    double distance = calculDistance(initial_id, neighbor_id);
    double difference = height(initial_id.x, initial_id.y) - height(neighbor_id.x, neighbor_id.y);

    // tan(a) = opp/adj, avec opp = différence de hauteur entre les 2 et adj = distance entre les 2
    weight_t a = fabs(std::atan2(difference, distance));
    a = static_cast<int>(a * 100);

    return a;
}


weight_t HeightField::computeCostThroughEnvironment(const vec2i &initial_id, const vec2i &neighbor_id) const {
    double distance = calculDistance(initial_id, neighbor_id);
    double depth = height(neighbor_id.x, neighbor_id.y) * (10.f / (float)nx);

    if (depth > 0.8 || depth <= 0.15) {
        return 1000*distance;
    }
    
    return 1*distance;
}


////////// GENERATION D'UNE ROUTE //////////


std::list<vertex_t> HeightField::createRoad(const vertex_t &source, const vertex_t& end, const unsigned int &road) const {
    
    // Init adjacency list
    adjacency_list_t adjacency_list;
    computeNeighbors(adjacency_list, road);

    // Compute Dijkstra's algorithm
    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;
    DijkstraComputePaths(source, adjacency_list, min_distance, previous);

    std::list<vertex_t> path;
    path = DijkstraGetShortestPathTo(end, previous);
    return path;
}



////////// FONCTION DE DRAW DE LA ROUTE //////////

void HeightField::drawPath(Image &image, std::list<vertex_t> path) {
    M2 mask; 
    Color col = Color(0.75, 0, 0);
	for (vertex_t p : path) {
        vec2i indices = index(p);
        int i = indices.x;
        int j = indices.y;
        if (inRange(i, j, 1)) {
            if (nx*ny > 40000) { // route de largeur 3
                for (int k = 0; k < mask.nb; k++) {
                       vec2i n = vec2i(i, j) + mask.m2[k];
                       image(n.y, n.x) = col;
                }
            } else if (nx*ny > 4000) { // route de largeur 2
                for (int k = 0; k < mask.nb_demi; k++) {
                       vec2i n = vec2i(i, j) + mask.m2[k];
                       image(n.y, n.x) = col;
                }
            }
            image(j, i) = col;
        }
	}
}