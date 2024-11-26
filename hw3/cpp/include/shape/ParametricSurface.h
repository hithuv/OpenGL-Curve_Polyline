// ParametricSurface.h
#ifndef PARAMETRICSURFACE_H
#define PARAMETRICSURFACE_H

#include <glm/glm.hpp>
#include "util/Shader.h"
#include "shape/GLShape.h"

enum SurfaceType {
    SPHERE = 0,
    CYLINDER = 1,
    CONE = 2,
    TORUS = 3
};

class ParametricSurface : public Renderable, public GLShape {
public:
    /// Constructor
    /// @param pShader Pointer to the shader program
    /// @param surfaceType Type of the surface (0: Sphere, 1: Cylinder, 2: Cone, 3: Torus)
    /// @param center Center position of the surface
    /// @param radius Radius (for sphere and cylinder base; minor radius for torus)
    /// @param height Height (for cylinder and cone; ignored for torus)
    /// @param majorRadius Major radius (only for torus; ignored otherwise)
    /// @param color Color of the surface
    /// @param model Transformation matrix
    ParametricSurface(
        Shader* pShader,
        SurfaceType surfaceType,
        const glm::vec3& center,
        float radius,
        float height,
        float majorRadius, // Only used for torus
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
    float majorRadius; // Only for torus
    glm::vec3 color;
};
#endif // PARAMETRICSURFACE_H
