#include <tzpch.h>
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace TribleZ
{
	static std::random_device m_Device;
	static std::mt19937_64 m_Engine(m_Device());		//()  算一个操作符，就像 - 是一个操作符，加上后可以把一个数字变成负的, 我猜是类似构造操作符之类的
	static std::uniform_int_distribution<uint64_t> m_UniformDistribution;

	UUID::UUID()
		:m_UUID(m_UniformDistribution(m_Engine))
	{

	}

	UUID::UUID(uint64_t uuid)
		:m_UUID(uuid)
	{

	}
}