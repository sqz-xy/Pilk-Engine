#version 330 core

in vec2 TexCoords;

uniform vec4 uColour;
uniform sampler2D uTextureDiffuse1;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTextureDiffuse1, TexCoords);
} 