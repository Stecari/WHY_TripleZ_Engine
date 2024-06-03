#include <tzpch.h>
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace TribleZ
{
	static std::random_device m_Device;
	static std::mt19937_64 m_Engine(m_Device());		//()  ��һ�������������� - ��һ�������������Ϻ���԰�һ�����ֱ�ɸ���, �Ҳ������ƹ��������֮���
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