// SuperQuadric.h
#ifndef SUPERQUADRIC_H
#define SUPERQUADRIC_H

#include <glm/glm.hpp>
#include "shape/GLShape.h"
#include "util/Shader.h"

enum class SuperSurfaceType {
    SUPER_ELLIPSOID = 4,
    SUPER_TOROID = 5
};

class SuperQuadric : public Renderable, public GLShape {
public:
    SuperQuadric(
        Shader* pShader,
        SuperSurfaceType surfaceType,
        const glm::vec3& center,
        float radius,
        float height,
        float majorRadius,
        const glm::vec3& color,
        float a, float b, float c, float m, float n, // For super-ellipsoid
        const glm::mat4& model
    );

    void render(float timeElapsedSinceLastFrame);

private:
    SuperSurfaceType surfaceType;
    glm::vec3 center;
    float radius;
    float height;
    float majorRadius;
    glm::vec3 color;

    // Super-Ellipsoid Parameters
    float a, b, c, m, n; // For Super-Ellipsoid

    // // For rendering
    // GLuint vao, vbo;
};

#endif // SUPERQUADRIC_H
