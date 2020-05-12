#version 330 core

uniform vec4 aUColor;

out vec4 aOutFragColor;

void main()
{
    //aOutFragColor = vec4(1.0, 1.0, 0.0, 1.0);
    aOutFragColor = aUColor;
}
