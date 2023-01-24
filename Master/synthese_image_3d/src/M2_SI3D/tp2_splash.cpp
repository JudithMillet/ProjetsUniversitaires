
//! \file TP2 Affichage d'une scène 3D depuis un GLTF

#include "wavefront.h"
#include "texture.h"

#include "orbiter.h"
#include "draw.h"
#include "app_time.h"
#include "gltf.h"

#include "program.h"
#include "uniforms.h"

#include "widgets.h"  // interface minimaliste

struct GLTFElements
{
    std::vector<int> indices_count;
    std::vector<GLuint> vaos;

    std::vector<int> index_materials;


    void create(const GLTFMesh &mesh)
    {
        // parcours les primitives du mesh
        for (unsigned int primitive_id = 0; primitive_id < mesh.primitives.size(); primitive_id++)
        {
            const GLTFPrimitives& primitive = mesh.primitives[primitive_id];
            
            indices_count.push_back(primitive.indices.size());

            // matiere des primitives / groupe de triangles
            index_materials.push_back(primitive.material_index);

            GLuint vao;

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            GLuint vertex_buffer = 0;
            glGenBuffers(1, &vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vec3) * primitive.positions.size() +
                             sizeof(vec2) * primitive.texcoords.size() +
                             sizeof(vec3) * primitive.normals.size(),
                         nullptr, GL_STATIC_DRAW);

            // transfere les positions des sommets
            size_t offset= 0;
            size_t size = primitive.positions.size() * sizeof(vec3);
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, primitive.positions.data());
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) offset);
            glEnableVertexAttribArray(0);

            // transfere les texcoords des sommets
            offset += size;
            size = primitive.texcoords.size() * sizeof(vec2);
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, primitive.texcoords.data());
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) offset);
            glEnableVertexAttribArray(1);

            // transfere les normales des sommets
            offset += size;
            size = primitive.normals.size() * sizeof(vec3);
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, primitive.normals.data());
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, /* stride */ 0, (const GLvoid *) offset);
            glEnableVertexAttribArray(2);

            GLuint index_buffer = 0;
            // index_buffer
            if(primitive.indices.size() > 0)
            {
                glGenBuffers(1, &index_buffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             sizeof(unsigned int) * primitive.indices.size(),
                             primitive.indices.data(), GL_STATIC_DRAW);
            }

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            vaos.push_back(vao);
        }
    }

    void release()
    {
        for (GLuint vao : vaos)
            glDeleteVertexArrays(1, &vao);
    }
};

class TP : public AppTime
{
public:
    TP(const char* _filename, const int &splashMesh) : AppTime(1024, 640, 3, 3, 4) {
        filename = _filename;

        if (splashMesh)
            isSplashMesh = true;
    }

    // Charge le matérial (metal et rugosité) dans le program
    void load_program_materials(GLTFMaterial material, uint i)
    {
        // material metallic
        GLint location = glGetUniformLocation(program, "material_metallic");
        glUniform1f(location, material.metallic);

        // material roughness
        location = glGetUniformLocation(program, "material_roughness");
        glUniform1f(location, material.roughness);
    }
    
