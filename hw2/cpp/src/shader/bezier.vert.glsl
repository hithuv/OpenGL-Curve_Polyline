#version 330 core

layout (location = 0) in vec2 aPos;

uniform float windowWidth;
uniform float windowHeight;
uniform vec3 color;

out vec3 fragColor;

void main()
{
    // Convert from screen coordinates to clip space
    vec2 clipSpace = (aPos / vec2(windowWidth, windowHeight)) * 2.0 - 1.0;
    
    // Flip y-coordinate because screen coordinates are y-down
    // clipSpace.y = -clipSpace.y;
    
    // gl_Position = vec4(clipSpace, 0.0, 1.0);
    vec2 normalizedPos = vec2(aPos.x / windowWidth * 2.0 - 1.0, 
                              aPos.y / windowHeight * 2.0 - 1.0);
    gl_Position = vec4(normalizedPos, 0.0, 1.0);
    fragColor = color;
}



