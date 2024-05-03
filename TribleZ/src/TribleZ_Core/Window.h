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

		WindowProps(const std::string& title = "TribleZ Window",	//const引用才能引用右值
					uint32_t width = 1280/*1600*/,		//这里直接改真的不会有问题么......
					uint32_t height = 720/*900*/)
			:Title(title), Width(width), Height(height)
		{}
	};

	//基于桌面窗口的接口
	class TRIBLEZ_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;		//void OnEvent(Event&) {...}

		virtual ~Window() {}

		virtual void OnUpdata() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//窗口属性
		virtual void SetEventCallBack(const EventCallbackFn& callback) = 0;
		virtual void SetVSnyc(bool enabled) = 0;
		virtual bool IsVSnyc() const = 0;	//如果画面输出在显示器刷新到一半的时候就会造成画面撕裂，VSnyc的意思就是等显示器刷新完成后再更新


		/*重写 Windows::Create*/
		static Scope<Window> Create(const WindowProps& props = WindowProps());
		static Window* CreatWindowsWindow(const WindowProps& props = WindowProps());

	};
}