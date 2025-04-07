#include "Mesh.h"

#include <glad/glad.h>

#include <tiny_gltf.h>
#include <iostream>

Mesh::Mesh(const std::vector<float>& vertexData,
	const std::vector<uint32_t>& indexData,
	const std::vector<Attribute>& vertexLayout)
{
	// Genereate Buffers and Upload Data
	
	// Set up vertex attribute object
}

Mesh::~Mesh()
{
	// Delete objects
}

void Mesh::Draw()
{
	// bind and draw vertex Attribute Object
}

std::shared_ptr<Mesh> loadFirstPrimitiveFormGltf(std::string_view modePath, bool loadNormals, bool loadTextureCoords)
{
	// setup vectors for mesh constructor

	// load gltf model and extract first primitive

	{  // 1. extract indices
	}

	// Collect vertex data per vertice for better buffer alignment
	std::vector<std::vector<float>> perVertexData = {};
	{  // 2. extract vertexdata Position
		
	}

	// 3. load normal data if wanted
	if (loadNormals) {
		
	}

	// 3. load texture coords data if wanted
	if (loadTextureCoords) {
		
	}

	// 5. flatten perVertexData to vertexData
	

	// create new mesh and return it
	return nullptr;
}
