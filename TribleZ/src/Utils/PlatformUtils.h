#pragma once
#include "string"

namespace TribleZ
{
	/*ֻ��һ�������ĺϼ���û�е�Ҳ�ǿ��ԣ����������Ǿ���ôƯ����TribleZ�ռ�����һ������*/
	class FileDialogs
	{
	public:
		/*
		* ���fileterʵ���Ͼ������ǵ�һ�α����ļ���ʱ��
		* �����ļ������Ǹ��Ի���һ�ߵ��ļ�������
		* ����ѡ������Ҫ���ļ���׺����
		* ��������������һ���ַ�������������Ҫ�������ļ��ľ���·��
		* ������;ȡ���ͻ᷵�ؿ��ַ���
		*/
		//static std::optional<std::string> OpenFile(const char* filter);	//���ܻ᷵�ؿյĶ���
		//static std::optional<std::string> SaveFile(const char* filter);
		static std::string OpenFile(const char* filter);		//���������������ƽ̨�仯��һ�㲻ͬ��������ֻ����Windows
		static std::string SaveFile(const char* filter);
	};
}