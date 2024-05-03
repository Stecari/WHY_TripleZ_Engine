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

		virtual void OnAttach() {}				//����,��һ���㱻�Ƶ���ջ�ϵ�ʱ�����ǻ�̶���������������൱��Init
		virtual void OnDetach() {}				//����
		virtual void OnUpdata(TimeStep time_step) {}				//ˢ��
		virtual void OnEvent(Event& event) {}	//���¼�������Ӧ���ʱ����д���(�ص�)

		virtual void OnImGuiRender() {}			//��������Ҫ����ImGui������ԵĶ���

		inline const std::string& GetDebugName() const { return m_DebugName; }

	};
}