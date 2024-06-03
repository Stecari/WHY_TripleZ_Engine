#pragma once
#include <xhash>

namespace TribleZ
{
	class UUID
	{
	private:
		uint64_t m_UUID;

	public:
		UUID();
		UUID(uint64_t UUID);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	};
}

namespace std
{
	/*结构体的模板函数*/	//这个模板是必须添加的，因为我们要创造一个关于UUID和Entity的哈希映射，像是unordered_map啥的
	template<>				//假如不手动写这个模板，电脑就不知道该怎么对于TribleZ::UUID这个类进行映射的设置
	struct hash<TribleZ::UUID>	//hash这个结构体在std中是负责哈希映射的，跟所有的hash map 有关，像是unordered_map，unordered_set
	{
		//重载hash结构体中的()操作符
		size_t operator() (const TribleZ::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}