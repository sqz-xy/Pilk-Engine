#version 330 core

in vec2 oTexCoords;

uniform vec4 uColour;
uniform sampler2D uTextureDiffuse1;
uniform sampler2D uTextureSpecular1;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTextureDiffuse1, oTexCoords);
} 