#pragma once
#include "memory"
#define _Win32
#define _Win64

#ifdef _Win32
	/*Windows x32/x86*/
	#ifdef _Win64
		/*Windows x64*/
		#define TZ_PLATFORM_WINDOWS
	#endif // _Win64
#elif defined(__APPLE__) || defined(__MACH__)
	/*苹果平台上的一个头文件*/
	#include <TargetConditionals.h>
	/*TARGET_OS_MAC 存在在所有的平台中，所以我们必须按照这个顺序挨个检查
	 *才能确定我们真的是在MAC上运行的而不是别的*/
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define TZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define TZ_PLATFORM_MAC
		#error "MacOS is not supported!"
	#else 
		#error "Unkonw Apple platform!"
	#endif 
	/*先检查__ANDROID__再检查__Linux__，因为Android是基于Linux的内核
	 *所以安卓里面已经包含了__Linux__*/
#elif defined(__ANDROID__)
	#define TZ_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__Linux__)
	#define TZ_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unkown platform"
#endif 

#ifdef TZ_DEBUG
	#if defined(TZ_PLATFORM_WINDOWS)
		#define TZ_DEBUGBREACK  __debugbreak()
	#elif defined(TZ_PLATFORM_LINUX)
		/*Linux里面的头文件*/
		#include <signal.h>
		#define TZ_DEBUGBREACK raise(SIGTRAP)
	#else
		#error "Platform hasn't support yet!"
	#endif
#else
	#define TZ_DEBUGBREACK 
#endif 


#ifdef TZ_PLATFORM_WINDOWS	//定义平台，只为Windows设计

	#if TZ_DYNAMIC_LINK
		#ifdef TZ_BUILD_DLL			//假如这个文件是DLL文件，就定义这个
			#define TRIBLEZ_API __declspec(dllexport)
		#else						//不然定义这个
			#define TRIBLEZ_API __declspec(dllimport)
		#endif TZ_BUILD_DLL
	#else
		#define TRIBLEZ_API
	#endif TZ_DYNAMIC_LINK
#else						//平台错误或者未定义会报错
	#error TribleZ only build for windows!
#endif TZ_PLATFORM_WINDOWS


#define TZ_ENABLE_ASSERTS

//添加一种不需要报错消息，单纯在这里打断程序的断言（就是断点）
#ifdef TZ_ENABLE_ASSERTS
	#define TZ_CORE_ASSERT(x,...)  { if(!(x)){TZ_CORE_ERROR("Assertion Error: {0}" __VA_ARGS__);  __debugbreak(); }}
	#define TZ_CLIENT_ASSERT(x,...)  { if(!(x)){TZ_CLIENT_ERROR("Assertion Error: {0}" __VA_ARGS__);  __debugbreak(); }}
#else
	#define TZ_CORE_ASSERT(x,...)
	#define TZ_CLIENT_ASSERT(x,...)
#endif // TZ_ENABLE_ASSERTS


/*位移操作，右往左数第x位变成1，其他0*/
#define BIT(x)	(1 << (x))

#define TZ_CORE_BIND_EVENT_Fn(fn) std::bind(fn, this, std::placeholders::_1)
//视频的接口是长这样的，所以下面那个因该和这个接口配对，所以直接用会有问题
// 都是后要把所有代码里的BIND改一下，要么研究一下换下面那个接口
//#define TZ_CORE_BIND_EVENT_Fn(fn) std::bind(&fn, this, std::placeholders::_1)

//这个不知道是啥，视屏里写的，到时候再修吧		这个竟然是Lambda  之前再CSDN上又看到博主的cherno学习笔记里有写到时候取查一下
//#define TZ_CORE_BIND_EVENT_Fn(fn) [this](auto&&... args) -> decltype(auto) {return this->fn(std::forward<decltype(args)>(args)...);}


namespace TribleZ
{
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreatRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreatScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}