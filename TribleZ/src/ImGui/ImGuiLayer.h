#pragma once

#include "TribleZ_Core/Layer.h"

#include "TribleZ_Core/Event/MouseEvent.h"
#include "TribleZ_Core/Event/KeyboardEvent.h"
#include "TribleZ_Core/Event/ApplicationEvent.h"

namespace TribleZ
{
	class TRIBLEZ_API ImGuiLayer : public Layer
	{
	private:
		float m_Time;
		bool m_BlockEvent;
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;				//链接,当一个层被推到层栈上的时候我们会固定他，就用这个，相当于Init
		virtual void OnDetach() override;				//断链
		//virtual void OnUpdata() override;				//刷新
		virtual void OnEvent(Event& event) override;		//层事件来到对应层的时候进行处理(回调)

		virtual void OnImGuiRender() override;

		void SetBlockEvent(bool block) { m_BlockEvent = block; }

		void BEGIN();
		void END();

	private:
		void SetDarkThemeStyleGuiWinodw();

	};
}