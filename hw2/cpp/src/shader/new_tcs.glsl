#version 410 core

layout (vertices = 4) out;

in vec2 vPos[];
out vec2 tcPos[];

uniform float tessLevel;

void main()
{
    tcPos[gl_InvocationID] = vPos[gl_InvocationID];
    
    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = 1.0;
        gl_TessLevelOuter[1] = tessLevel;
    }
}
