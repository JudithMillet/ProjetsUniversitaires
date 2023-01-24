//! \file TP3 utils

#include <vector>
#include <cfloat>
#include <chrono>

#include "gltf.h"

// rayon 
struct Ray
{
    Point o;            // origine
    float pad;
    Vector d;           // direction
    float tmax;         // tmax= 1 ou \inf, le rayon est un segment ou une demi droite infinie
    
    Ray( const Point& _o, const Point& _e ) :  o(_o), d(Vector(_o, _e)), tmax(1) {} // segment, t entre 0 et 1
    Ray( const Point& _o, const Vector& _d ) :  o(_o), d(_d), tmax(FLT_MAX) {}  // demi droite, t entre 0 et \inf
    Ray( const Point& _o, const Vector& _d, const float _tmax ) :  o(_o), d(_d), tmax(_tmax) {} // explicite
};

// intersection / gltf
struct Hit
{
    float t;            // p(t)= o + td, position du point d'intersection sur le rayon
    float u, v;         // p(u, v), position du point d'intersection sur le triangle
    int instance_id;    // indice de l'instance
    int mesh_id;        // indexation globale du triangle dans la scene gltf
    int primitive_id;
    int triangle_id;
    int pad;

    Hit()
        : t(FLT_MAX), u(), v(), instance_id(-1), mesh_id(-1), primitive_id(-1),
          triangle_id(-1) {}
    Hit(const Ray &ray)
        : t(ray.tmax), u(), v(), instance_id(-1), mesh_id(-1), primitive_id(-1),
          triangle_id(-1) {}

    Hit(const float _t, const float _u, const float _v, const int _mesh_id,
        const int _primitive_id, const int _id)
        : t(_t), u(_u), v(_v), instance_id(-1), mesh_id(_mesh_id),
          primitive_id(_primitive_id), triangle_id(_id) {}

    operator bool() {
        return (triangle_id != -1);
    } // renvoie vrai si l'intersection est definie / existe

    void set_instance_id(const int _instance_id) { instance_id = _instance_id; }
};


// triangle / gltf
struct Triangle
{
    Point p;            // sommet a du triangle
    Vector e1, e2;      // aretes ab, ac du triangle
    int mesh_id;
    int primitive_id;
    int triangle_id;
    int instance_id;

    Triangle(const vec3 &a, const vec3 &b, const vec3 &c, const int _mesh_id,
             const int _primitive_id, const int _instance_id, const int _id)
        : p(a), e1(Vector(a, b)), e2(Vector(a, c)), mesh_id(_mesh_id),
          primitive_id(_primitive_id), instance_id(_instance_id),
          triangle_id(_id) {}

    /* calcule l'intersection ray/triangle
        cf "fast, minimum storage ray-triangle intersection"

        renvoie faux s'il n'y a pas d'intersection valide (une intersection peut
       exister mais peut ne pas se trouver dans l'intervalle [0 tmax] du rayon.)
        renvoie vrai + les coordonnees barycentriques (u, v) du point
       d'intersection + sa position le long du rayon (t). convention
       barycentrique : p(u, v)= (1 - u - v) * a + u * b + v * c
    */
    Hit intersect( const Ray &ray, const float htmax ) const
    {
        Vector pvec= cross(ray.d, e2);
        float det= dot(e1, pvec);
        
        float inv_det= 1 / det;
        Vector tvec(p, ray.o);
        
        float u= dot(tvec, pvec) * inv_det;
        if(u < 0 || u > 1) return Hit();
        
        Vector qvec= cross(tvec, e1);
        float v= dot(ray.d, qvec) * inv_det;
        if(v < 0 || u + v > 1) return Hit();
        
        float t= dot(e2, qvec) * inv_det;
        if(t < 0 || t > htmax) return Hit();
        
        return Hit(t, u, v, mesh_id, primitive_id, triangle_id);
    }
};

// Source de lumière
struct Source
{
    Point s;
    Color emission;
};


// METHODES UTILES

Point hit_position(const Hit &hit, const Ray &ray) {
  assert(hit.triangle_id != -1);
  return ray.o + hit.t * ray.d;
}

bool has_normals(const Hit& hit, const GLTFScene& scene)
{
    assert(hit.mesh_id != -1);
    assert(hit.primitive_id != -1);
    const GLTFMesh& mesh = scene.meshes[hit.mesh_id];
    const GLTFPrimitives& primitives = mesh.primitives[hit.primitive_id];
    return primitives.normals.size() > 0;
}

Vector hit_normal(const Hit& hit, const GLTFScene& scene)
{
    assert(hit.instance_id != -1);
    assert(hit.mesh_id != -1);
    assert(hit.primitive_id != -1);
    assert(hit.triangle_id != -1);

    const GLTFMesh& mesh = scene.meshes[hit.mesh_id];
    const GLTFPrimitives& primitives = mesh.primitives[hit.primitive_id];

    // indice des sommets
    int a = primitives.indices[3 * hit.triangle_id];
    int b = primitives.indices[3 * hit.triangle_id + 1];
    int c = primitives.indices[3 * hit.triangle_id + 2];

    // normales des sommets
    assert(primitives.normals.size());
    Vector na = primitives.normals[a];
    Vector nb = primitives.normals[b];
    Vector nc = primitives.normals[c];

    // interpole la normale au point d'intersection
    Vector n = (1 - hit.u - hit.v) * na + hit.u * nb + hit.v * nc;

    // transforme la normale dans le repere de la scene
    const GLTFNode& node = scene.nodes[hit.instance_id];

    Transform T = node.model.normal();

    n = normalize(T(n));
    return n;
}

