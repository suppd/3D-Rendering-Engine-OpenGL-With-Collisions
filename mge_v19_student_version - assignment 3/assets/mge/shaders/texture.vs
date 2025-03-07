//DIFFUSE COLOR VERTEX SHADER

#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
    FragPos = vec3(modelMatrix * vec4(vertex, 1.0)); // Calculate world position of the vertex
    Normal = mat3(transpose(inverse(modelMatrix))) * normal; // Transform the normal to world space
	TexCoord = uv;
}
