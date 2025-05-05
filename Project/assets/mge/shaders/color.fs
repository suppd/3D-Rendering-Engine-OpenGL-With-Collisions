#version 330 

uniform vec3 diffuseColor;
out vec4 FragColor;

// Uniforms for lighting, attenuation, and specular reflection
uniform vec3 lightPos; // Light position
uniform vec3 lightColor; // Light color
uniform float ambientStrength; // Ambient light strength
const float constant = 1.0;
const float linear = 0.09;
const float quadratic = 0.032;
uniform vec3 viewPos;


// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;

void main(void) {
    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
	float specularLight = 1.50f; // brightness regardless of shininess
    vec3 viewDir = normalize(viewPos-FragPos); //direction from the frag to the viewer
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); //last number shininess, by raising to the power we emphazise the diffrence in intensity of aligned/misaligned
	 //calculate speculair intesity based on angle betwen view and reflect direction using max to keep out negative values 
	 //and the dot product to calculate the angle with a number like 16 to exponentially make the speculair component "sharper" aka shininess
    vec3 specular = specularLight * spec * lightColor;

    // Attenuation
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Combine lighting components with the solid color and apply attenuation
    vec3 result = (ambient + diffuse + specular) * diffuseColor * attenuation;

    FragColor = vec4(result, 1.0);
}
