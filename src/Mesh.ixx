module;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstddef>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

import Shader;

export module Mesh;
export namespace Graphics {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int VAO{}, VBO{}, EBO{};

        explicit Mesh(const std::string& path) {
            loadOBJ(path);
            setupMesh();
        }

        ~Mesh() {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);
        }

        void Draw(const Graphics::Shader& shader) const;

    private:
        void loadOBJ(const std::string& path);
        void setupMesh();
    };

    void Mesh::loadOBJ(const std::string& path) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
            throw std::runtime_error(warn + err);

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.Position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                if (!attrib.normals.empty() && index.normal_index >= 0)
                    vertex.Normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };

                if (!attrib.texcoords.empty() && index.texcoord_index >= 0)
                    vertex.TexCoords = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };

                vertices.push_back(vertex);
                indices.push_back(static_cast<unsigned int>(indices.size()));
            }
        }
    }

    void Mesh::setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, Normal)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void Mesh::Draw(const Graphics::Shader& shader) const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
