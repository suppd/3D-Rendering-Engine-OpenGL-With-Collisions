//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
out vec4 fragment_color;

// Uniforms for lighting
uniform vec3 lightPos; // Light position
uniform vec3 lightColor; // Light color
uniform float ambientStrength; // Ambient light strength

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;

void main( void ) {
	//fragment_color = vec4 (diffuseColor,1);
	
	 // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine lighting components with the solid color
    vec3 result = (ambient + diffuse) * diffuseColor;

    fragment_color = vec4(result, 1.0);
}
