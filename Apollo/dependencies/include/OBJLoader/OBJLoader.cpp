#include "OBJLoader/OBJLoader.h"

#include <fstream>
#include <iostream>


std::unordered_map<std::string, OBJLoader::Color> OBJLoader::m_MaterialMap;
std::tuple<std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<glm::vec3>> OBJLoader::m_VertexDataCollection;
std::vector<OBJLoader::PTNmtl> OBJLoader::m_PTNmtlData;
std::vector<unsigned int> OBJLoader::m_Indices;
std::vector<float> OBJLoader::m_Vertices;
std::vector<unsigned int> OBJLoader::m_MeshLayout;

OBJLoader::OBJLoader() = default;
OBJLoader::~OBJLoader() = default;



std::vector<unsigned int> OBJLoader::GetMeshLayout()
{
	return m_MeshLayout;
}

void OBJLoader::AcquireVerticesFromOBJFile(const char* objFilepath, bool useTexture, bool useMaterial, bool useNormal)
{
	m_MeshLayout.clear();
	m_MeshLayout.push_back(3); //P
	if (useTexture)
	{
		m_MeshLayout.push_back(2); //T
	}
	if (useNormal)
	{
		m_MeshLayout.push_back(3); //N
	}
	if (useMaterial)
	{
		m_MeshLayout.push_back(3); //mtl
	}

	ParseVertexDataCollectionFromOBJFile(objFilepath);
	ParsePTNmtlFromOBJFile(objFilepath, useTexture, useMaterial, useNormal);
	ParseMaterialsFromOBJFile(objFilepath);

	std::vector<glm::vec3>& positions = std::get<0>(m_VertexDataCollection);
	std::vector<glm::vec2>& texCoords = std::get<1>(m_VertexDataCollection);
	std::vector<glm::vec3>& normals = std::get<2>(m_VertexDataCollection);
	for (PTNmtl& ptnmtl : m_PTNmtlData)
	{

		m_Vertices.push_back(positions[ptnmtl.posIndex - 1].x);
		m_Vertices.push_back(positions[ptnmtl.posIndex - 1].y);
		m_Vertices.push_back(positions[ptnmtl.posIndex - 1].z);
		if (useTexture) {
			m_Vertices.push_back(texCoords[ptnmtl.texCoordIndex - 1].x);
			m_Vertices.push_back(texCoords[ptnmtl.texCoordIndex - 1].y);
		}
		if (useNormal) {
			m_Vertices.push_back(normals[ptnmtl.normalIndex - 1].x);
			m_Vertices.push_back(normals[ptnmtl.normalIndex - 1].y);
			m_Vertices.push_back(normals[ptnmtl.normalIndex - 1].z);
		}
		if (useMaterial) {
			m_Vertices.push_back(m_MaterialMap[ptnmtl.mtl].r);
			m_Vertices.push_back(m_MaterialMap[ptnmtl.mtl].g);
			m_Vertices.push_back(m_MaterialMap[ptnmtl.mtl].b);
		}
	}
}


