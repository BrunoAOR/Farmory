#version 330 core

in vec4 aVertexColor;

out vec4 aOutFragColor;

void main()
{
    //aOutFragColor = vec4(1.0, 0.5, 0.2, 1.0);
    aOutFragColor = aVertexColor;
}
