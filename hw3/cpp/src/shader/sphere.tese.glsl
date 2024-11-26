#version 410 core

layout (quads, equal_spacing, ccw) in;
//layout (quads, equal_spacing, ccw, point_mode) in;

out vec3 ourNormal;
out vec3 ourFragPos;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Add the missing uniform declarations
uniform int surfaceType;    // To determine which surface to render
uniform float height;       // Applicable for cylinder and cone

uniform vec3 center;
uniform float radius;
uniform vec3 color;

const float kPi = 3.14159265358979323846f;

void main()
{
    // Use gl_in[0] to prevent the shader from being optimized out
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float phi = 2.0f * kPi * u;

    vec3 pos;
    vec3 normal;

    if (surfaceType == 0)
    {
        // Sphere parameters
        float theta = kPi * v;
        pos = center + vec3(radius * sin(theta) * cos(phi),
                           radius * sin(theta) * sin(phi),
                           radius * cos(theta));
        normal = normalize(vec3(model * vec4(pos, 0.0f)));
    }
    else if (surfaceType == 1)
    {
        // Cylinder parameters
        float y = height * v;
        pos = center + vec3(radius * cos(phi), y, radius * sin(phi));
        normal = normalize(vec3(radius * cos(phi),
                                 0.0f,
                                 radius * sin(phi)));
    }
    else if (surfaceType == 2)
    {
        // Cone parameters
        float y = height * v;
        float currentRadius = radius * (1.0f - v); // Radius decreases linearly
        pos = center + vec3(currentRadius * cos(phi),
                           y,
                           currentRadius * sin(phi));
        // Calculate the slope for the normal (assuming a 45-degree slope)
        float slope = radius / height;
        normal = normalize(vec3(radius * cos(phi),
                                 slope,
                                 radius * sin(phi)));
    }

    gl_Position = projection * view * model * vec4(pos, 1.0f);

    ourFragPos = vec3(model * vec4(pos, 1.0f));
    ourNormal = normalize(vec3(transpose(inverse(mat3(model))) * normal));
    ourColor = color;
}
