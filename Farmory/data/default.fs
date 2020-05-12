#version 330 core

in vec2 aUvCoords;

uniform float aMixValue;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;

out vec4 aOutFragColor;

void main()
{
    aOutFragColor = mix(texture(aTexture0, aUvCoords), texture(aTexture1, aUvCoords), aMixValue);
}
