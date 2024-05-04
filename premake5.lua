
workspace "TribleZ_Engine" 
	architecture "x64"
	startproject "TribleZ_Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.system}-%{cfg.buildcfg}-%{cfg.architecture}"
--????GLFW????????????(?????????)
IncludeDir = {}
IncludeDir["GLFW"] = "vender/GLFW/include"
IncludeDir["Glad"] = "vender/Glad/include"
IncludeDir["ImGui"] = "TribleZ/Gitvender/ImGui"
IncludeDir["glm"] = "TribleZ/Gitvender/glm"
IncludeDir["entt"] = "TribleZ/Gitvender/entt"
IncludeDir["stb_image"] = "TribleZ/Gitvender/stb_image"
IncludeDir["yaml_cpp"] = "TribleZ/Gitvender/yaml_cpp/include"
IncludeDir["ImGuizmo"] = "TribleZ/Gitvender/ImGuizmo"

group "Dependencies"
	--include "vender/premake"
	include "vender/GLFW"
	include "vender/Glad"
	include "TribleZ/Gitvender/ImGui"
	include "TribleZ/Gitvender/yaml_cpp"
group ""

project "TribleZ"
	location ("TribleZ")	--???λ???????premake?????·??
	kind ("Staticlib")
	language ("C++")
	cppdialect "C++20"
	staticruntime "on"		--???????????dll

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin_int/"..outputdir.."/%{prj.name}")

	pchheader "tzpch.h"
	pchsource "TribleZ/src/tzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Gitvender/glm/glm/**.hpp",
		"%{prj.name}/Gitvender/glm/glm/**.inl",
		--"%{prj.name}/Gitvender/glm/glm/**.cppm"
		"%{prj.name}/Gitvender/stb_image/stb_image.cpp",
		"%{prj.name}/Gitvender/stb_image/stb_image.h",
		
		"%{prj.name}/Gitvender/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/Gitvender/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE",	--这个一开始我没有搞，cherno没说，tmd一直报error LINK2019
									--我说为什么输出框报错一直显示“__declspec(dllimport) public: __cdecl YAML.....”
									--结果就是没定义这个将它当作动态库来链接，按照静态库来使用了，吐了
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/Gitvender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	--我们不希望我们导入的子模块和我们自己写的代码一样全都需要包括我们的预编译头文件
	--于是用文件过滤器找出这些文件然后将其加上“NoPCH”的标志来表示不要预编译头文件
	filter "files:TribleZ/Gitvender/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		--staticruntime "On"  --?????????????
		systemversion "latest"

		-- nice

		defines
		{
			--"GLFW_INCLUDE_NONE",	--在一个文件中同时include"GLFW.h"和"glad.h"的时候要事先定义这个
			"TZ_PLATFORM_WINDOWS",
			"TZ_BUILD_DLL",
			--"TZ_ENABLE_ASSERTS",
			"_WINDLL",
			"_UNICODE",
			"UNICODE"
		}

		--postbuildcommands	--动态连接要用的，后置构建命令，将生成的ddl文件拷贝到sandbox正确的位置
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} bin/" ..outputdir.. "/Sand_Box_Game")
		--}

	filter "configurations:Debug"
		defines "TZ_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TZ_DIST"
		--buildoptions "/MDd"
		runtime "Release"
		optimize "on"



project "Sand_Box_Game"
	location ("Sand_Box_Game")
	kind ("ConsoleApp")
	language ("C++")
	cppdialect "C++20"
	staticruntime "on"		--???????????dll

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin_int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"

	}

	includedirs
	{
		"TribleZ/Gitvender/spdlog/include",
		"TribleZ/src",
		"%{IncludeDir.glm}",
		"TribleZ/Gitvender",
		"%{IncludeDir.entt}"
	}

	links
	{
		"TribleZ"
	}

	filter "system:windows"
		--staticruntime "On"
		systemversion "latest"

		defines
		{
			"TZ_PLATFORM_WINDOWS",
			"_UNICODE",
			"UNICODE"
		}

	

	filter "configurations:Debug"
		defines "TZ_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TZ_DIST"
		runtime "Release"
		--buildoptions "/MD"
		optimize "on"


project "TribleZ_Editor"
	location ("TribleZ_Editor")
	kind ("ConsoleApp")
	language ("C++")
	cppdialect "C++20"
	staticruntime "on"		--???????????dll

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin_int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"

	}

	includedirs
	{
		"TribleZ/Gitvender/spdlog/include",
		"TribleZ/src",
		"%{IncludeDir.glm}",
		"TribleZ/Gitvender",
		"%{IncludeDir.entt}",
		--"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"TribleZ"
	}

	filter "system:windows"
		--staticruntime "On"
		systemversion "latest"

		defines
		{
			"TZ_PLATFORM_WINDOWS",
			"_UNICODE",
			"UNICODE"
		}

	

	filter "configurations:Debug"
		defines "TZ_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TZ_DIST"
		runtime "Release"
		--buildoptions "/MD"
		optimize "on"