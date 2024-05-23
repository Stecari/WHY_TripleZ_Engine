project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
	}

	includedirs
	{
		"include"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"		--TMD���Define���Լ�����Ŀ��ҲҪdefine��Cherno�����Ƿ������sb��
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		--cppdialect "C++20"
		staticruntime "off"		--����cherno���ǽ��ˣ���������Ҫ������Ϊ��̬�����ӣ�����Ҫ��Ϊon

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		--cppdialect "C++20"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
