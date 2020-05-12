#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

uniform mat4 aModelMatrix;
uniform mat4 aViewMatrix;
uniform mat4 aProjectionMatrix;

out vec2 aUvCoords;

void main()
{
    gl_Position = aProjectionMatrix * aViewMatrix * aModelMatrix * vec4(aPos, 1.0);
    aUvCoords = aUv;
}
