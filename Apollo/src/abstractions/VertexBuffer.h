#pragma once



//Instance - Identifier
typedef unsigned int VertexBufferID;

//Manager
class VertexBufferManager
{
public:
	VertexBufferManager();
	~VertexBufferManager();

	static VertexBufferID CreateBuffer();
	static void DeleteBuffer(VertexBufferID& vboID);

	static void AddData(VertexBufferID& vboID, const void* data, unsigned int size, unsigned int type);
	static void Bind(VertexBufferID& vboID);
	static void Unbind();
};