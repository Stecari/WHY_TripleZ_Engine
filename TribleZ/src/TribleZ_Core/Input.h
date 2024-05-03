#pragma once

#include "Core.h"

namespace TribleZ
{
	class TRIBLEZ_API Input
	{
	private:
		static Input* Input_Instence;
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int Button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		//static float GetMouseRollingOffSet()
	};

}