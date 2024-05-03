#include "tzpch.h"

#include "TribleZ_Core/Input.h"
#include "TribleZ_Core/Application.h"

#include "GLFW/glfw3.h"

namespace TribleZ
{
	Input* Input::Input_Instence = new Input();


	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstence().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(int Button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstence().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, Button);

		return (state == GLFW_PRESS);
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstence().GetWindow().GetNativeWindow());
		double PosX, PosY;
		glfwGetCursorPos(window, &PosX, &PosY);

		return {(float)PosX, (float)PosY};
	}

	float Input::GetMouseX()
	{
		//GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstence().GetWindow().GetNativeWindow());
		//auto [x, y] = GetMousePosition();
		//return x;
		auto v = GetMousePosition();
		return std::get<0>(v);
	}

	float Input::GetMouseY()
	{
		//GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstence().GetWindow().GetNativeWindow());
		//auto [x, y] = GetMousePosition();
		//return y;

		auto v = GetMousePosition();
		return std::get<1>(v);
	}


}