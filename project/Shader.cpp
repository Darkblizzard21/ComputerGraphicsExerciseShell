#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

#include "glm/ext.hpp"

namespace {
	bool CompileShader(int shader)
	{
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}
}  // namespace

Shader::Shader(const std::string_view vertexShaderName, const std::string_view fragmentShaderName)
{
	// Compile Vertex shader
	

	// compile Fragment shader

	// create Shader Program
	
}

Shader::~Shader()
{
	// delte program and shaders
}

void Shader::use()
{
	// bind program
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	// set uniform bool
}

void Shader::setInt(const std::string& name, int value) const
{
	// set uniform int
}

void Shader::setFloat(const std::string& name, float value) const
{
	// set uniform float
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	// set uniform vec3
}

void Shader::setMat4x4(const std::string& name, const glm::mat4x4& value) const
{
	// set uniform mat4x4
}

unsigned int Shader::GetProgramId() const
{
	return ID;
}

unsigned int Shader::GetVertexShaderId() const
{
	return vertexID;
}

unsigned int Shader::GetFragmentShaderId() const
{
	return fragmentID;
}
