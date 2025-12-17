#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 Tex;

void main(){
   gl_Position = model * vec4(pos, 1.0);
   Tex = tex;
   Normal = normal;
   FragPos = vec3(model * vec4(pos, 1.0));
}
