#pragma once

#include <memory>
#include <string>
#include <vector>

class Mesh {
public:
    // specify vertex attributes
    struct Attribute {
        uint32_t type;  // GL_FLOAT GL_INT etc.
        uint32_t count;
    };

    // create mesh
    Mesh(const std::vector<float>&     vertexData,
         const std::vector<uint32_t>&  indexData,
         const std::vector<Attribute>& vertexLayout);
    // release buffers
    ~Mesh();

    // draw mesh (using current shader)
    void Draw();

private:
    uint32_t vertexBuffer          = -1;
    uint32_t indexElementBuffer    = -1;
    uint32_t vertexAttributeObject = -1;

    int32_t triangleCount = -1;
};

// loads the first primitve form the gltf model
// vertex attributes that are loaded:
// POSITION
// NORMAL     (optional)
// TEXCOORD_0 (optional)
std::shared_ptr<Mesh> loadFirstPrimitiveFormGltf(std::string_view modelPath,
                                                 bool             loadNormals       = true,
                                                 bool             loadTextureCoords = true);