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
	/*�ṹ���ģ�庯��*/	//���ģ���Ǳ�����ӵģ���Ϊ����Ҫ����һ������UUID��Entity�Ĺ�ϣӳ�䣬����unordered_mapɶ��
	template<>				//���粻�ֶ�д���ģ�壬���ԾͲ�֪������ô����TribleZ::UUID��������ӳ�������
	struct hash<TribleZ::UUID>	//hash����ṹ����std���Ǹ����ϣӳ��ģ������е�hash map �йأ�����unordered_map��unordered_set
	{
		//����hash�ṹ���е�()������
		size_t operator() (const TribleZ::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}