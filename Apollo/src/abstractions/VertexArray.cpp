#include "VertexArray.h"

VertexArrayManager::VertexArrayManager() = default;
VertexArrayManager::~VertexArrayManager() = default;

VertexArrayID VertexArrayManager::CreateBuffer()
{
	VertexArrayID vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	return vaoID;
}

void VertexArrayManager::DeleteBuffer(VertexArrayID& vaoID)
{
	glDeleteVertexArrays(1, &vaoID);
}

void VertexArrayManager::AddVertexBuffer(VertexArrayID& vaoID, VertexBufferID& vboID, const VertexBufferLayout& layout)
{
	glBindVertexArray(vaoID);
	VertexBufferManager::Bind(vboID);
	const auto& elements = layout.GetElements();
	unsigned __int64 offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		offset += element.count * VertexBufferElement::SizeOfElementType(element.type);
	}
}

void VertexArrayManager::Bind(VertexArrayID& vaoID)
{
	glBindVertexArray(vaoID);
}

void VertexArrayManager::Unbind()
{
	glBindVertexArray(0);
}