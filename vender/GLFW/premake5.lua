project "GLFW"
	location ("")
	kind ("Staticlib")
	language ("C")
	staticruntime "on"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin_int/"..outputdir.."/%{prj.name}")

	files
	{
		"inclued/GLFW/**.h",
		"src/**.h",
		"src/**.c"
	}

	filter "system:windows"
	--	buildoptions ("-std=c11", "-lgid32")
		systemversion "latest"


	defines
	{
		"_GLFW_WIN32",
		"_CRT_SECURE_NO_WARNINGS"
	}

filter "configurations:Debug"
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	runtime "Release"
	optimize "on"


	--filter ("system:windows", "configrations:Release")
	--	buildoptions "/MT"