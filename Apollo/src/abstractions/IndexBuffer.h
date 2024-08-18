#pragma once

#include <tuple>


typedef unsigned int IndexBufferID;
struct IndexBufferID_Count
{
	IndexBufferID ID = 0;
	unsigned int count = 0;
};

class IndexBufferManager
{
public:
	IndexBufferManager();
	~IndexBufferManager();

	static IndexBufferID_Count CreateBuffer();
	static void DeleteBuffer(IndexBufferID& iboID);

	static void AddData(IndexBufferID_Count& iboID_Count, const unsigned int* data, unsigned int count, unsigned int type);
	static void Bind(IndexBufferID& iboID);
	static void Unbind();
	
};