#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "shape/Mesh.h"

class Shader;

/// Class representing a subdividable Icosahedron
class Icosahedron : public Mesh
{
public:
    /// Constructor
    /// @param pShader Pointer to the shader program
    /// @param vertexFile Path to the vertex data file
    /// @param model Transformation matrix
    Icosahedron(Shader* pShader, const std::string& vertexFile, const glm::mat4& model);

    /// Destructor
    ~Icosahedron() noexcept override = default;

    /// Render the icosahedron
    /// @param timeElapsedSinceLastFrame Time elapsed since the last frame
    void render(float timeElapsedSinceLastFrame) override;

    /// Subdivide the current mesh to create a finer approximation
    void subdivide();

private:
    /// Cache to store midpoints and avoid duplication
    std::unordered_map<std::string, glm::vec3> midpointCache;

    /// Generate a unique key for an edge based on vertex positions
    /// @param v1 First vertex
    /// @param v2 Second vertex
    /// @return Unique string key representing the edge
    std::string getEdgeKey(const glm::vec3& v1, const glm::vec3& v2);

    /// Find or create the midpoint vertex for an edge
    /// @param v1 First vertex
    /// @param v2 Second vertex
    /// @return Midpoint vertex normalized to lie on the unit sphere
    glm::vec3 getMidpoint(const glm::vec3& v1, const glm::vec3& v2);
};

#endif  // ICOSAHEDRON_H
