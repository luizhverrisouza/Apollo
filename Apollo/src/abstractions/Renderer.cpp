#include "Renderer.h"

void Renderer::Draw(VertexArrayID& vaoID, IndexBufferID_Count& iboID_Count, Shader& shader) const
{
	shader.Bind();
    VertexArrayManager::Bind(vaoID);
	IndexBufferManager::Bind(iboID_Count.ID);
	glDrawElements(GL_TRIANGLES, iboID_Count.count, GL_UNSIGNED_INT, nullptr);
}
