
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
	location ("TribleZ")	--???��???????premake?????��??
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
		"YAML_CPP_STATIC_DEFINE",	--���һ��ʼ��û�и㣬chernoû˵��tmdһֱ��error LINK2019
									--��˵Ϊʲô����򱨴�һֱ��ʾ��__declspec(dllimport) public: __cdecl YAML.....��
									--�������û�����������������̬�������ӣ����վ�̬����ʹ���ˣ�����
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