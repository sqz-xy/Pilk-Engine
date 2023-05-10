#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

uniform float uTime;
uniform vec2 uScale;

out vec2 oTexCoords;
out vec3 oFragPos;

/// Original Author: Thomas Beet
/// <summary>
/// Billboarding shader to make sure some entities always face the camera
/// </summary>
void main()
{
    oTexCoords = aTexCoords.xy * uScale;
    oTexCoords = vec2(aTexCoords.x, 1.0 - aTexCoords.y);

    mat4 modelView = uModel * uView;

    modelView[0][0] = 1;
    modelView[0][1] = 0;
    modelView[0][2] = 0;

    modelView[1][0] = 0;
    modelView[1][1] = 1;
    modelView[1][2] = 0;

    modelView[2][0] = 0;
    modelView[2][1] = 0;
    modelView[2][2] = 1;

    vec4 p = modelView * vec4(aPos, 1.0);

    oFragPos = vec3(uModel * p);
    gl_Position = uProj * p;
}