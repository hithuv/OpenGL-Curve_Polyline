#include "shape/Dodecahedron.h"
#include "util/Shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

// Constructor
Dodecahedron::Dodecahedron(Shader* pShader, const std::string& vertexFile, const glm::mat4& model)
    : Mesh(pShader, model)  // Call the protected Mesh constructor
{
    // Initialize vertex data by reading from the vertexFile
    if (std::ifstream fin {vertexFile})
    {
        glm::vec3 v1, v2, v3;

        while (fin >> v1.x >> v1.y >> v1.z
                  >> v2.x >> v2.y >> v2.z
                  >> v3.x >> v3.y >> v3.z)
        {
            // Compute the normal for the current triangle (normal should be recalculated for each subdivision)
            glm::vec3 fn = glm::normalize(glm::cross(v2 - v1, v3 - v2));

            // Assign a color (you can choose to vary colors or keep them constant)
            glm::vec3 color = glm::vec3(0.31f, 0.5f, 1.0f);  // Example color

            // Add the three vertices of the triangle
            vertices.emplace_back(v1, fn, color);
            vertices.emplace_back(v2, fn, color);
            vertices.emplace_back(v3, fn, color);
        }

        fin.close();
    }
    else
    {
        throw std::runtime_error("Failed to open " + vertexFile);
    }

    // Bind and populate the OpenGL buffer with the initial vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(vertices.size() * sizeof(Vertex)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0U);
}

// Render method (inherits from Mesh)
void Dodecahedron::render(float timeElapsedSinceLastFrame)
{
    Mesh::render(timeElapsedSinceLastFrame);
}

// Generate a unique key for an edge
std::string Dodecahedron::getEdgeKey(const glm::vec3& v1, const glm::vec3& v2)
{
    // Sort the vertices to ensure the key is unique regardless of the order
    if (v1.x < v2.x || (v1.x == v2.x && v1.y < v2.y) ||
        (v1.x == v2.x && v1.y == v2.y && v1.z < v2.z))
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << v1.x << "," << v1.y << "," << v1.z << "-"
            << v2.x << "," << v2.y << "," << v2.z;
        return oss.str();
    }
    else
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << v2.x << "," << v2.y << "," << v2.z << "-"
            << v1.x << "," << v1.y << "," << v1.z;
        return oss.str();
    }
}

// Find or create the midpoint vertex for an edge
glm::vec3 Dodecahedron::getMidpoint(const glm::vec3& v1, const glm::vec3& v2)
{
    std::string key = getEdgeKey(v1, v2);
    auto it = midpointCache.find(key);
    if (it != midpointCache.end())
    {
        return it->second;
    }

    // Compute the midpoint and normalize it to lie on the unit sphere
    glm::vec3 midpoint = glm::normalize((v1 + v2) / 2.0f);

    // Cache the computed midpoint
    midpointCache[key] = midpoint;
    return midpoint;
}

// Subdivide a triangle into 4 smaller triangles
void Dodecahedron::subdivideTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, 
                                     std::vector<Vertex>& newVertices)
{
    glm::vec3 a = getMidpoint(v1, v2);
    glm::vec3 b = getMidpoint(v2, v3);
    glm::vec3 c = getMidpoint(v3, v1);

    glm::vec3 normal1 = glm::normalize(glm::cross(a - v1, c - v1));
    glm::vec3 normal2 = glm::normalize(glm::cross(b - v2, a - v2));
    glm::vec3 normal3 = glm::normalize(glm::cross(c - v3, b - v3));
    glm::vec3 normal4 = glm::normalize(glm::cross(a - c, b - c));

    glm::vec3 color = glm::vec3(0.31f, 0.5f, 1.0f);  // Example color

    // Create the new subdivided triangles
    newVertices.emplace_back(Vertex(v1, normal1, color));
    newVertices.emplace_back(Vertex(a, normal1, color));
    newVertices.emplace_back(Vertex(c, normal1, color));

    newVertices.emplace_back(Vertex(v2, normal2, color));
    newVertices.emplace_back(Vertex(b, normal2, color));
    newVertices.emplace_back(Vertex(a, normal2, color));

    newVertices.emplace_back(Vertex(v3, normal3, color));
    newVertices.emplace_back(Vertex(c, normal3, color));
    newVertices.emplace_back(Vertex(b, normal3, color));

    newVertices.emplace_back(Vertex(a, normal4, color));
    newVertices.emplace_back(Vertex(b, normal4, color));
    newVertices.emplace_back(Vertex(c, normal4, color));
}

