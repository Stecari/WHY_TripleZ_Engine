#pragma once
#include "Core.h"
#include "Window.h"

#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "TribleZ_Core/Event/Event.h"
#include "Event/ApplicationEvent.h"
//#include "Event/KeyboardEvent.h"
//#include "Event/MouseEvent.h"

#include "TribleZ_Core/Render/Shader.h"
#include "TribleZ_Core/Render/Buffer.h"
#include "TribleZ_Core/Render/VertexArray.h"

#include "TribleZ_Core/Render/Camera.h"


namespace TribleZ
{
	class TRIBLEZ_API Application
	{
	private:
		static Application* APP_Instance;

		std::unique_ptr<Window> m_Window;		//Ӧ����һ�������࣬��������һ��window��window������
		ImGuiLayer* m_ImGuiLayer;	//��������Լ���һ��ImGui���Բ㣬�������ͻ���
		
		bool Running_Flag = true;
		LayerStack m_LayersStack;


	public:
		Application(const std::string& name = "TribleZ_APP");
		virtual ~Application();

		inline static Application& GetInstence() { return *APP_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void PushLayer(Layer* layer);			//�������Ǹ���װ����
		void PushOverlayer(Layer* overlayer);	//����Ҫ��m_LayersStack��push����

		void RUN();
		void Close();

		void OnEvent(Event& event);

	private:
		bool OnWindowClose(EventWindowClose& event);
		bool OnWindowResize(EventWindowResize& event);

	private:
		float LastFrameTime = 0;

		bool Minimized_flag = false;	//������С����־

	};
	/*��������Ǵ���APP����Ҳ���Ǵ�ͻ��ˣ���ÿ���ͻ��˶��ǲ�һ���ģ��ⲻ��������Ҫ�����£���������������Ƿ��ڿͻ���(sandbox)����*/
	Application* CreatApplication();
}


