//! \file TP3 lancer de rayon

#include "utils.h"


int main( const int argc, const char **argv )
{
    const char *filename = "data/gltf/cornell.gltf";
    if(argc > 1)
        filename = argv[1];
    
    // charge la scene
    GLTFScene scene = read_gltf_scene(filename);
    
    // recupere les sources
    std::vector<Source> sources;

    // recupere les triangles
    std::vector<Triangle> triangles;

    // Parcours dans la scene
    for(unsigned node_id = 0; node_id < scene.nodes.size(); node_id++)
    {
        const GLTFNode& node= scene.nodes[node_id];
        
        const Transform& model = node.model;
        int mesh_id = node.mesh_index;
        
        const GLTFMesh& mesh = scene.meshes[mesh_id];
        for(unsigned primitive_id = 0; primitive_id < mesh.primitives.size(); primitive_id++)
        {
            const GLTFPrimitives& primitives= mesh.primitives[primitive_id];
            for(unsigned i = 0; i + 2 < primitives.indices.size(); i+=3)
            {
                const GLTFMaterial& material = scene.materials[primitives.material_index];

                // indice des sommets
                int a = primitives.indices[i];
                int b = primitives.indices[i+1];
                int c = primitives.indices[i+2];
                
                // position des sommets
                Point pa = primitives.positions[a];
                Point pb = primitives.positions[b];
                Point pc = primitives.positions[c];
                
                // transforme les sommets dans le repere de la scene
                pa = model( Point(pa) );
                pb = model( Point(pb) );
                pc = model( Point(pc) );
                
                triangles.push_back( Triangle(pa, pb, pc, mesh_id, primitive_id, node_id, i/3) );

                if (material.emission.r + material.emission.g + material.emission.b > 0)
                {
                    Point centre = (pa + pb + pc) / 3;
                    sources.push_back({ centre, material.emission });
                }
            }
        }
    }
    assert(triangles.size());
    assert(sources.size());
    
    // recupere les matrices de la camera gltf
    assert(scene.cameras.size());
    Transform view = scene.cameras[0].view;
    Transform projection = scene.cameras[0].projection;
    
    // cree l'image en respectant les proportions largeur/hauteur de la camera gltf
    int width = 1024;
    int height = width / scene.cameras[0].aspect;
    Image image(width, height);
    
    // transformations
    Transform model = Identity();
    Transform viewport = Viewport(image.width(), image.height());
    Transform inv = Inverse(viewport * projection * view * model);
    
    // c'est parti, parcours tous les pixels de l'image
    for(int y = 0; y < image.height(); y++)
    for(int x = 0; x < image.width(); x++)
    {
        // generer le rayon
        Point origine = inv(Point(x, y, 0));
        Point extremite = inv(Point(x, y, 1));
        Ray ray(origine, extremite);
        
        // calculer les intersections avec tous les triangles
        Hit hit;
        for(unsigned i = 0; i < triangles.size(); i++)
        {
            if(Hit h = triangles[i].intersect(ray, hit.t))
                // ne conserve que l'intersection la plus proche de l'origine du rayon
                // attention !! h.t <= hit.t !! sinon ca ne marche pas...
                hit = h;
        }
        
        if(hit)
        {
            // PARTIE 1
            // coordonnees barycentriques de l'intersection
            // image(x, y)= Color(1 - hit.u - hit.v, hit.u, hit.v);


            // PARTIE 2
            // 1 - normale interpolee
            // Vector n = hit_normal(hit, scene);
            // image(x, y) = Color(n.x, n.y, n.z);

            // 2 -  Avec Lambert (ancien code avec une seule source)
            // Color color = color_lambert(hit, scene, ray);
            // image(x,y) = Color(color,1);


            // 3 - Avec le modèle empirique et plusieurs sources 
            hit.set_instance_id(triangles[hit.triangle_id].instance_id);
            for (Source source : sources)
            {
                // Color color = color_lambert(hit, scene, ray, source, triangles, trur);
                Color color = color_modele_empirique(hit, scene, ray, source, triangles, true);

                image(x,y) = image(x, y) + Color(color,1);
            }
        }
    }
    
    write_image(image, "data/resultats/rendu.png");
    return 0;
}
