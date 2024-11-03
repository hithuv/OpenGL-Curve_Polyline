#version 410 core
layout (location = 0) in vec2 aPos;

uniform float windowWidth;
uniform float windowHeight;

void main()
{
    vec2 normalizedPos = vec2(aPos.x / windowWidth * 2.0 - 1.0, 
                              aPos.y / windowHeight * 2.0 - 1.0);
    gl_Position = vec4(normalizedPos, 0.0, 1.0);
}
