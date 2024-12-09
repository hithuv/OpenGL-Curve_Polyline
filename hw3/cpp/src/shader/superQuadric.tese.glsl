#version 410 core

layout (quads, equal_spacing, ccw) in;

out vec3 ourNormal;
out vec3 ourFragPos;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Uniforms for surface parameters
uniform int surfaceType;       // 4: Super-Ellipsoid, 5: Super-Toroid
uniform float radius;          // Radius for Super-Ellipsoid or Super-Toroid
uniform float height;          // Height for Super-Ellipsoid or Super-Toroid
uniform float majorRadius;     // Major radius for Super-Toroid
uniform vec3 center;           // Center of the shape
uniform vec3 color;            // Color of the surface

// Super-Ellipsoid parameters
uniform float a, b, c;         // For Super-Ellipsoid
uniform float m, n;            // For Super-Ellipsoid

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

    

    if (surfaceType == 4) // Super-Ellipsoid
    {
        // Super-Ellipsoid Parametric Equations
        float thetaEllipsoid = kPi * v;
        pos = center + vec3(
            a * cos(thetaEllipsoid) * pow(abs(cos(phi)), n),
            b * cos(thetaEllipsoid) * pow(abs(sin(phi)), m),
            c * sin(thetaEllipsoid)
        );

        // Compute the normal for Super-Ellipsoid
        normal = normalize(vec3(model * vec4(pos - center, 0.0f)));
    }
    else if (surfaceType == 5) // Super-Toroid
    {
        // Super-Toroid Parametric Equations
        pos = center + vec3(
            (majorRadius + radius * cos(m * v)) * cos(u),
            radius * sin(m * v),
            (majorRadius + radius * cos(m * v)) * sin(u)
        );

        // Compute the normal for Super-Toroid
        normal = normalize(vec3(
            cos(u) * cos(m * v),
            sin(m * v),
            sin(u) * cos(m * v)
        ));
    }

    // Final position transformation
    gl_Position = projection * view * model * vec4(pos, 1.0f);

    // Pass the normal and position to the fragment shader
    ourFragPos = vec3(model * vec4(pos, 1.0f));
    ourNormal = normalize(vec3(transpose(inverse(mat3(model))) * normal));
    ourColor = color;
}
