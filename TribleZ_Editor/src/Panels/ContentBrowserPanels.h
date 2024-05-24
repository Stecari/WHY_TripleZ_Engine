#pragma once
#include <filesystem>
#include "TribleZ_Core/Render/Renderer2D.h"

namespace TribleZ
{
	class ContentBrowserPanels
	{
	private:
		std::filesystem::path m_CurrentPath;

		//Àı¬‘ÕºÕº±Í
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

	public:
		ContentBrowserPanels();

		void OnImGuiRender();

	};
}