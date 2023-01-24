#version 330
#ifdef VERTEX_SHADER
// vertex shader

layout ( location = 0) in vec3 position ; // attribut
layout ( location = 1) in vec2 texcoord; // attribut
layout ( location = 2) in vec3 normal ; // attribut

// transforms
uniform mat4 view;
uniform mat4 projection;
uniform mat4 models[128];


// sorties
out vec3 vertex_position;
out vec3 vertex_normal;
out vec2 vertex_texcoord;


void main (void)
{
        // recuperer la transform de l'instance
        mat4 model= models[gl_InstanceID];
        
        gl_Position = projection * view * model * vec4(position, 1);

        vertex_position = vec3 ( view * model * vec4 (position, 1.0) ) ;
        vertex_normal = vec3 ( view * model * vec4 (normal, 0) ) ;

        // Position de la texture
        vertex_texcoord.x = texcoord.x;
        vertex_texcoord.y = 1 - texcoord.y;
}
#endif

#ifdef FRAGMENT_SHADER
// fragment shader

const float PI = 3.141592654;
uniform vec3 source ;

in vec3 vertex_position ; // entree interpolee
in vec3 vertex_normal ; // entree interpolee
out vec4 fragment_color ;

in vec2 vertex_texcoord;
uniform sampler2D texture0;
uniform int id_material;

// materials
uniform float material_metallic;
uniform float material_roughness;
uniform vec4 material_color;

void main (void)
{
        vec3 o = normalize ( vec3(0,0,1) - vertex_position );
        vec3 l = normalize ( source - vertex_position );
        vec3 n = normalize ( vertex_normal );
        vec3 h = normalize ( o + l );

        float cos_theta = dot ( n, l );
        float cos_theta_h = max(dot(n, h), 0.0);

        vec3 diffuse_color = (1 - material_metallic) * material_color.xyz*3;
        vec3 specular_color = (1 - material_metallic) * vec3(0.04) + material_metallic * material_color.xyz*3;
        float alpha = 2 / pow(material_roughness,4) - 2;

        // Modele empirique
        vec3 frag_col = diffuse_color / PI + specular_color * max((alpha+8)/ (8 * PI) * pow(cos_theta_h,alpha), 0.0);
        vec3 color = frag_col * vec3(1) * cos_theta ; // emission = vec3(1)


        // gestion texture si elle existe
        if (id_material < 0)
        {
                fragment_color = vec4(color, material_color.a);
        }
        else 
        {
                vec4 color_tex = texture(texture0, vertex_texcoord) ;
                fragment_color= color_tex * vec4(color, material_color.a);
        }
}       
#endif

// TP2 M2 SI3D