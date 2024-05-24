#include <tzpch.h>
#include <ImGui/imgui.h>
#include "ContentBrowserPanels.h"

namespace TribleZ
{
	//const std::filesystem::path g_AssertPath = "asserts";
	extern const std::filesystem::path g_AssertPath = "asserts";
	/*不知道是不是C++的特性，假如要将一个const变量用作全局变量，那么所有的变量声明都得加上extern，包括第一个定义的声明*/

	ContentBrowserPanels::ContentBrowserPanels()
		:m_CurrentPath(g_AssertPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrower/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrower/FileIcon.png");
	}

	void ContentBrowserPanels::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentPath != std::filesystem::path(g_AssertPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		static float thumbnailSize = 128.0;			//文件缩略图的尺寸
		static float padding = 16.0;				//文件缩略图周围一圈填充的尺寸
		float cellSize = thumbnailSize + padding;	//一行的大小

		float panelWeidth = ImGui::GetContentRegionAvail().x;	//获取空区的宽度
		int colunmCount = (int)(panelWeidth / cellSize);
		if (colunmCount < 1) {
			colunmCount = 1;
		}

		ImGui::Columns(colunmCount, 0, false);

		for (auto& directEntry : std::filesystem::directory_iterator(m_CurrentPath))
		{
			const auto& filepath = directEntry.path();
			std::string filename = filepath.filename().string();
			std::filesystem::path relativePath = std::filesystem::relative(directEntry, g_AssertPath);
			std::string relativePathstring = relativePath.string();

			Ref<Texture2D> Icon = directEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushID(filename.c_str());		//因为ImGui内部是通过每个ImageButton的ID来区分你不同的按钮，假如你不提供一个你自己的ID那么默认使用的就是纹理的ID
						/*将文件名用作id*/			//不提供的话，因为我们所有的文件按钮用的都是一张图片，TextureID也就是一样的，这里提供的ID可以是一串字符，也可以是一个数字

			//ImGui::PushStyleColor(ImGuiCol_Button, { 0.0, 0.0, 0.0, 0.0 });
			ImGui::ImageButton((ImTextureID)Icon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropSource())
			{
				//TZ_CORE_INFO("Drag_Drop");
				const wchar_t* ItemPath = relativePath.c_str();		//这个API有点抽象啊
				ImGui::SetDragDropPayload("Content_Browers_panel", ItemPath, (wcslen(ItemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();										/*+1加的是最后的空字符串终止符，不然会报异常*/
			}
			//ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directEntry.is_directory())
				{
					m_CurrentPath /= filepath.filename();
					//m_CurrentPath = path;
				}
			}

			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("thumbnailSize", &thumbnailSize, 16.0f, 512.0f, "%.2f");
		ImGui::SliderFloat("padding", &padding, 1.0f, 32.0f, "%.2f");

		ImGui::End();
	}
}