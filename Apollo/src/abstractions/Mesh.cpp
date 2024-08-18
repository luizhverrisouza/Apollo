#include "Mesh.h"
#include "OBJLoader/OBJLoader.h"
#include <fstream>
#include <iostream>


Mesh::Mesh() = default;
Mesh::~Mesh() = default;

void Mesh::LoadOBJ(std::string filepath, bool useTexture, bool useMaterial, bool useNormal)
{
	OBJLoader::AcquireVerticesFromOBJFile(filepath.c_str(), useTexture, useMaterial, useNormal);
	m_Vertices = OBJLoader::GetVertices();
	m_Indices = OBJLoader::GetIndices();
	m_MeshLayout = OBJLoader::GetMeshLayout();
	if (m_Vertices.empty() || m_Indices.empty()) {
		std::cerr << "Falha ao carregar dados do OBJ." << std::endl;
	}
}

std::vector<float> Mesh::GetVertices()
{
	return m_Vertices;
}
std::vector<unsigned int> Mesh::GetIndices()
{
	return m_Indices;
}
std::vector<unsigned int> Mesh::GetMeshLayout()
{
	return m_MeshLayout;
}