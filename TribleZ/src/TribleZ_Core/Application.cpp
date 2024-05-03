
#include "tzpch.h"
#include "Application.h"

#include "Input.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "Render/Buffer.h"

#include "Render/Renderer.h"

#include "TimeStep.h"

namespace TribleZ
{			

											/*预留占位符，这个位置需要一个输入*/
#define BIND_EVENT_CALLBACK_FUNC(x) std::bind(x, this, std::placeholders::_1)	//指定一个函数指针进行绑定,运行对象为当前单例类(this)，仅有一个要输入的参数
																				//返回一个函数指针
	Application* Application::APP_Instance = nullptr;		//声明

	Application::Application(const std::string& name)		//应用初始化
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*---------------------单例类初始化-----------------------------------------------------------------*/
		TZ_CORE_ASSERT(APP_Instance == nullptr, "APP is already exist!")
		APP_Instance = this;		//单例类初始化

		/*重写 Windows::Create*/
		m_Window = Window::Create(WindowProps(name));//创建应用的窗口
		//m_Window = std::unique_ptr<Window>(Window::CreatWindowsWindow());				//创建应用的窗口
		m_Window->SetEventCallBack(BIND_EVENT_CALLBACK_FUNC(&Application::OnEvent));	//绑定回调函数到应用窗口的内部数据
										/*从这开始，使用m_Window中m_WindowData中的EventCallback等于使用OnEvent*/
		m_Window->SetVSnyc(m_Window->IsVSnyc());
		/*------------Renderer--------------*/
		Renderer::Init();
		/*------------ImGui-----------------*/
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlayer(m_ImGuiLayer);
		/*-------------------------------------------------------------------------------------------------*/

	}																

	Application::~Application()
	{}

	void Application::PushLayer(Layer* layer)	//推入栈，实际上更像是Attach的包装函数外加一个入栈
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_LayersStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlayer(Layer* overlayer)	//推入栈，实际上更像是Attach的包装函数外加一个入栈
	{
		TZ_PROFILE_FUNCTION_SIG();

		//m_LayersStack.PopOverLayer(overlayer);	//偶然发现的问题，这个POP函数有问题
		m_LayersStack.PushOverLayer(overlayer);
		overlayer->OnAttach();
	}


	/*应用中事件总回调
	*处理应用的各个层的事件函数
	*每个层事件函数里大部分就是处理一些回调函数
	* 
	* 这个被绑定到了WindowsWindow的WindowsData里的EventCallback上了
	*/
	void Application::OnEvent(Event& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		//会在这里使用调度函数EventDispacher，来调度合适的函数到这里运行
		EventDispatcher event_dispacher(event);
		event_dispacher.DisPatcher<EventWindowClose>(BIND_EVENT_CALLBACK_FUNC(&Application::OnWindowClose));
		event_dispacher.DisPatcher<EventWindowResize>(BIND_EVENT_CALLBACK_FUNC(&Application::OnWindowResize));

		//TZ_CORE_INFO("{0}", event);

		for (auto it = m_LayersStack.end(); it != m_LayersStack.begin();)	//遍历所有层
		{
			if (event.m_Handle)	break;		//检查是否是我们需要的回调函数是否已经被处理过了
			(*--it)->OnEvent(event);		//查看一个层自己的回调函数，查看是否是我们需要的回调函数
		}
	}

	void Application::Close()
	{
		Running_Flag = false;
	}

	/*运行函数
	*目前在运行中我们要做的事遍历层栈，挨个调用各个层的轮询函数，查找对应的周边硬件是否有匹配的更新
	*glfwPollEvents()更新Windowswindow,轮询键鼠等等周边，并且调用对应内部的的包装回调函数（回调函数主题是我们自己传过去的）
	*glfwSwapBuffers(m_Window)交换m_Window的颜色缓冲，把颜色缓冲区中间存的屏幕各个像素的颜色全吐出来
	*
	*/
	void Application::RUN()
	{

		while (Running_Flag)		
		{
			/*-----------获取每帧时间------------------------*/
			float time = glfwGetTime();
			TimeStep time_step = time - LastFrameTime;
			LastFrameTime = time;
			/*-----------------------------------------------*/

			if (!Minimized_flag)	//查看窗口是否最小化
			{
				{
					TZ_PROFILE_SCOPE("LAYER-OnUpdata");
					for (Layer* layer : m_LayersStack)		//遍历所有层
					{
						layer->OnUpdata(time_step);		//刷新？看上去像是渲染，传数据，实时获取的一些东西
					}
				}
								/*OnUpdata就是轮询，也就是搜寻周边设备是否有更新*/
				m_ImGuiLayer->BEGIN();	//
				for (Layer* layer : m_LayersStack)
				{
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->END();
			}

			m_Window->OnUpdata();	//更新（获取）应用窗口信息，像是读取鼠标位置，键盘之类的在这里做的
		}
	}

	bool Application::OnWindowClose(EventWindowClose& event)
	{
		TZ_CORE_ERROR("OnEvent CloseWincodw!");
		Running_Flag = false;
		return true;
	}

	bool Application::OnWindowResize(EventWindowResize& event)
	{
		if (event.GetWindowsWidth() == 0 || event.GetWindowsHeight() == 0)
		{
			Minimized_flag = true;
			return false;
		}
		Minimized_flag = false;
		Renderer::OnWindowResize(event.GetWindowsWidth(), event.GetWindowsHeight());

		return false;
	}


}