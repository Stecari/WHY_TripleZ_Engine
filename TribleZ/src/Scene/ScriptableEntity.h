#pragma once
#include "Scene/Entity.h"
namespace TribleZ
{
	class ScriptableEntity		//唯一使用它的场景就是添加的他的时候进行一次Bind
	{
	private:
		friend class Scene;
		Entity m_Entity;

	protected:
		//脚本程序基本程序虚函数
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