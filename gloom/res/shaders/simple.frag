#version 430 core
in vec3 vert_position;
in vec3 vert_normal;
in vec4 vert_color;

uniform vec4 tint = vec4(1,1,1,1);
uniform float specularity = 0.125;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_direction = normalize(vec3(0.8, -0.5, 0.6)); 
uniform vec3 light_color = vec3(0.93, 0.89, 0.88); 


vec3 directional_light(in vec3 lightDir, in vec3 intensities) {
	//Ambience
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * intensities;

	//Diffuse
    vec3 norm = normalize(vert_normal);
    lightDir = normalize(lightDir);
    float lambert = max(0.0, dot(norm, -lightDir));
    vec3 diffuse = lambert * intensities;

	//Specularity
    vec3 viewDir = normalize(-view[3].xyz - vert_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specPower = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularity * specPower *  intensities;

	return (ambient + diffuse + specular);
}


out vec4 color;
void main() {
    color = vec4(vert_color.rgb * directional_light(light_direction, light_color), vert_color.a);
//    color = vec4(vert_color.rgb * max(0, dot(normalize(vert_normal), -light_direction)), vert_color.a); 
}