// Subdivide the current dodecahedron mesh
void Dodecahedron::subdivide()
{
    std::vector<Vertex> newVertices;
    midpointCache.clear();  // Clear cache for the new subdivision step

    // Check if the current faces are pentagons or triangles
    if (vertices.size() % 3 == 0)
    {
        // If it's the first subdivision, all faces are pentagons (5 vertices each)
        for (size_t i = 0; i < vertices.size(); i += 5)  // Iterate over pentagonal faces
        {
            // Extract the five vertices of the current pentagon
            const glm::vec3& v1 = vertices[i].position;
            const glm::vec3& v2 = vertices[i + 1].position;
            const glm::vec3& v3 = vertices[i + 2].position;
            const glm::vec3& v4 = vertices[i + 3].position;
            const glm::vec3& v5 = vertices[i + 4].position;

            // Find the center of the pentagon
            glm::vec3 center = glm::normalize((v1 + v2 + v3 + v4 + v5) / 5.0f);

            // Subdivide the pentagon into 5 triangles
            newVertices.emplace_back(Vertex(center, glm::normalize(glm::cross(v2 - v1, v3 - v1)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v1, glm::normalize(glm::cross(v2 - v1, v3 - v1)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v2, glm::normalize(glm::cross(v2 - v1, v3 - v1)), glm::vec3(0.31f, 0.5f, 1.0f)));

            newVertices.emplace_back(Vertex(center, glm::normalize(glm::cross(v3 - v2, v4 - v2)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v2, glm::normalize(glm::cross(v3 - v2, v4 - v2)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v3, glm::normalize(glm::cross(v3 - v2, v4 - v2)), glm::vec3(0.31f, 0.5f, 1.0f)));

            newVertices.emplace_back(Vertex(center, glm::normalize(glm::cross(v4 - v3, v5 - v3)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v3, glm::normalize(glm::cross(v4 - v3, v5 - v3)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v4, glm::normalize(glm::cross(v4 - v3, v5 - v3)), glm::vec3(0.31f, 0.5f, 1.0f)));

            newVertices.emplace_back(Vertex(center, glm::normalize(glm::cross(v5 - v4, v1 - v4)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v4, glm::normalize(glm::cross(v5 - v4, v1 - v4)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v5, glm::normalize(glm::cross(v5 - v4, v1 - v4)), glm::vec3(0.31f, 0.5f, 1.0f)));

            newVertices.emplace_back(Vertex(center, glm::normalize(glm::cross(v1 - v5, v2 - v5)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v5, glm::normalize(glm::cross(v1 - v5, v2 - v5)), glm::vec3(0.31f, 0.5f, 1.0f)));
            newVertices.emplace_back(Vertex(v1, glm::normalize(glm::cross(v1 - v5, v2 - v5)), glm::vec3(0.31f, 0.5f, 1.0f)));
        }
    }
    else
    {
        // Further subdivision for pure triangles
        for (size_t i = 0; i < vertices.size(); i += 3)
        {
            const glm::vec3& v1 = vertices[i].position;
            const glm::vec3& v2 = vertices[i + 1].position;
            const glm::vec3& v3 = vertices[i + 2].position;

            subdivideTriangle(v1, v2, v3, newVertices);
        }
    }

    // Replace the old vertex list with the new subdivided vertices
    vertices = newVertices;

    // Update the OpenGL buffer with the new vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizei>(vertices.size() * sizeof(Vertex)),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
