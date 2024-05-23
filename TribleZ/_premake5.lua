project "TribleZ"
	location ("TribleZ")
	kind "Staticlib"
	language ("C++")
	cppdialect "C++20"
	staticruntime "on"	
	--staticruntime "off"		

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
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"			--glfw和glad防止冲突
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
		"%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",

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