#pragma once
#include "tzpch.h"

#include "Core.h"
#include "Event/Event.h"

namespace TribleZ
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "TribleZ Window",	//const���ò���������ֵ
					uint32_t width = 1280/*1600*/,		//����ֱ�Ӹ���Ĳ���������ô......
					uint32_t height = 720/*900*/)
			:Title(title), Width(width), Height(height)
		{}
	};

	//�������洰�ڵĽӿ�
	class TRIBLEZ_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;		//void OnEvent(Event&) {...}

		virtual ~Window() {}

		virtual void OnUpdata() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//��������
		virtual void SetEventCallBack(const EventCallbackFn& callback) = 0;
		virtual void SetVSnyc(bool enabled) = 0;
		virtual bool IsVSnyc() const = 0;	//��������������ʾ��ˢ�µ�һ���ʱ��ͻ���ɻ���˺�ѣ�VSnyc����˼���ǵ���ʾ��ˢ����ɺ��ٸ���


		/*��д Windows::Create*/
		static Scope<Window> Create(const WindowProps& props = WindowProps());
		static Window* CreatWindowsWindow(const WindowProps& props = WindowProps());

	};
}