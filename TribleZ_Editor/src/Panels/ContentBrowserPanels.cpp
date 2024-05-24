#include <tzpch.h>
#include <ImGui/imgui.h>
#include "ContentBrowserPanels.h"

namespace TribleZ
{
	//const std::filesystem::path g_AssertPath = "asserts";
	extern const std::filesystem::path g_AssertPath = "asserts";
	/*��֪���ǲ���C++�����ԣ�����Ҫ��һ��const��������ȫ�ֱ�������ô���еı����������ü���extern��������һ�����������*/

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

		static float thumbnailSize = 128.0;			//�ļ�����ͼ�ĳߴ�
		static float padding = 16.0;				//�ļ�����ͼ��ΧһȦ���ĳߴ�
		float cellSize = thumbnailSize + padding;	//һ�еĴ�С

		float panelWeidth = ImGui::GetContentRegionAvail().x;	//��ȡ�����Ŀ��
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
			ImGui::PushID(filename.c_str());		//��ΪImGui�ڲ���ͨ��ÿ��ImageButton��ID�������㲻ͬ�İ�ť�������㲻�ṩһ�����Լ���ID��ôĬ��ʹ�õľ��������ID
						/*���ļ�������id*/			//���ṩ�Ļ�����Ϊ�������е��ļ���ť�õĶ���һ��ͼƬ��TextureIDҲ����һ���ģ������ṩ��ID������һ���ַ���Ҳ������һ������

			//ImGui::PushStyleColor(ImGuiCol_Button, { 0.0, 0.0, 0.0, 0.0 });
			ImGui::ImageButton((ImTextureID)Icon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropSource())
			{
				//TZ_CORE_INFO("Drag_Drop");
				const wchar_t* ItemPath = relativePath.c_str();		//���API�е����
				ImGui::SetDragDropPayload("Content_Browers_panel", ItemPath, (wcslen(ItemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();										/*+1�ӵ������Ŀ��ַ�����ֹ������Ȼ�ᱨ�쳣*/
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