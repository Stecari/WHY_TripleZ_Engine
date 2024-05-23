project "TribleZ_Editor"
	location ("TribleZ_Editor")
	kind ("ConsoleApp")
	language ("C++")
	cppdialect "C++20"
	staticruntime "on"
	--staticruntime "off"

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
