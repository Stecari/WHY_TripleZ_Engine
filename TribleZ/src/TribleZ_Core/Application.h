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
	struct ApplicationCommandLineArgs		//命令行参数
	{
		int Count = 0;
		char** Args = nullptr;

		char* operator[](int index)
		{
			TZ_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class TRIBLEZ_API Application
	{
	private:
		static Application* APP_Instance;

		std::unique_ptr<Window> m_Window;		//应用有一个窗口类，类里面有一个window，window的数据
		ImGuiLayer* m_ImGuiLayer;	//引擎可以自己有一个ImGui调试层，不依赖客户端
		
		bool Running_Flag = true;
		LayerStack m_LayersStack;

		ApplicationCommandLineArgs m_CommandLineArgs;

	public:
		Application(const std::string& name = "TribleZ_APP", ApplicationCommandLineArgs Args = ApplicationCommandLineArgs());
		virtual ~Application();

		inline static Application& GetInstence() { return *APP_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		void PushLayer(Layer* layer);			//这俩就是个包装函数
		void PushOverlayer(Layer* overlayer);	//还是要靠m_LayersStack的push函数

		void RUN();
		void Close();

		void OnEvent(Event& event);

	private:
		bool OnWindowClose(EventWindowClose& event);
		bool OnWindowResize(EventWindowResize& event);

	private:
		float LastFrameTime = 0;

		bool Minimized_flag = false;	//窗口最小化标志

	};
	/*由于这个是创建APP对象，也就是搭建客户端，而每个客户端都是不一样的，这不是引擎里要做的事，所以这个函数我们放在客户端(sandbox)定义*/
	Application* CreatApplication(ApplicationCommandLineArgs Args);
}