Vector triangle_normal(const Hit& hit, const GLTFScene& scene)
{
    assert(hit.instance_id != -1);
    assert(hit.mesh_id != -1);
    assert(hit.primitive_id != -1);
    assert(hit.triangle_id != -1);
    const GLTFMesh& mesh = scene.meshes[hit.mesh_id];
    const GLTFPrimitives& primitives = mesh.primitives[hit.primitive_id];

    // indice des sommets
    int a = primitives.indices[3 * hit.triangle_id];
    int b = primitives.indices[3 * hit.triangle_id + 1];
    int c = primitives.indices[3 * hit.triangle_id + 2];

    // postion des sommets
    assert(primitives.positions.size());
    Point pa = primitives.positions[a];
    Point pb = primitives.positions[b];
    Point pc = primitives.positions[c];

    // normale geometrique
    Vector n = cross(Vector(pa, pb), Vector(pa, pc));

    // transforme la normale dans le repere de la scene
    const GLTFNode& node = scene.nodes[hit.instance_id];
    // les normales ne se transforment pas exactement comme les positions... 
    // les sommets sont transformes par node.model, comment transformer la normale 
    // pour quelle reste perpendiculaire au triangle ? cf Transform::normal()
    Transform T = node.model.normal();

    n = normalize(T(n));
    return n;
}

static GLTFMaterial default_material = {
    White(), Black(), 0, 1, 0, 0, 0, Black(), -1, -1, -1, -1, -1, -1, -1, -1};

const GLTFMaterial& hit_material(const Hit& hit, const GLTFScene& scene)
{
    assert(hit.mesh_id != -1);
    assert(hit.primitive_id != -1);
    const GLTFMesh& mesh = scene.meshes[hit.mesh_id];
    const GLTFPrimitives& primitives = mesh.primitives[hit.primitive_id];
    if (primitives.material_index == -1)
        return default_material;

    assert(primitives.material_index < int(scene.materials.size()));
    return scene.materials[primitives.material_index];
}

Color hit_color(const Hit& hit, const GLTFScene& scene)
{
    return hit_material(hit, scene).color;
}

Vector calcul_normale(const Hit& hit, const GLTFScene& scene)
{
    Vector n;
    if (has_normals(hit, scene))
        // normale interpolee
        n = hit_normal(hit, scene);
    else
        // normale geometrique du triangle
        n = triangle_normal(hit, scene);

    return n;
}

// CALCUL DE L'OMBRE

int calcul_ombre(const std::vector<Triangle>& triangles, 
                   const Vector& n,
                   const Point& s,
                   const Point& position,
                   const bool& ombre)
{
    int v = 1;
    if (ombre)
    {
        Ray shadow_ray(position + 0.001f * n, s);
        for(int i= 0; i < int(triangles.size()); i++)
        {
            if(triangles[i].intersect(shadow_ray, 1 - .001f))
            {
                // on vient de trouver un triangle entre p et s. p est donc a l'ombre
                v = 0;
                break;  // pas la peine de continuer
            }
        }
    }
    return v;
}

// METHODES DE CALCUL DE COULEURS

Color color_lambert(const Hit& hit, const GLTFScene& scene, 
                    const Ray& ray, const Source& source, 
                    const std::vector<Triangle>& triangles,
                    const bool& ombre)
{
    Vector n = calcul_normale(hit, scene);

    Point s = source.s;
    Color emission = source.emission;

    // Point s = Point(0,1.5,1);
    // Color emission = hit_color(hit,scene)*3;

    Color k = hit_color(hit,scene);
    Point position = hit_position(hit, ray);

    Vector l = normalize(s - position);

    float cos_theta = std::abs(dot(n, l));

    // Calcul de l'ombre
    int v = calcul_ombre(triangles, n, s, position, ombre);

    return v * emission * cos_theta * k / M_PI;
}



Color color_modele_empirique(const Hit& hit, const GLTFScene& scene, 
                             const Ray& ray, const Source& source, 
                             const std::vector<Triangle>& triangles,
                             const bool& ombre)
{
    Vector n = calcul_normale(hit, scene);

    Point s = source.s;
    Color emission = source.emission;

    Color k = hit_color(hit,scene);
    Point position = hit_position(hit, ray);

    Vector o = normalize(Point(0,0,1) - position);
    Vector l = normalize(s - position);
    Vector h = normalize(o + l);

    float cos_theta = std::abs(dot(n, l));
    float cos_theta_h = std::abs(dot(n, h));


    const GLTFMaterial &material = hit_material(hit, scene);


    Color diffuse_color = (1 - material.metallic) * k;
    Color specular_color = (1 - material.metallic) * Color(0.04) + material.metallic * k;

    float alpha = 2 / pow(material.roughness, 4) - 2;

    // Somme de la couleur diffuse et speculaire
    Color fr = diffuse_color / M_PI + specular_color * std::max((alpha + 8) 
                / (8 * M_PI) * std::pow(cos_theta_h, alpha), 0.0);

    // Calcul de l'ombre
    int v = calcul_ombre(triangles, n, s, position, ombre);

    return v * emission * fr * cos_theta;
}