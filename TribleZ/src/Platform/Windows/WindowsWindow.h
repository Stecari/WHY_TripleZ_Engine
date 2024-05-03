#pragma once

#include "TribleZ_Core/Window.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "TribleZ_Core/Render/GraphicRenderContext.h"

namespace TribleZ
{
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;
		GraphicRenderContext* m_Context;

		struct  WindowData		//放在一个结构体里面方便回调函数调用数据
		{
			std::string m_Title;
			unsigned int m_Width;
			unsigned int m_Height;

			bool VSync;

			EventCallbackFn EventCallback;

		};

		WindowData m_WindowData;
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdata() override;	//更新GLFW，事件缓存区流出事件

		inline unsigned int GetWidth() const override { return m_WindowData.m_Width; }
		inline unsigned int GetHeight() const override { return m_WindowData.m_Height; }
		inline virtual void* GetNativeWindow() const { return m_Window; }

		//窗口属性
		inline void SetEventCallBack(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; };	//设定本窗口回调函数
		void SetVSnyc(bool enabled) override;
		bool IsVSnyc() const override;
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shatdown();


	};  //class WindowsWindow

}