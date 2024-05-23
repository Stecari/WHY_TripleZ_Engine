#pragma once
#include "string"

namespace TribleZ
{
	/*只是一个函数的合集，没有倒也是课以，不过让他们就这么漂浮在TribleZ空间里有一点难受*/
	class FileDialogs
	{
	public:
		/*
		* 这个fileter实际上就是像是第一次保存文件的时候
		* 输入文件名的那个对话框一边的文件过滤器
		* 可以选择我们要的文件后缀类型
		* 这两个函数返回一个字符串，就是我们要操作的文件的绝对路径
		* 假如中途取消就会返回空字符串
		*/
		//static std::optional<std::string> OpenFile(const char* filter);	//可能会返回空的东西
		//static std::optional<std::string> SaveFile(const char* filter);
		static std::string OpenFile(const char* filter);		//这个过滤器会随着平台变化有一点不同，但现在只考虑Windows
		static std::string SaveFile(const char* filter);
	};
}