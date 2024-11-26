#version 410 core

layout (quads, equal_spacing, ccw) in;
//layout (quads, equal_spacing, ccw, point_mode) in;

out vec3 ourNormal;
out vec3 ourFragPos;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Uniforms for surface parameters
uniform int surfaceType;       // 0: Sphere, 1: Cylinder, 2: Cone, 3: Torus
uniform float height;          // Applicable for Cylinder and Cone
uniform float majorRadius;     // Applicable for Torus
uniform vec3 center;
uniform float radius;          // Minor radius for Torus, or radius for Sphere and Cylinder
uniform vec3 color;

const float kPi = 3.14159265358979323846f;

void main()
{
    // Use gl_in[0] to prevent the shader from being optimized out
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float phi = 2.0f * kPi * u;
    float theta = 2.0f * kPi * v;

    vec3 pos;
    vec3 normal;

    if (surfaceType == 0)
    {
        // Sphere parameters
        float thetaSphere = kPi * v;
        pos = center + vec3(radius * sin(thetaSphere) * cos(phi),
                           radius * sin(thetaSphere) * sin(phi),
                           radius * cos(thetaSphere));
        normal = normalize(vec3(model * vec4(pos - center, 0.0f)));
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
    else if (surfaceType == 3)
    {
        // Torus parameters
        float R = majorRadius; // Major radius
        float r = radius;       // Minor radius

        pos = center + vec3((R + r * cos(phi)) * cos(theta),
                           r * sin(phi),
                           (R + r * cos(phi)) * sin(theta));

        // Compute normals
        normal = normalize(vec3(cos(phi) * cos(theta),
                                 sin(phi),
                                 cos(phi) * sin(theta)));
    }

    gl_Position = projection * view * model * vec4(pos, 1.0f);

    ourFragPos = vec3(model * vec4(pos, 1.0f));
    ourNormal = normalize(vec3(transpose(inverse(mat3(model))) * normal));
    ourColor = color;
}
