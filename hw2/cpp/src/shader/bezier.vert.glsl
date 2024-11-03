#version 410 core

layout (location = 0) in vec2 aPos;

uniform float windowWidth;
uniform float windowHeight;

out vec2 vPos;

void main()
{
    vPos = aPos;
}
