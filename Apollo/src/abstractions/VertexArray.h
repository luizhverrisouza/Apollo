#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

typedef unsigned int VertexArrayID;

class VertexArrayManager
{
public:
	VertexArrayManager();
	~VertexArrayManager();

	static VertexArrayID CreateBuffer();
	static void DeleteBuffer(VertexArrayID& vaoID);

	static void AddVertexBuffer(VertexArrayID& vaoID, VertexBufferID& vboID, const VertexBufferLayout& layout);
	static void Bind(VertexArrayID& vaoID);
	static void Unbind();

	
};