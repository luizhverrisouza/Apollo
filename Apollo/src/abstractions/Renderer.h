#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:
	void Draw(VertexArrayID& vaoID, IndexBufferID_Count& iboID_Count, Shader& shader) const;
};