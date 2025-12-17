#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 Tex;

out vec4 FragColor;

uniform vec3 color;
void main(){
  FragColor = vec4(color,1);
}