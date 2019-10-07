#version 430 core
in vec3 position;   //model space position
in vec3 normal;     //model space normals
in vec4 color;      //model space color - technically, not differing from other spaces

uniform mat4 mvp;   //pre-computed (CPU-side) model-view-projection matrix
// In addition to supplying the shader with the mvp, 
// I supply it with the other matrices as well, to allow for phong-shading
uniform mat4 m2w;           // model space to world space - model matrix 

out vec3 vert_wPosition;    //world position
out vec3 vert_wNormal;      //world normal
out vec4 vert_color;
void main()
{
    vec4 pos = vec4(position, 1.0f);

    vert_wPosition = (m2w * pos).xyz;
    vert_wNormal = mat3(transpose(inverse(m2w))) * normal;
    vert_color = color;
    gl_Position = mvp * pos;
}
