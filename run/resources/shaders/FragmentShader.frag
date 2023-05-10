#version 330 core

in vec2 oTexCoords;

uniform vec4 uColour;
uniform sampler2D uTextureDiffuse1;
uniform sampler2D uTextureSpecular1;

uniform float uTime;

out vec4 FragColor;

/// Original Author: Thomas Beet
/// <summary>
/// Basic fragment shader for texturing an object
/// </summary>
void main()
{
    FragColor = texture(uTextureDiffuse1, oTexCoords);
} 