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
		"YAML_CPP_STATIC_DEFINE"		--TMD这个Define在自己的项目里也要define，Cherno我真是服了你个sb了
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		--cppdialect "C++20"
		staticruntime "off"		--这里cherno忘记讲了，由于我们要把它作为静态库链接，所以要改为on

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
