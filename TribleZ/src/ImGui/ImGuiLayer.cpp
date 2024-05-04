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

	void ImGuiLayer::OnAttach()     //说实话，这个更像是这个层实际功能的初始化
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
        
        //设置字体      // //像是往一个数组里面存入了以中国子体，需要的时候可以用imgui_io.Fonts->Fonts[索引] 来找到并调用,索引就是使用顺序
        imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-Light.ttf", 20.0f);                              //0
        imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-CondensedExtraBold.ttf", 18.0f);                 //1
        imgui_io.FontDefault = imgui_io.Fonts->AddFontFromFileTTF("asserts/font/OpenSans/OpenSans-Regular.ttf", 20.0f);     //2


		ImGui::StyleColorsDark();	//暗色调界面
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        //设置ImGui界面颜色，纯抄的
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
    *   ImGui渲染开始点
    *   开始新的帧
    */
    void ImGuiLayer::BEGIN()
    {
        TZ_PROFILE_FUNCTION_SIG();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //开始记录ImGuizmo的Frame
        ImGuizmo::BeginFrame();
    }

    /*
    *   ImGui渲染结束点
    *   渲染准备完成(结束)
    *   开始向屏幕上吐数据(渲染)
    */
    void ImGuiLayer::END()
    {
        TZ_PROFILE_FUNCTION_SIG();

        /*--------------导入成员，设置窗框架大小------------------------------*/
        ImGuiIO& imgui_io = ImGui::GetIO();
        Application& APP = Application::GetInstence();
        imgui_io.DisplaySize = ImVec2((float)APP.GetWindow().GetWidth(), (float)APP.GetWindow().GetHeight());

        GLFWwindow* window = (GLFWwindow*)APP.GetWindow().GetNativeWindow();

        /*-----------设置Δt(变化的时间)(?)-------------------------------------*/
        float time = (float)glfwGetTime();
        imgui_io.DeltaTime = (m_Time > 0.0f)? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;


        /*--------------Rendering----------------------------------------------*/
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /*----检测 视口 使能了没有---------------------------*/
        if (imgui_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

    }

    /*
    *   渲染器
    *   负责渲染的主体部分
    *   就是要在这里设置我们渲染的东西和设置
    */
    void ImGuiLayer::OnImGuiRender()
    {

        /*----Our state--------显示窗口即显示状态绑定？----------------*/
        bool show_demo_window = true;
        bool show_another_window = false;

        //ImGui::ShowDemoWindow(&show_demo_window);


    }

    /*
    *   事件响应
    *   负责接收事件的主体部分
    *   接收事件然后交付句柄
    */
    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvent)
        {
            ImGuiIO& imgui_io = ImGui::GetIO();
            //假如存在阻塞事件，将会手动使现在接受的事件的句柄全部变成已处理？
            event.m_Handle |= event.Filter_IsInCategory(EventCategoryMouse) & imgui_io.WantCaptureMouse;
            event.m_Handle |= event.Filter_IsInCategory(EventCategoryKeyboard) & imgui_io.WantCaptureKeyboard;
        }
    }

    //设置一些深色的预设
    void ImGuiLayer::SetDarkThemeStyleGuiWinodw()
    {
        auto& Colors = ImGui::GetStyle().Colors;
        Colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };        //

        //标头
        Colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };   
        Colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //按钮
        Colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        Colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


        //框
        Colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        Colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //标签
        Colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        Colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        Colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        //标题
        Colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


    }

}