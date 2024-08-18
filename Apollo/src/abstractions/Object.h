#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Mesh.h"
#include <glm/glm.hpp>

class Object {
private:
	glm::vec3 m_Position;
	glm::mat4 m_Rotation;
	std::string m_Name;

	Shader* m_Shader;


	unsigned int m_UUID;

public:
	Object(std::string& name, Mesh& mesh, Shader* shader);
	~Object();





};