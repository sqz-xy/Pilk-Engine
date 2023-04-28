#version 330 core

in vec2 oTexCoords;
in vec3 oFragPos;

uniform vec4 uColour;
uniform sampler2D uTextureDiffuse1;
uniform sampler2D uTextureSpecular1;

uniform float uTime;

out vec4 FragColor;

void main()
{
    vec2 uv = oTexCoords;

    uv.y = 1.0 - uv.y;
    uv.x = (oTexCoords.x - 0.5) + 0.5;

    uv.x += 0.1 * uv.y * sin(10 * (uv.y + uTime));
    uv.y += 0.1 * uv.y * sin(10 * (uv.x + uTime));

    FragColor = texture(uTextureDiffuse1, uv);
} 