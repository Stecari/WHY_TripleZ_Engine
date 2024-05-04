#include "tzpch.h"
#include "Utils/PlatformUtils.h"

/*
* Win32��API�����ļ��Ի����������������Ϊ�ļ�ʱ���ߵ����ļ�ʱ�ᵯ�����ġ��ļ�����������
* �Ǹ����ھͽ���FileDialogs�ļ��Ի���*/
#include <commdlg.h>	

/*����Ҫ�ڴ�file dialog ���ڵ�ʱ�����Ǹ�����֪�����ǵ�glfw���������ĸ�����
* Ҳ���ǽ����ǵ��Ѵ��ڵ�glfw���ش��ھ������Windows��file dialog ����*/
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32		//˵��Ϊ�˻�ȡ�������ڵĴ��ھ������Ϊ��һ���ļ��Ի�����ʱ���������Ĵ��ڵĽ����Ǳ����ص�
#include <GLFW/glfw3native.h>			//������VS������ļ�ʱ���ǲ�������VS�Ĵ����������������룬����ȵȲ�����

#include "TribleZ_Core/Application.h"

namespace TribleZ
{
	/*�����ļ��ľ���·��*/
	std::string FileDialogs::OpenFile(const char* filter)
	{
		//OPENFILENAME ��Ascii�汾�����������GetOpenFileNameA���ִ���A��Ҳ��
		OPENFILENAMEA Openfilename;		//commom dialog box structure
		CHAR szFile[260] = { 0 };	//if using TCHAR macros
		//��ʼ�� OPENFILENAMEA
		ZeroMemory(&Openfilename, sizeof(OPENFILENAME));
		Openfilename.lStructSize = sizeof(OPENFILENAME);
		/*��ȡ����GLFW�ı��ش��ڵĴ��ھ����������Ϊ�������ڣ�Ҳ���Ǹ����ڣ�*/
		Openfilename.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstence().GetWindow().GetNativeWindow());
		Openfilename.lpstrFile = szFile;
		Openfilename.nMaxFile = sizeof(szFile);
		Openfilename.lpstrFilter = filter;
		Openfilename.nFilterIndex = 1;
		Openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;	//���һ��OFN_NOCHANGEDIR����Ҫ
		if (GetOpenFileNameA(&Openfilename) == TRUE){				//����û�����һ���Ļ�������GetOpenFileNameA��ʱ��Ὣ��Application�Ĺ���Ŀ¼�ĳ���򿪵��ļ����Ǹ��ļ���
			return Openfilename.lpstrFile;							//VS���õ�Ĭ�Ϲ���Ŀ¼ʱ��Ŀ·�����������Ҽ���Ŀ�������еĵ����в鿴������������һ��exe�ļ������·��������exe�ļ���·��(��������һ����ݷ�ʽ)
		}												//����û�������������ô���ڴ򿪻��߱����ļ���ʱ��ı��˹���Ŀ¼���ͻ�Ӱ�쵽Application�������ಿ�ֵ��ļ��ļ��أ����ǻᱻ���ص�����assert/scene֮��ĵط������Ǹ�Ŀ¼
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