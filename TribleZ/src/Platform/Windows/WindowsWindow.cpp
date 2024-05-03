#include "tzpch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLContext.h"

#include "TribleZ_Core/Event/MouseEvent.h"
#include "TribleZ_Core/Event/KeyboardEvent.h"
#include "TribleZ_Core/Event/ApplicationEvent.h"
//#include "TribleZ_Core/Input.h"
//#include "Platform/Windows/WindowsInput.h"


namespace TribleZ
{
	static bool GLFWInitialize = false;

	static void GLWFErrorCallback(int error, const char* description)
	{
		TZ_CORE_ERROR("[{0}]: {1}", error, description);
	}

	Window* Window::CreatWindowsWindow(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shatdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_WindowData.m_Title = props.Title;
		m_WindowData.m_Width = props.Width;
		m_WindowData.m_Height = props.Height;

		TZ_CORE_INFO("CreatWindowsWindow {0}: ({1}, {2})", props.Title, props.Width, props.Height);


		if (!GLFWInitialize)
		{
			int success = glfwInit();	//glfw窗口初始化
			TZ_CORE_ASSERT(success, "Could not initialized!");
			glfwSetErrorCallback(GLWFErrorCallback);
			GLFWInitialize = true;
		}

		m_Window = glfwCreateWindow(m_WindowData.m_Width, m_WindowData.m_Height,
									m_WindowData.m_Title.c_str(), nullptr, nullptr);

		/*----------------环境上下文创建-----------------------------------------------*/
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		/*-----------------------------------------------------------------------------*/


		glfwSetWindowUserPointer(m_Window, &m_WindowData);	//把一套数据通过void指针的方式绑定到GLFWWindow上
		SetVSnyc(true);										//以便于我们在APP端可以利用对应的Get函数把数据那出来用(其中包括一个回调函数)






		/*----------------------------设置callback回调函数--------------------------------------------------------------*/
				/*这里设置的函数(条件)会被glfwPollEvents()读取，这个函数在RUN()里循环*/
																		/*现在移到OnUpdata里了*/
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);	//拿到了上面glfwSetWindowUserPointer设置的指针
				data.m_Width = width;
				data.m_Height = height;

				EventWindowResize event(width, height);
				data.EventCallback(event);  //看不太懂，大概是隐式转换转过来的
				/*这里是调用啊！！操*/
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				EventWindowClose event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						EventKeyPress event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						EventKeyReleased event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						EventKeyRepeat event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				EventKeyTypes event(codepoint);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						EventMouseButtonPressed event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						EventMouseButtonReleased event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				EventMouseScrolled event((float)xoffset, (float)yoffset);
				data.EventCallback(event);

			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				EventMouseMoved event((float)xpos, (float)ypos);
				data.EventCallback(event);

			});
		/*------------------------------------------------------------------------------------------------------------------*/

	}
	

	void WindowsWindow::Shatdown()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdata()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glfwPollEvents();		//读窗口信息，鼠标位置，点击，键值
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSnyc(bool enabled)
	{
		TZ_PROFILE_FUNCTION_SIG();

		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_WindowData.VSync = enabled;
	}

	bool WindowsWindow::IsVSnyc() const
	{
		return m_WindowData.VSync;
	}

}