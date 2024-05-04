#include "tzpch.h"
#include "ImGuiLayer.h"


#define GLFW_INCLUDE_NONE
#include "imgui.h"
//#include "backends/imgui_impl_opengl3_loader.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <GLFW/glfw3.h> // Will drag system OpenGL FrameBgs
#include "glad/glad.h"

#include "TribleZ_Core/Application.h"
#include "ImGuizmo.h"


namespace TribleZ
{
   /* static ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int key);*/

	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer"), m_Time(0)
	{}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()     //˵ʵ������������������ʵ�ʹ��ܵĳ�ʼ��
	{
        TZ_PROFILE_FUNCTION_SIG();

        IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& imgui_io = ImGui::GetIO(); (void)imgui_io;
		imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		imgui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;  //
        imgui_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        imgui_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        //imgui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
        //imgui_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
        //imgui_io.ConfigViewportsNoAutoMerge = true;
        //imgui_io.ConfigViewportsNoTaskBarIcon = true;
        
        //��������      // //������һ������������������й����壬��Ҫ��ʱ�������imgui_io.Fonts->Fonts[����] ���ҵ�������,��������ʹ��˳��
        imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-Light.ttf", 20.0f);                              //0
        imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-CondensedExtraBold.ttf", 18.0f);                 //1
        imgui_io.FontDefault = imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-Regular.ttf", 20.0f);     //2


		ImGui::StyleColorsDark();	//��ɫ������
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        //����ImGui������ɫ��������
        SetDarkThemeStyleGuiWinodw();


        Application& APP = Application::GetInstence();
        GLFWwindow* window = static_cast<GLFWwindow*>(APP.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

	}

    void ImGuiLayer::OnDetach()
    {
        TZ_PROFILE_FUNCTION_SIG();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    /*
    *   ImGui��Ⱦ��ʼ��
    *   ��ʼ�µ�֡
    */
    void ImGuiLayer::BEGIN()
    {
        TZ_PROFILE_FUNCTION_SIG();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //��ʼ��¼ImGuizmo��Frame
        ImGuizmo::BeginFrame();
    }

    /*
    *   ImGui��Ⱦ������
    *   ��Ⱦ׼�����(����)
    *   ��ʼ����Ļ��������(��Ⱦ)
    */
    void ImGuiLayer::END()
    {
        TZ_PROFILE_FUNCTION_SIG();

        /*--------------�����Ա�����ô���ܴ�С------------------------------*/
        ImGuiIO& imgui_io = ImGui::GetIO();
        Application& APP = Application::GetInstence();
        imgui_io.DisplaySize = ImVec2((float)APP.GetWindow().GetWidth(), (float)APP.GetWindow().GetHeight());

        GLFWwindow* window = (GLFWwindow*)APP.GetWindow().GetNativeWindow();

        /*-----------���æ�t(�仯��ʱ��)(?)-------------------------------------*/
        float time = (float)glfwGetTime();
        imgui_io.DeltaTime = (m_Time > 0.0f)? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;


        /*--------------Rendering----------------------------------------------*/
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /*----��� �ӿ� ʹ����û��---------------------------*/
        if (imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }

    /*
    *   ��Ⱦ��
    *   ������Ⱦ�����岿��
    *   ����Ҫ����������������Ⱦ�Ķ���������
    */
    void ImGuiLayer::OnImGuiRender()
    {

        /*----Our state--------��ʾ���ڼ���ʾ״̬�󶨣�----------------*/
        bool show_demo_window = true;
        bool show_another_window = false;

        //ImGui::ShowDemoWindow(&show_demo_window);


    }

    /*
    *   �¼���Ӧ
    *   ��������¼������岿��
    *   �����¼�Ȼ�󽻸����
    */
    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvent)
        {
            ImGuiIO& imgui_io = ImGui::GetIO();
            //������������¼��������ֶ�ʹ���ڽ��ܵ��¼��ľ��ȫ������Ѵ���
            event.m_Handle |= event.Filter_IsInCategory(EventCategoryMouse) & imgui_io.WantCaptureMouse;
            event.m_Handle |= event.Filter_IsInCategory(EventCategoryKeyboard) & imgui_io.WantCaptureKeyboard;
        }
    }

    //����һЩ��ɫ��Ԥ��
    void ImGuiLayer::SetDarkThemeStyleGuiWinodw()
    {
        auto& Colors = ImGui::GetStyle().Colors;
        Colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };        //

        //��ͷ
        Colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };   
        Colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //��ť
        Colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        Colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


        //��
        Colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        Colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //��ǩ
        Colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        Colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        Colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        //����
        Colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


    }

}