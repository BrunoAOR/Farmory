#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

uniform mat4 aTransform;

out vec2 aUvCoords;

void main()
{
    gl_Position = aTransform * vec4(aPos, 1.0);
    aUvCoords = aUv;
}
