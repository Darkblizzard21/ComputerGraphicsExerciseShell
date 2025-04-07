#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    // constructor reads and builds the shader
    Shader(const std::string_view vertexShaderSource, const std::string_view fragmentShaderSource);

    // deletes the shaders
    ~Shader();

    // use/activate the shader
    void use();
    // utility uniform functions we can expand them later as we need them
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4x4(const std::string& name, const glm::mat4x4& value) const;

    // getFunctions
    unsigned int GetProgramId() const;
    unsigned int GetVertexShaderId() const;
    unsigned int GetFragmentShaderId() const;

private:
    // the program ID
    unsigned int ID         = -1;
    unsigned int vertexID   = -1;
    unsigned int fragmentID = -1;
};
