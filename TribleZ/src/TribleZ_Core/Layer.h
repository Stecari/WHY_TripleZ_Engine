#pragma once
#include "tzpch.h"
#include "Core.h"
#include "TribleZ_Core/Event/Event.h"

#include "TribleZ_Core/TimeStep.h"

namespace TribleZ
{
	class TRIBLEZ_API Layer
	{
	protected:
		std::string m_DebugName;

	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {}				//链接,当一个层被推到层栈上的时候我们会固定他，就用这个，相当于Init
		virtual void OnDetach() {}				//断链
		virtual void OnUpdata(TimeStep time_step) {}				//刷新
		virtual void OnEvent(Event& event) {}	//层事件来到对应层的时候进行处理(回调)

		virtual void OnImGuiRender() {}			//各个层想要放在ImGui里面调试的东西

		inline const std::string& GetDebugName() const { return m_DebugName; }

	};
}