#version 410 core
layout (isolines, equal_spacing, ccw) in;

uniform mat4 projection;
uniform mat4 view;

vec4 bezier(float t)
{
    float u = 1.0 - t;
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;
    return u*u*u*p0 + 3*u*u*t*p1 + 3*u*t*t*p2 + t*t*t*p3;
}

void main()
{
    float t = gl_TessCoord.x;
    vec4 pos = bezier(t);
    gl_Position = projection * view * pos;
}
