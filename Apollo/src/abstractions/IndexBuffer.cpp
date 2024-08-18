#include "IndexBuffer.h"
#include "Renderer.h"

IndexBufferManager::IndexBufferManager() = default;
IndexBufferManager::~IndexBufferManager() = default;

IndexBufferID_Count IndexBufferManager::CreateBuffer()
{
	IndexBufferID ID;
	glGenBuffers(1, &ID);
	IndexBufferID_Count iboID_Count = { ID, 0 };
	return iboID_Count;
}

void IndexBufferManager::DeleteBuffer(IndexBufferID& iboID)
{
	glDeleteBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

void IndexBufferManager::AddData(IndexBufferID_Count& iboID_Count, const unsigned int* data, unsigned int count, unsigned int type)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID_Count.ID);
	iboID_Count.count = count;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, type);
}

void IndexBufferManager::Bind(IndexBufferID& iboID)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
}

void IndexBufferManager::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}