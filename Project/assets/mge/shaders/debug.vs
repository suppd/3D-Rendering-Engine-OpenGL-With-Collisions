#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragColor;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    fragColor = color;
}