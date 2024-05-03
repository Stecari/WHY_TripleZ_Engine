#pragma once
#ifdef TZ_PLATFORM_WINDOWS

#include "TribleZ_Core/Core.h"
#include "Tool/BenchMarking.h"

extern TribleZ::Application* TribleZ::CreatApplication();

int main(int argc, char** argv)
{
	TZ_PROFILE_BEGIN_SESSION("StartUp", "TribleZProfile-StartUp.json");
	TribleZ::Log::Init();
	//TZ_CLIENT_ERROR("Core ready");
	//TZ_CORE_INFO("Client ready");
	//int a = 5;
	//TZ_CORE_WARN("Taht is {:08d}", a);
	//TribleZ::Log::GetClientLogger()->error("Some error message with arg: {}", 1);
	//TribleZ::Log::GetClientLogger()->warn("Easy padding in numbers like {:08d}", 12);
	//TribleZ::Log::GetClientLogger()->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	//TribleZ::Log::GetClientLogger()->info("Support for floats {:03.2f}", 1.23456);
	//TribleZ::Log::GetClientLogger()->info("Positional args are {1} {0}..", "too", "supported");
	//TribleZ::Log::GetClientLogger()->info("{:<30}", "left aligned");
	std::cout << "TribleZ Engine Loading Start!" << std::endl;

	auto app = TribleZ::CreatApplication();
	TZ_PROFILE_END_SESSION();

	TZ_PROFILE_BEGIN_SESSION("RunTime", "TribleZProfile-RunTime.json")
	app->RUN();
	TZ_PROFILE_END_SESSION();
	delete app;
}

#endif

