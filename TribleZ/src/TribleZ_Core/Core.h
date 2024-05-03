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
	/*ƻ��ƽ̨�ϵ�һ��ͷ�ļ�*/
	#include <TargetConditionals.h>
	/*TARGET_OS_MAC ���������е�ƽ̨�У��������Ǳ��밴�����˳�򰤸����
	 *����ȷ�������������MAC�����еĶ����Ǳ��*/
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
	/*�ȼ��__ANDROID__�ټ��__Linux__����ΪAndroid�ǻ���Linux���ں�
	 *���԰�׿�����Ѿ�������__Linux__*/
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
		/*Linux�����ͷ�ļ�*/
		#include <signal.h>
		#define TZ_DEBUGBREACK raise(SIGTRAP)
	#else
		#error "Platform hasn't support yet!"
	#endif
#else
	#define TZ_DEBUGBREACK 
#endif 


#ifdef TZ_PLATFORM_WINDOWS	//����ƽ̨��ֻΪWindows���

	#if TZ_DYNAMIC_LINK
		#ifdef TZ_BUILD_DLL			//��������ļ���DLL�ļ����Ͷ������
			#define TRIBLEZ_API __declspec(dllexport)
		#else						//��Ȼ�������
			#define TRIBLEZ_API __declspec(dllimport)
		#endif TZ_BUILD_DLL
	#else
		#define TRIBLEZ_API
	#endif TZ_DYNAMIC_LINK
#else						//ƽ̨�������δ����ᱨ��
	#error TribleZ only build for windows!
#endif TZ_PLATFORM_WINDOWS


#define TZ_ENABLE_ASSERTS

//���һ�ֲ���Ҫ������Ϣ�������������ϳ���Ķ��ԣ����Ƕϵ㣩
#ifdef TZ_ENABLE_ASSERTS
	#define TZ_CORE_ASSERT(x,...)  { if(!(x)){TZ_CORE_ERROR("Assertion Error: {0}" __VA_ARGS__);  __debugbreak(); }}
	#define TZ_CLIENT_ASSERT(x,...)  { if(!(x)){TZ_CLIENT_ERROR("Assertion Error: {0}" __VA_ARGS__);  __debugbreak(); }}
#else
	#define TZ_CORE_ASSERT(x,...)
	#define TZ_CLIENT_ASSERT(x,...)
#endif // TZ_ENABLE_ASSERTS


/*λ�Ʋ���������������xλ���1������0*/
#define BIT(x)	(1 << (x))

#define TZ_CORE_BIND_EVENT_Fn(fn) std::bind(fn, this, std::placeholders::_1)
//��Ƶ�Ľӿ��ǳ������ģ����������Ǹ���ú�����ӿ���ԣ�����ֱ���û�������
// ���Ǻ�Ҫ�����д������BIND��һ�£�Ҫô�о�һ�»������Ǹ��ӿ�
//#define TZ_CORE_BIND_EVENT_Fn(fn) std::bind(&fn, this, std::placeholders::_1)

//�����֪����ɶ��������д�ģ���ʱ�����ް�		�����Ȼ��Lambda  ֮ǰ��CSDN���ֿ���������chernoѧϰ�ʼ�����д��ʱ��ȡ��һ��
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