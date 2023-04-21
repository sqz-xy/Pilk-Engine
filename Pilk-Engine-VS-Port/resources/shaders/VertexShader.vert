#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

uniform float uTime;

out vec2 oTexCoords;

void main()
{
    oTexCoords = aTexCoords;

    vec3 pos = aPos;
    gl_Position = uProj * uView * uModel * vec4(pos, 1.0);
}