    // creation des objets de l'application
    int init( )
    {
        scene = read_gltf_scene(filename);

        // Gestion des données des maillages de la scène
        for (unsigned mesh_id = 0; mesh_id < scene.meshes.size(); mesh_id++)
        {
            // recupere le mesh du noeud
            const GLTFMesh &mesh = scene.meshes[mesh_id];

            // chaque info du mesh 
            GLTFElements element;
            element.create(mesh);

            elements.push_back(element);
        }

        // Gestion des instances
        for (GLTFInstances &instance : scene.instances())
        {
            // Draw seulement si moins de 128 instances
            if (instance.transforms.size() < 128)
            {
                instances.push_back(instance);
            }
        }

        // Gestion des textures
        for (ImageData texture : read_gltf_images(filename))
        {
            textures.push_back(make_texture(0, texture));
        }

        // Initialisation du shader
        program = read_program("data/shaders/shader_splash.glsl");
        program_print_errors(program);

        
        // Initialisation de la camera
        Point pmin, pmax;
        scene.bounds(pmin, pmax);
        m_camera.lookat(pmin, pmax);
        m_camera.projection(window_width(), window_height(), 25);
                
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        release_program(program);

        for (GLTFElements element : elements)
            element.release();

        for (unsigned int i = 0; i < textures.size(); i++)
            glDeleteTextures(1, &textures[i]);
        
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // permet d'avoir de la transparence
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // deplace la camera
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            m_camera.rotation(mx, my);
        else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

        // Chargement du bon shader
        glUseProgram(program);

        // Chargement des tranforms communes
        Transform view = m_camera.view();
        Transform projection = m_camera.projection();

        GLint location = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(location, 1, GL_TRUE, projection.data());

        location = glGetUniformLocation(program, "view");
        glUniformMatrix4fv(location, 1, GL_TRUE, view.data());

        // Pour l'eau
        GLTFInstances sea_mesh;


        for (GLTFInstances &instance : instances)
        {
            // On passe les oiseaux
            if (isSplashMesh && (instance.mesh_index == 3 || instance.mesh_index == 4))
            {
                continue;
            }

            // On passe l'eau pour l'instant
            if (isSplashMesh && instance.mesh_index == 30)
            {
                sea_mesh = instance;
                continue;
            }

            // Chargement des transforms models 
            location = glGetUniformLocation(program, "models");
            glUniformMatrix4fv(location, 128, GL_TRUE, (GLfloat *)instance.transforms.data());

            // pour chaque objet d'indice instance.mesh_index
            for (size_t i = 0; i < elements[instance.mesh_index].vaos.size(); i++)
            {
                // On récupère le GLTFMaterial de la scène
                int index_material = elements[instance.mesh_index].index_materials[i];
                GLTFMaterial material = scene.materials[index_material];

                // chargement des materials
                load_program_materials(material, i);


                // Cas instance sans texture
                if (material.color_texture == -1 || material.color_texture > textures.size())
                {
                    location = glGetUniformLocation(program, "id_material");
                    glUniform1i(location, -1);

                    // Evite la mer
                    if (!isSplashMesh && instance.mesh_index != 30)
                    {
                        Color col = material.color;
                        location = glGetUniformLocation(program, "material_color");
                        glUniform4f(location, col.r, col.g, col.b, col.a);
                    }
                    
                }
                else
                {
                    location = glGetUniformLocation(program, "id_material");
                    glUniform1i(location, material.color_texture);

                    location = glGetUniformLocation(program, "material_color");
                    glUniform4f(location, 1, 1, 1, 1);

                    GLuint texture = textures[material.color_texture];
                    program_use_texture(program, "texture0", 0, texture);
                }

                // Dessine les objets
                glBindVertexArray(elements[instance.mesh_index].vaos[i]);
                glDrawElementsInstanced(
                    GL_TRIANGLES,
                    elements[instance.mesh_index].indices_count[i],
                    GL_UNSIGNED_INT, 0, instance.transforms.size());
            }
        }

        // Eau dessinée en dernier
        if (isSplashMesh)
        {
            location = glGetUniformLocation(program, "models");
            glUniformMatrix4fv(location, 128, GL_TRUE, (GLfloat *)sea_mesh.transforms.data());

            // On récupère le GLTFMaterial de la scène
            int index_material = elements[sea_mesh.mesh_index].index_materials[0];
            GLTFMaterial material = scene.materials[index_material];

            load_program_materials(material, 0);

            location = glGetUniformLocation(program, "material_color");
            glUniform4f(location, 0.f, 0.6f, 0.8f, 0.6f);

            location = glGetUniformLocation(program, "id_material");
            glUniform1i(location, material.color_texture);

            glBindVertexArray(elements[sea_mesh.mesh_index].vaos[0]);
            glDrawElementsInstanced(
                GL_TRIANGLES, elements[sea_mesh.mesh_index].indices_count[0],
                GL_UNSIGNED_INT, 0, sea_mesh.transforms.size());
        }

        return 1;
    }

protected:
    const char *filename;

    GLuint program = 0;

    GLTFScene scene;

    std::vector<GLTFElements> elements;
    std::vector<GLTFInstances> instances;
    std::vector<GLuint> textures;

    Orbiter m_camera;


    bool isSplashMesh = false;
};


int main( int argc, char **argv )
{
    const char *filename = "data/gltf/splash3_bake/splash3_bake.gltf";
    int splashMesh = 1;

    if (argc > 1) filename = argv[1];
    if (argc > 2) splashMesh = atoi(argv[2]);

    TP tp(filename, splashMesh);
    tp.run();
    
    return 0;
}
