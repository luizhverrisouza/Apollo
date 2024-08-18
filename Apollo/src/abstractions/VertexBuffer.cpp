#include "VertexBuffer.h"
#include "Renderer.h"

VertexBufferManager::VertexBufferManager() = default;
VertexBufferManager::~VertexBufferManager() = default;

VertexBufferID VertexBufferManager::CreateBuffer()
{
	VertexBufferID vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	return vboID;
}

void VertexBufferManager::DeleteBuffer(VertexBufferID& vboID)
{
	glDeleteBuffers(1, &vboID);
}

void VertexBufferManager::AddData(VertexBufferID& vboID, const void* data, unsigned int size, unsigned int type)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

void VertexBufferManager::Bind(VertexBufferID& vboID)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void VertexBufferManager::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}