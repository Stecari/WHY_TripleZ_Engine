include "./vender/premake/premake_customization/solution_items.lua"
include "Dependence.lua"

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



group "Dependencies"
	--include "vender/premake"
	include "vender/GLFW"
	include "vender/Glad"
	include "TribleZ/Gitvender/ImGui"
	include "TribleZ/Gitvender/yaml_cpp"
	include "TribleZ/Gitvender/Box2D"
	--include "D:/Vulkan/VulkanSDK1.3.280.0"
group ""

project "TribleZ"
	location ("TribleZ")
	kind "Staticlib"
	language ("C++")
	cppdialect "C++20"
	--staticruntime "on"	
	staticruntime "off"		

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
		"YAML_CPP_STATIC_DEFINE",	--���һ��ʼ��û�и㣬chernoû˵��tmdһֱ��error LINK2019
									--��˵Ϊʲô����򱨴�һֱ��ʾ��__declspec(dllimport) public: __cdecl YAML.....��
									--�������û�����������������̬�������ӣ����վ�̬����ʹ���ˣ�����
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"			--glfw��glad��ֹ��ͻ
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
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"Box2D",
		"yaml-cpp",
		"opengl32.lib",

	}

	--���ǲ�ϣ�����ǵ������ģ��������Լ�д�Ĵ���һ��ȫ����Ҫ�������ǵ�Ԥ����ͷ�ļ�
	--�������ļ��������ҳ���Щ�ļ�Ȼ������ϡ�NoPCH���ı�־����ʾ��ҪԤ����ͷ�ļ�
	filter "files:TribleZ/Gitvender/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		--staticruntime "On"  --?????????????
		systemversion "latest"

		-- nice

		defines
		{
			--"GLFW_INCLUDE_NONE",	--��һ���ļ���ͬʱinclude"GLFW.h"��"glad.h"��ʱ��Ҫ���ȶ������
			"TZ_PLATFORM_WINDOWS",
			"TZ_BUILD_DLL",
			--"TZ_ENABLE_ASSERTS",
			"_WINDLL",
			"_UNICODE",
			"UNICODE"
		}

		--postbuildcommands	--��̬����Ҫ�õģ����ù�����������ɵ�ddl�ļ�������sandbox��ȷ��λ��
		--{
		--	("{COPY} %{cfg.buildtarget.relpath} bin/" ..outputdir.. "/Sand_Box_Game")
		--}

	filter "configurations:Debug"
		defines "TZ_DEBUG"
		--buildoptions "/MTd"
		--buildoptions "/MLd"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.ShaderC}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "TZ_RELEASE"
		--buildoptions "/MT"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}


	filter "configurations:Dist"
		defines "TZ_DIST"
		--buildoptions "/MTd"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.ShaderC}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}



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
	--staticruntime "on"
	staticruntime "off"

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
		--buildoptions "/MTd"
		--buildoptions "/MLd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TZ_RELEASE"
		--buildoptions "/MT"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TZ_DIST"
		runtime "Release"
		--buildoptions "/MT"
		optimize "on"
