#version 410 core

in vec3 teColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(teColor, 1.0);
}
