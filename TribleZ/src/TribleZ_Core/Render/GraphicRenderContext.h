#pragma once
#include "TribleZ_Core/Core.h"


namespace TribleZ
{
	class GraphicRenderContext
	{
	private:


	public:
		//GraphicRenderContext();
		//~GraphicRenderContext();

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	};
}