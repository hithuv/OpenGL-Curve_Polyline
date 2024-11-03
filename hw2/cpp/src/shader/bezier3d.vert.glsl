//#version 410 core
//layout (location = 0) in vec3 aPos;

//void main()
//{
//    gl_Position = vec4(aPos, 1.0);
//}

#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}
