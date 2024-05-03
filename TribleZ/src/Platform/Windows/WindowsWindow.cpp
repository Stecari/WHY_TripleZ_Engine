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
			int success = glfwInit();	//glfw���ڳ�ʼ��
			TZ_CORE_ASSERT(success, "Could not initialized!");
			glfwSetErrorCallback(GLWFErrorCallback);
			GLFWInitialize = true;
		}

		m_Window = glfwCreateWindow(m_WindowData.m_Width, m_WindowData.m_Height,
									m_WindowData.m_Title.c_str(), nullptr, nullptr);

		/*----------------���������Ĵ���-----------------------------------------------*/
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		/*-----------------------------------------------------------------------------*/


		glfwSetWindowUserPointer(m_Window, &m_WindowData);	//��һ������ͨ��voidָ��ķ�ʽ�󶨵�GLFWWindow��
		SetVSnyc(true);										//�Ա���������APP�˿������ö�Ӧ��Get�����������ǳ�����(���а���һ���ص�����)






		/*----------------------------����callback�ص�����--------------------------------------------------------------*/
				/*�������õĺ���(����)�ᱻglfwPollEvents()��ȡ�����������RUN()��ѭ��*/
																		/*�����Ƶ�OnUpdata����*/
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);	//�õ�������glfwSetWindowUserPointer���õ�ָ��
				data.m_Width = width;
				data.m_Height = height;

				EventWindowResize event(width, height);
				data.EventCallback(event);  //����̫�����������ʽת��ת������
				/*�����ǵ��ð�������*/
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

		glfwPollEvents();		//��������Ϣ�����λ�ã��������ֵ
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