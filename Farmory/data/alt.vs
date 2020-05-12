#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 aTransform;
out vec4 aVertexColor;

void main()
{
    gl_Position = aTransform * vec4(aPos, 1.0);
    aVertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}
