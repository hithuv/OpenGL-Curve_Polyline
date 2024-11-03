#version 410 core

layout (isolines, equal_spacing, ccw) in;

in vec2 tcPos[];
out vec3 teColor;

uniform float windowWidth;
uniform float windowHeight;
uniform vec3 color;

vec2 bezier(float t)
{
    float u = 1.0 - t;
    return u*u*u*tcPos[0] + 3*u*u*t*tcPos[1] + 3*u*t*t*tcPos[2] + t*t*t*tcPos[3];
}

void main()
{
    float t = gl_TessCoord.x;
    vec2 pos = bezier(t);
    
    vec2 normalizedPos = vec2(pos.x / windowWidth * 2.0 - 1.0, 
                              pos.y / windowHeight * 2.0 - 1.0);
    gl_Position = vec4(normalizedPos, 0.0, 1.0);
    teColor = color;
}
