#version 430 core
in vec3 vert_wPosition;
in vec3 vert_wNormal;
in vec4 vert_color;

uniform float specularity = 0.5;
uniform float time = 0;


//========= LIGHTS =========
uniform vec3 light_color = vec3(0.93, 0.89, 0.88); 

const vec3 light_direction = normalize(vec3(0.8, -0.5, 0.6)); 

//========= CAMERA =========
uniform mat4 mvp;   //pre-computed (CPU-side) model-view-projection matrix
// In addition to supplying the shader with the mvp, 
// I supply it with the other matrices as well, to allow for phong-shading
uniform mat4 m2w;           // model space to world space - model matrix 
uniform mat4 view;          // view matrix
uniform mat4 projection;    // projection matrix

uniform vec3 view_wPosition; //the world space camera position


//========= ACTION =========
vec3 directional_light(in vec3 light_dir, in vec3 intensities) {
	//Ambience
	float ambient_strength = 0.2;
	vec3 ambient = ambient_strength * intensities;

	//Diffuse
    vec3 norm = normalize(vert_wNormal);
    light_dir = normalize(-light_dir); //direction to the light source
    float lambert = max(0.0, dot(norm, light_dir));
    vec3 diffuse = lambert * intensities;

	//Specularity
    vec3 view_dir = normalize(vert_wPosition - view_wPosition); //direction to the camera
    vec3 reflect_dir = reflect(light_dir, norm);
    float spec_power = pow(max(dot(view_dir, reflect_dir ), 0.0), 32);
    vec3 specular = specularity * spec_power *  intensities;

	return (ambient + diffuse + specular);
}

out vec4 frag_result;
void main() {

    vec3 animated_light_direction = vec3(cos(light_direction.x+time), light_direction.y, sin(light_direction.z+time));

    frag_result = vec4(vert_color.rgb * directional_light(animated_light_direction, light_color), vert_color.a);
    // Old Lambertian shading:
    // frag_result = vec4(vert_color.rgb * max(0, dot(normalize(vert_normal), -light_direction)), vert_color.a); 
}