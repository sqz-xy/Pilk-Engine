#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

uniform vec2 uScale;

uniform float uTime;

out vec2 oTexCoords;
out vec3 oNormal;
out vec3 oFragPos;

void main()
{
    oTexCoords = aTexCoords.xy * uScale;

    vec3 pos = aPos;
    oFragPos = vec3(uModel * vec4(aPos, 1.0));
    oNormal = vec3(transpose(inverse(uModel)) * vec4(aNormal, 1.0));
    gl_Position = uProj * uView * uModel * vec4(pos, 1.0);
}