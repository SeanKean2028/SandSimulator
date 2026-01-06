#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 instanceModelRow0;
layout (location = 4) in vec4 instanceModelRow1;
layout (location = 5) in vec4 instanceModelRow2;
layout (location = 6) in vec4 instanceModelRow3;

layout (location = 7) in vec4 instanceColor;
out vec4 vColor;

void main()
{

    mat4 instanceModel = mat4(
        instanceModelRow0,
        instanceModelRow1,
        instanceModelRow2,
        instanceModelRow3
    );

    gl_Position = instanceModel * vec4(aPos, 1.0);
    
    vColor = instanceColor;
}