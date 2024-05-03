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


		virtual void OnAttach() override;				//����,��һ���㱻�Ƶ���ջ�ϵ�ʱ�����ǻ�̶���������������൱��Init
		virtual void OnDetach() override;				//����
		//virtual void OnUpdata() override;				//ˢ��
		virtual void OnEvent(Event& event) override;		//���¼�������Ӧ���ʱ����д���(�ص�)

		virtual void OnImGuiRender() override;

		void SetBlockEvent(bool block) { m_BlockEvent = block; }

		void BEGIN();
		void END();

	private:
		void SetDarkThemeStyleGuiWinodw();

	};
}