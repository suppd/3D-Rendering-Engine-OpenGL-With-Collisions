#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor; 
uniform vec3 viewPos;

const float constant = 1.0;
const float linear = 0.09;
const float quadratic = 0.032;

void main()
{
    // Ambient lighting
    vec3 ambient = 0.1 * vec3(texture(texture_diffuse1, TexCoord));

    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoord));

    // Specular lighting
	float specularLight = 1.50f;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1); //last number shininess
    vec3 specular = specularLight * spec * lightColor;


    // Attenuation
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));


	vec3 result = (ambient + diffuse + specular) * attenuation;

    FragColor = vec4(result, 1.0);
}
