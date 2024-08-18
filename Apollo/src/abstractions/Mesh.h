#pragma once

#include "Shader.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <array>
#include <tuple>

class Mesh
{
private:
	std::vector<unsigned int> m_Indices;
	std::vector<float> m_Vertices;
	std::vector<unsigned int> m_MeshLayout;
public:
	Mesh();
	~Mesh();
	void LoadOBJ(std::string filepath, bool useTexture, bool useMaterial, bool useNormal);
	std::vector<float> GetVertices();
	std::vector<unsigned int> GetIndices();
	std::vector<unsigned int> GetMeshLayout();
private:
	
};