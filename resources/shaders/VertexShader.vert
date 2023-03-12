#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;



void main()
{
    vec4 position = vec4(aPos, 1) * uModel * uView; //* uProj;
    gl_Position = position;
}