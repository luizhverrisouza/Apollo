#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <array>
#include <tuple>

class OBJLoader
{
private:
	// f 1/1/2 3/2/1 P/T/N
	struct PTNmtl {
		unsigned int posIndex;
		unsigned int texCoordIndex;
		unsigned int normalIndex;
		std::string mtl;
		PTNmtl() : posIndex(0), texCoordIndex(0), normalIndex(0), mtl("") {}

		bool operator==(const PTNmtl& other) const {
			return posIndex == other.posIndex &&
				texCoordIndex == other.texCoordIndex &&
				normalIndex == other.normalIndex &&
				mtl == other.mtl;
		}
	};
	struct Color {
		float r;
		float g;
		float b;
		Color(float x = 0.0f) : r(x), g(x), b(x) {}
	};

	static std::unordered_map<std::string, Color> m_MaterialMap;
	static std::tuple<std::vector<glm::vec3>,
		std::vector<glm::vec2>,
		std::vector<glm::vec3>> m_VertexDataCollection; // position, texCoord, normal
	static std::vector<PTNmtl> m_PTNmtlData; // como construir os vértices

	static std::vector<unsigned int> m_Indices;
	static std::vector<float> m_Vertices;

	static std::vector<unsigned int> m_MeshLayout;

public:
	OBJLoader();
	~OBJLoader();
	static std::vector<unsigned int> GetIndices();
	static std::vector<float> GetVertices();
	static std::vector<unsigned int> GetMeshLayout();
	static void AcquireVerticesFromOBJFile(const char* objFilepath, bool useTexture, bool useMaterial, bool useNormal);
private:
	static void ParseMaterialsFromOBJFile(const char* objFilepath);
	static bool StartsWith(const std::string& line, const char* text);

	static std::string FindMTLFilepath(const char* objFilepath, const char* mtlFileName);
	static void ParsePTNmtlFromOBJFile(const char* objFilepath, bool useTexture, bool useMaterial, bool useNormal);
	static std::pair<std::vector<PTNmtl>, std::vector<unsigned int>> ExtractUniquePTNmtlAndVertexIndices(const std::vector<PTNmtl>& PTNmtlGroups);
	static void ParseVertexDataCollectionFromOBJFile(const char* objFilepath);

};
