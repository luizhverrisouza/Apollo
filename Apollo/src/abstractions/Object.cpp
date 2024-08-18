#include "Object.h"

#include <iostream>
#include <random>



Object::Object(std::string& name, Mesh& mesh, Shader* shader)
	: m_Position(glm::vec3(0.0f)), m_Rotation(glm::mat4(1.0f)),
	  m_Shader(shader), m_Name(name)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned int> distrib(0, std::numeric_limits<unsigned int>::max());

	//Create Buffers
	std::vector<float> vertices = mesh.GetVertices();
	std::vector<unsigned int> indices = mesh.GetIndices();



	

	m_UUID = distrib(gen);
	std::cout << "Número unsigned int aleatório: " << m_UUID << std::endl;
	/*if (scene != nullptr)
	{
		SetParentScene(scene);
	}*/
}

Object::~Object() = default;


