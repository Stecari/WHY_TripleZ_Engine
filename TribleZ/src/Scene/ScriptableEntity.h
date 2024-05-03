#pragma once
#include "Scene/Entity.h"
namespace TribleZ
{
	class ScriptableEntity		//Ψһʹ�����ĳ���������ӵ�����ʱ�����һ��Bind
	{
	private:
		friend class Scene;
		Entity m_Entity;

	protected:
		//�ű�������������麯��
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdata(TimeStep timestep) {}

	public:
		~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	};
}