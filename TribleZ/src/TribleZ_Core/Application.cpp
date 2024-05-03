
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

											/*Ԥ��ռλ�������λ����Ҫһ������*/
#define BIND_EVENT_CALLBACK_FUNC(x) std::bind(x, this, std::placeholders::_1)	//ָ��һ������ָ����а�,���ж���Ϊ��ǰ������(this)������һ��Ҫ����Ĳ���
																				//����һ������ָ��
	Application* Application::APP_Instance = nullptr;		//����

	Application::Application(const std::string& name)		//Ӧ�ó�ʼ��
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*---------------------�������ʼ��-----------------------------------------------------------------*/
		TZ_CORE_ASSERT(APP_Instance == nullptr, "APP is already exist!")
		APP_Instance = this;		//�������ʼ��

		/*��д Windows::Create*/
		m_Window = Window::Create(WindowProps(name));//����Ӧ�õĴ���
		//m_Window = std::unique_ptr<Window>(Window::CreatWindowsWindow());				//����Ӧ�õĴ���
		m_Window->SetEventCallBack(BIND_EVENT_CALLBACK_FUNC(&Application::OnEvent));	//�󶨻ص�������Ӧ�ô��ڵ��ڲ�����
										/*���⿪ʼ��ʹ��m_Window��m_WindowData�е�EventCallback����ʹ��OnEvent*/
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

	void Application::PushLayer(Layer* layer)	//����ջ��ʵ���ϸ�����Attach�İ�װ�������һ����ջ
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_LayersStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlayer(Layer* overlayer)	//����ջ��ʵ���ϸ�����Attach�İ�װ�������һ����ջ
	{
		TZ_PROFILE_FUNCTION_SIG();

		//m_LayersStack.PopOverLayer(overlayer);	//żȻ���ֵ����⣬���POP����������
		m_LayersStack.PushOverLayer(overlayer);
		overlayer->OnAttach();
	}


	/*Ӧ�����¼��ܻص�
	*����Ӧ�õĸ�������¼�����
	*ÿ�����¼�������󲿷־��Ǵ���һЩ�ص�����
	* 
	* ������󶨵���WindowsWindow��WindowsData���EventCallback����
	*/
	void Application::OnEvent(Event& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		//��������ʹ�õ��Ⱥ���EventDispacher�������Ⱥ��ʵĺ�������������
		EventDispatcher event_dispacher(event);
		event_dispacher.DisPatcher<EventWindowClose>(BIND_EVENT_CALLBACK_FUNC(&Application::OnWindowClose));
		event_dispacher.DisPatcher<EventWindowResize>(BIND_EVENT_CALLBACK_FUNC(&Application::OnWindowResize));

		//TZ_CORE_INFO("{0}", event);

		for (auto it = m_LayersStack.end(); it != m_LayersStack.begin();)	//�������в�
		{
			if (event.m_Handle)	break;		//����Ƿ���������Ҫ�Ļص������Ƿ��Ѿ����������
			(*--it)->OnEvent(event);		//�鿴һ�����Լ��Ļص��������鿴�Ƿ���������Ҫ�Ļص�����
		}
	}

	void Application::Close()
	{
		Running_Flag = false;
	}

	/*���к���
	*Ŀǰ������������Ҫ�����±�����ջ���������ø��������ѯ���������Ҷ�Ӧ���ܱ�Ӳ���Ƿ���ƥ��ĸ���
	*glfwPollEvents()����Windowswindow,��ѯ����ȵ��ܱߣ����ҵ��ö�Ӧ�ڲ��ĵİ�װ�ص��������ص����������������Լ�����ȥ�ģ�
	*glfwSwapBuffers(m_Window)����m_Window����ɫ���壬����ɫ�������м�����Ļ�������ص���ɫȫ�³���
	*
	*/
	void Application::RUN()
	{

		while (Running_Flag)		
		{
			/*-----------��ȡÿ֡ʱ��------------------------*/
			float time = glfwGetTime();
			TimeStep time_step = time - LastFrameTime;
			LastFrameTime = time;
			/*-----------------------------------------------*/

			if (!Minimized_flag)	//�鿴�����Ƿ���С��
			{
				{
					TZ_PROFILE_SCOPE("LAYER-OnUpdata");
					for (Layer* layer : m_LayersStack)		//�������в�
					{
						layer->OnUpdata(time_step);		//ˢ�£�����ȥ������Ⱦ�������ݣ�ʵʱ��ȡ��һЩ����
					}
				}
								/*OnUpdata������ѯ��Ҳ������Ѱ�ܱ��豸�Ƿ��и���*/
				m_ImGuiLayer->BEGIN();	//
				for (Layer* layer : m_LayersStack)
				{
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->END();
			}

			m_Window->OnUpdata();	//���£���ȡ��Ӧ�ô�����Ϣ�����Ƕ�ȡ���λ�ã�����֮�������������
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