#include "tzpch.h"
#include "Utils/PlatformUtils.h"

/*
* Win32的API用于文件对话窗，就是像是另存为文件时或者导入文件时会弹出来的“文件检索”窗口
* 那个窗口就叫做FileDialogs文件对话框*/
#include <commdlg.h>	

/*我们要在打开file dialog 窗口的时候让那个窗口知道我们的glfw窗口是他的父窗口
* 也就是将我们的已存在的glfw本地窗口句柄交给Windows的file dialog 窗口*/
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32		//说是为了获取到父窗口的窗口句柄，因为打开一个文件对话窗口时，它所属的窗口的交互是被拦截的
#include <GLFW/glfw3native.h>			//比如在VS中另存文件时，是不允许与VS的代码界面进行像是输入，点击等等操作的

#include "TribleZ_Core/Application.h"

namespace TribleZ
{
	/*返回文件的绝对路径*/
	std::string FileDialogs::OpenFile(const char* filter)
	{
		//OPENFILENAME 的Ascii版本，包括下面的GetOpenFileNameA这种带个A的也是
		OPENFILENAMEA Openfilename;		//commom dialog box structure
		CHAR szFile[260] = { 0 };	//if using TCHAR macros
		//初始化 OPENFILENAMEA
		ZeroMemory(&Openfilename, sizeof(OPENFILENAME));
		Openfilename.lStructSize = sizeof(OPENFILENAME);
		/*获取我们GLFW的本地窗口的窗口句柄交给它作为所属窗口，也就是父窗口？*/
		Openfilename.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstence().GetWindow().GetNativeWindow());
		Openfilename.lpstrFile = szFile;
		Openfilename.nMaxFile = sizeof(szFile);
		Openfilename.lpstrFilter = filter;
		Openfilename.nFilterIndex = 1;
		Openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;	//最后一个OFN_NOCHANGEDIR很重要
		if (GetOpenFileNameA(&Openfilename) == TRUE){				//假如没有最后一个的话，调用GetOpenFileNameA的时候会将你Application的工作目录改成你打开的文件的那个文件夹
			return Openfilename.lpstrFile;							//VS设置的默认工作目录时项目路径，可以在右键项目的属性中的调试中查看，假如你运行一个exe文件，这个路径就是你exe文件的路径(除非你有一个快捷方式)
		}												//假如没有设置这个，那么你在打开或者保存文件的时候改变了工作目录，就会影响到Application程序其余部分的文件的加载，他们会被加载到像是assert/scene之类的地方而不是根目录
		return std::string();

	}
	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA Openfilename;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&Openfilename, sizeof(OPENFILENAME));
		Openfilename.lStructSize = sizeof(OPENFILENAME);
		Openfilename.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstence().GetWindow().GetNativeWindow());
		Openfilename.lpstrFile = szFile;
		Openfilename.nMaxFile = sizeof(szFile);
		Openfilename.lpstrFilter = filter;
		Openfilename.nFilterIndex = 1;
		Openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&Openfilename) == TRUE) {
			return Openfilename.lpstrFile;
		}
		return std::string();
	}
}