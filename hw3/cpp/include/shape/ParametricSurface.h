#ifndef PARAMETRICSURFACE_H
#define PARAMETRICSURFACE_H

#include <glm/glm.hpp>
#include "util/Shader.h"
#include "shape/GLShape.h"

enum SurfaceType {
    SPHERE = 0,
    CYLINDER = 1,
    CONE = 2
};

class ParametricSurface : public Renderable, public GLShape {
public:
    /// Constructor
    /// @param pShader Pointer to the shader program
    /// @param surfaceType Type of the surface (0: Sphere, 1: Cylinder, 2: Cone)
    /// @param center Center position of the surface
    /// @param radius Radius (for sphere and cylinder base)
    /// @param height Height (for cylinder and cone)
    /// @param color Color of the surface
    /// @param model Transformation matrix
    ParametricSurface(
        Shader* pShader,
        SurfaceType surfaceType,
        const glm::vec3& center,
        float radius,
        float height,
        const glm::vec3& color,
        const glm::mat4& model
    );

    ~ParametricSurface() noexcept override = default;

    void render(float timeElapsedSinceLastFrame) override;

private:
    SurfaceType surfaceType;
    glm::vec3 center;
    float radius;
    float height;
    glm::vec3 color;
};

#endif // PARAMETRICSURFACE_H