void OBJLoader::ParseVertexDataCollectionFromOBJFile(const char* objFilepath)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	std::ifstream objFile(objFilepath);
	if (!objFile) {
		std::cerr << "Falha ao abrir o arquivo: " << objFilepath << std::endl;
		return;
	}
	std::string line, mtlFilepath;
	char mtlFileName[32] = { '\0' };
	while (std::getline(objFile, line))
	{
		if (StartsWith(line, "v "))
		{
			glm::vec3 position = glm::vec3();
			(void)sscanf_s(line.c_str(), "v %f %f %f", &position.x, &position.y, &position.z);

			position.x *= 0.2f;
			position.y *= 0.2f;
			position.z *= 0.2f;


			positions.push_back(position);
		}
		else if (StartsWith(line, "vt "))
		{
			glm::vec2 texCoord = glm::vec2();
			(void)sscanf_s(line.c_str(), "vt %f %f", &texCoord.x, &texCoord.y);
			texCoords.push_back(texCoord);
		}
		else if (StartsWith(line, "vn "))
		{
			glm::vec3 normal = glm::vec3();
			(void)sscanf_s(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
	}
	objFile.close();
	m_VertexDataCollection = std::tuple<std::vector<glm::vec3>,
		std::vector<glm::vec2>,
		std::vector<glm::vec3>>(positions, texCoords, normals);
}

std::pair<std::vector<OBJLoader::PTNmtl>, std::vector<unsigned int>> OBJLoader::ExtractUniquePTNmtlAndVertexIndices(const std::vector<OBJLoader::PTNmtl>& PTNmtlGroups)
{
	std::vector<PTNmtl> uniquePTNmtlGroups;
	std::vector<unsigned int> vertexIndices;

	for (const PTNmtl& current : PTNmtlGroups) {
		auto it = std::find(uniquePTNmtlGroups.begin(), uniquePTNmtlGroups.end(), current);
		if (it == uniquePTNmtlGroups.end()) {
			uniquePTNmtlGroups.push_back(current);
			//std::cout << "P: " << current.posIndex << "  T: " << current.texCoordIndex << "  N: " << current.normalIndex << "mtl: " << current.mtl << std::endl;
			vertexIndices.push_back((unsigned int)(uniquePTNmtlGroups.size() - 1)); // Índice do novo elemento
			//std::cout << "index: " << uniquePTNmtlGroups.size() - 1 << std::endl;
		}
		else {
			vertexIndices.push_back((unsigned int)(std::distance(uniquePTNmtlGroups.begin(), it)));
			//std::cout << "P: " << current.posIndex << "  T: " << current.texCoordIndex << "  N: " << current.normalIndex << "mtl: " << current.mtl << std::endl;
			//std::cout << "(REPEATED) index: " << (std::distance(uniquePTNmtlGroups.begin(), it)) << std::endl;
		}
	}

	return std::pair<std::vector<OBJLoader::PTNmtl>, std::vector<unsigned int>>(uniquePTNmtlGroups, vertexIndices);
}


void OBJLoader::ParsePTNmtlFromOBJFile(const char* objFilepath, bool useTexture, bool useMaterial, bool useNormal)
{
	std::vector<PTNmtl> PTNmtlGroups;

	std::ifstream objFile(objFilepath);
	if (!objFile) {
		std::cerr << "Falha ao abrir o arquivo: " << objFilepath << std::endl;
		return;
	}

	std::string line;
	PTNmtl ptnmtl1, ptnmtl2, ptnmtl3;
	while (std::getline(objFile, line)) {
		if (StartsWith(line, "usemtl ")) {
			char mtlName[32] = { '\0' };
			if ((sscanf_s(line.c_str(), "usemtl %s", mtlName, (unsigned int)sizeof(mtlName)) == 1) && useMaterial) {
				ptnmtl1.mtl = std::string(mtlName);
				ptnmtl2.mtl = std::string(mtlName);
				ptnmtl3.mtl = std::string(mtlName);
			}
		}
		if (StartsWith(line, "f ")) {

			if (sscanf_s(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&ptnmtl1.posIndex, &ptnmtl1.texCoordIndex, &ptnmtl1.normalIndex,
				&ptnmtl2.posIndex, &ptnmtl2.texCoordIndex, &ptnmtl2.normalIndex,
				&ptnmtl3.posIndex, &ptnmtl3.texCoordIndex, &ptnmtl3.normalIndex) == 9) {
				if (!useTexture)
				{
					ptnmtl1.texCoordIndex = 0;
					ptnmtl2.texCoordIndex = 0;
					ptnmtl3.texCoordIndex = 0;
				}
				if (!useMaterial)
				{
					ptnmtl1.mtl = "";
					ptnmtl2.mtl = "";
					ptnmtl3.mtl = "";
				}
				if (!useNormal)
				{
					ptnmtl1.normalIndex = 0;
					ptnmtl2.normalIndex = 0;
					ptnmtl3.normalIndex = 0;
				}
				PTNmtlGroups.push_back(ptnmtl1);
				PTNmtlGroups.push_back(ptnmtl2);
				PTNmtlGroups.push_back(ptnmtl3);
			}
			else { std::cerr << "Falha ao ler PTNmtl's" << std::endl; }
		}
	}
	objFile.close();
	std::pair<std::vector<OBJLoader::PTNmtl>, std::vector<unsigned int>> pair =
		ExtractUniquePTNmtlAndVertexIndices(PTNmtlGroups);
	m_PTNmtlData = pair.first;
	m_Indices = pair.second;
}

std::string OBJLoader::FindMTLFilepath(const char* objFilepath, const char* mtlFileName)
{
	std::string objPath(objFilepath);
	size_t lastSlashPos = objPath.find_last_of("/\\");
	std::string mtlFilepath = objPath.substr(0, lastSlashPos + 1) + std::string(mtlFileName);

	return mtlFilepath;
}


void OBJLoader::ParseMaterialsFromOBJFile(const char* objFilepath)
{
	std::ifstream objFile(objFilepath);
	if (!objFile) {
		std::cerr << "Falha ao abrir o arquivo: " << objFilepath << std::endl;
		return;
	}

	std::string line;
	std::string mtlFilepath;
	while (std::getline(objFile, line)) {
		char mtlFileName[32] = { '\0' };
		if (StartsWith(line, "mtllib")) {
			(void)sscanf_s(line.c_str(), "mtllib %s", mtlFileName, (unsigned int)sizeof(mtlFileName));
			mtlFilepath = FindMTLFilepath(objFilepath, mtlFileName);
			break;
		}
	}
	objFile.close();

	std::ifstream mtlFile(mtlFilepath);
	if (!mtlFile) {
		std::cerr << "Falha ao abrir o arquivo: " << mtlFilepath << std::endl;
		return;
	}

	char materialName[32] = { '\0' };
	while (std::getline(mtlFile, line)) {
		if (StartsWith(line, "newmtl")) {
			(void)sscanf_s(line.c_str(), "newmtl %s", materialName, (unsigned int)sizeof(materialName));
		}
		if (StartsWith(line, "Kd")) {
			Color color = Color();
			(void)sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
			m_MaterialMap[std::string(materialName)] = color;
			std::cout << color.r << ", " << color.g << ", " << color.b << std::endl;
		}
	}
	mtlFile.close();
}

bool OBJLoader::StartsWith(const std::string& line, const char* text)
{
	return line.rfind(text, 0) == 0;
}

std::vector<unsigned int> OBJLoader::GetIndices()
{
	return m_Indices;
}
std::vector<float> OBJLoader::GetVertices()
{
	return m_Vertices;
}