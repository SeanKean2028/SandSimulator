#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 Tex;

out vec4 FragColor;

uniform vec4 color = vec4(0.97f, 0.98f, 0.62f, 1);
void main(){
  FragColor = color;
}