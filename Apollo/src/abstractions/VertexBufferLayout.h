#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int SizeOfElementType(unsigned int type) {
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(float);
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE:
			return sizeof(unsigned char);
		}
		return 0;
	}

};

class VertexBufferLayout
{
private:

	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}
	
	template<typename T>
	void Push(unsigned int count)
	{

	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += VertexBufferElement::SizeOfElementType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::SizeOfElementType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::SizeOfElementType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride;  }
	
};