#pragma once

#include <unordered_map>
#include <array>

class BufferPool
{
private:
	std::unordered_map<unsigned int, std::array<unsigned int, 3>>  m_BufferPool;
public:

private:

};