#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/// Model: loads a 3D model using tinygltf and stores its meshes and textures for rendering.
class Model {
public:
    /// Constructs the model by loading it from the given file path
    Model(const std::string& path);

    /**
     * Draws the model by binding necessary textures and VAOs
     * @param shaderProgram OpenGL shader program ID
     * @param modelMatrix  Model transformation matrix
     */
    void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

private:
    /// Represents a single vertex with position, normal, and texture coordinates
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    /// Holds OpenGL texture ID, its type (e.g. "texture_diffuse"), and file path
    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    /// Encapsulates an OpenGL mesh: VAO/VBO/EBO, index count, and associated textures
    struct Mesh {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int indexCount;
        std::vector<Texture> textures;

        /// Constructs a mesh and uploads it to GPU
        Mesh(const std::vector<Vertex>& vertices,
            const std::vector<unsigned int>& indices,
            const std::vector<Texture>& textures);
    };

    std::vector<Mesh> meshes;                ///< All meshes in this model
    std::vector<Texture> texturesLoaded;     ///< Loaded textures to avoid duplication
    std::string directory;                   ///< Directory of the model file

    /// Loads the glTF/glb model and initializes meshes
    void loadModel(const std::string& path);
    /// Uploads a mesh to OpenGL (from raw vertex/index data)
    Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    /// Utility to load a texture from file using stb_image
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};
