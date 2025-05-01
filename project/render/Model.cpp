#include "Model.h"
#include "Mesh.h"
#include "stb_image.h"
#include <tiny_gltf.h>
#include <iostream>

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    glUseProgram(shaderProgram);
    GLint locModel = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, &modelMatrix[0][0]);

    unsigned int diffuseNr = 1;
    for (const auto& mesh : meshes) {
        for (unsigned int i = 0; i < mesh.textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            const std::string& name = mesh.textures[i].type;
            std::string number = std::to_string(diffuseNr++);
            glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
        }

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::loadModel(const std::string& path) {
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    // Dummy-Loader für Bilder, damit eingebettete Texturen ignoriert werden können

    // Add a custom LoadImageData callback to handle image loading  
    loader.SetImageLoader([](tinygltf::Image* image, const int imageIndex, std::string* err,  
                            std::string* warn, int req_width, int req_height,  
                            const unsigned char* bytes, int size, void* user_data) -> bool {  
       // Use stb_image to load the image data  
       int width, height, channels;  
       unsigned char* data = stbi_load_from_memory(bytes, size, &width, &height, &channels, 4);  
       if (!data) {  
           if (err) *err = "Failed to load image using stb_image.";  
           return false;  
       }  

       image->width = width;  
       image->height = height;  
       image->component = 4;  
       image->image.resize(width * height * 4);  
       std::memcpy(image->image.data(), data, width * height * 4);  
       stbi_image_free(data);  
       return true;  
    }, nullptr);  

    bool ret = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, path);


    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Err: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load glTF model: " << path << std::endl;
        return;
    }

    for (const auto& mesh : gltfModel.meshes) {
        for (const auto& primitive : mesh.primitives) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // POSITION
            const auto& posAccessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
            const auto& posView = gltfModel.bufferViews[posAccessor.bufferView];
            const auto& posBuffer = gltfModel.buffers[posView.buffer];
            const float* posData = reinterpret_cast<const float*>(
                &posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);

            for (size_t i = 0; i < posAccessor.count; ++i) {
                Vertex v{};
                v.Position = glm::vec3(
                    posData[i * 3 + 0],
                    posData[i * 3 + 1],
                    posData[i * 3 + 2]
                );
                vertices.push_back(v);
            }

            // NORMAL (optional)
            if (primitive.attributes.count("NORMAL")) {
                const auto& normAccessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                const auto& normView = gltfModel.bufferViews[normAccessor.bufferView];
                const auto& normBuffer = gltfModel.buffers[normView.buffer];
                const float* normData = reinterpret_cast<const float*>(
                    &normBuffer.data[normView.byteOffset + normAccessor.byteOffset]);

                for (size_t i = 0; i < normAccessor.count; ++i) {
                    vertices[i].Normal = glm::vec3(
                        normData[i * 3 + 0],
                        normData[i * 3 + 1],
                        normData[i * 3 + 2]
                    );
                }
            }   else {
                // Fallback
                for (auto& v : vertices) {
                    v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }
            }

            // TEXCOORD_0 (optional)
            if (primitive.attributes.count("TEXCOORD_0")) {
                const auto& uvAccessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& uvView = gltfModel.bufferViews[uvAccessor.bufferView];
                const auto& uvBuffer = gltfModel.buffers[uvView.buffer];
                const float* uvData = reinterpret_cast<const float*>(
                    &uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset]);

                for (size_t i = 0; i < uvAccessor.count; ++i) {
                    vertices[i].TexCoords = glm::vec2(
                        uvData[i * 2 + 0],
                        uvData[i * 2 + 1]
                    );
                }
            }

            // Indices
            const auto& idxAccessor = gltfModel.accessors[primitive.indices];
            const auto& idxView = gltfModel.bufferViews[idxAccessor.bufferView];
            const auto& idxBuffer = gltfModel.buffers[idxView.buffer];
            const unsigned short* idxData = reinterpret_cast<const unsigned short*>(
                &idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]);

            for (size_t i = 0; i < idxAccessor.count; ++i) {
                indices.push_back(static_cast<unsigned int>(idxData[i]));
            }

            std::vector<Texture> textures; // aktuell leer

            if (primitive.material >= 0) {
                const auto& material = gltfModel.materials[primitive.material];

                if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
                    int texIndex = material.pbrMetallicRoughness.baseColorTexture.index;
                    int imgIndex = gltfModel.textures[texIndex].source;
                    const tinygltf::Image& image = gltfModel.images[imgIndex];

                    GLuint texID;
                    glGenTextures(1, &texID);
                    glBindTexture(GL_TEXTURE_2D, texID);
                    GLenum format = (image.component == 4) ? GL_RGBA : GL_RGB;
                    glTexImage2D(GL_TEXTURE_2D, 0, format,
                        image.width, image.height, 0,
                        format, GL_UNSIGNED_BYTE,
                        image.image.data());
                    glGenerateMipmap(GL_TEXTURE_2D);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    Texture tex;
                    tex.id = texID;
                    tex.type = "texture_diffuse";
                    tex.path = image.uri;
                    textures.push_back(tex);
                }
            }


            meshes.emplace_back(vertices, indices, textures);
        }
    }
}

// Definition des Mesh-Konstruktors direkt hier
Model::Mesh::Mesh(const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    const std::vector<Texture>& textures)
    : textures(textures), indexCount(static_cast<unsigned int>(indices.size())) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
