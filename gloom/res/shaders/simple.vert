#version 430 core
in vec3 position;
in vec3 normal;
in vec4 color;

uniform float time = 0;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 mvp;

out vec3 vert_position;
out vec3 vert_normal;
out vec4 vert_color;
void main()
{
    vec4 pos = vec4(position, 1.0f);

    vert_position = (m2w * pos).xyz;
    vert_normal = mat3(transpose(inverse(m2w))) * normal;
    vert_color = color;
    gl_Position = mvp * pos;
}
