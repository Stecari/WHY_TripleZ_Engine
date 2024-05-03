#include "tzpch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Scene/SceneSerializer.h"

namespace TribleZ
{
	Scope<Window> Window::Create(const WindowProps& props)
	{

#ifdef TZ_PLATFORM_WINDOWS
		return CreatScope<WindowsWindow>(props);
#else
		TZ_CORE_ASSERT(0, "Create Window Error!");
		return 0;
#endif // TZ_PLATFORM_WINDOWS
	}
}