// ParametricSurface.cpp
#include "shape/ParametricSurface.h"
#include "util/Shader.h"
#include <glad/glad.h> // Ensure GLAD is included and initialized

ParametricSurface::ParametricSurface(
    Shader* pShader,
    SurfaceType surfaceType,
    const glm::vec3& center,
    float radius,
    float height,
    float majorRadius,
    const glm::vec3& color,
    const glm::mat4& model
) : Renderable(),
    GLShape(pShader, model),
    surfaceType(surfaceType),
    center(center),
    radius(radius),
    height(height),
    majorRadius(majorRadius),
    color(color)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Define a single control point (dummy vertex)
    float nullVertex = 0.0f;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), reinterpret_cast<void*>(0));

    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &nullVertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParametricSurface::render(float timeElapsedSinceLastFrame)
{
    pShader->use();

    // Set uniforms
    pShader->setMat4("model", model);
    pShader->setVec3("center", center);
    pShader->setFloat("radius", radius);
    pShader->setFloat("height", height);
    pShader->setFloat("majorRadius", majorRadius); // Only relevant for torus
    pShader->setVec3("color", color);
    pShader->setInt("surfaceType", static_cast<int>(surfaceType));

    // Bind VAO and draw patch
    glBindVertexArray(vao);
    glPatchParameteri(GL_PATCH_VERTICES, 1); // Single control point
    glDrawArrays(GL_PATCHES, 0, 1);
    glBindVertexArray(0);
}
