#include "SceneHierarchyPanel.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "glm/gtc/type_ptr.hpp"
#include "Scene/Component.h"

namespace TribleZ
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_SelectionContext = {};	//Ϊ�˷�ֹ����һ���µ�Scene��ʱ���ϵ�Scene��ָ��Υ��������Ҫ�ÿ�
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("SceneHierarchyPanel");

		/*
		* �°��entt�е�registryû��each������
		* ������m_Registry.view<entt::entity>()����������ÿһ��entity��Ȼ���ڵ���each����
		* ��������������ʵ�������е�entityĬ����һ��TagComponent��TransfoemComponent
		* ��������������д��Ҳ��������������
		* ����m_Registryû��each�����ˣ�����һ�ָ㷨*/
		//m_Context->m_Registry.view<entt::entity>().each([&](auto entityID) {});
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, TagComponent& TagCom)
		{
			Entity entity{ entityID, m_Context.get()};
			DrawEntityNode(entity);
		});
																						/*���������Ҫ��Item������ʾ*/
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))	//����һ�����window�󵯳��Ĵ��ڣ�flagָ���˵���ĸ���
		{
			if (ImGui::MenuItem("Create New Entity")){
				m_Context->CreateEntity("Empty Entity").GetComponent<TransformComponent>().Scale = glm::vec3{ 200.0f };	//�����ҵ����ұ߿����õ���720��1280�����Բ����óߴ�Ϳ����������Ǽ�������Ҫ����������ǳߴ��Ҫ���1.0(��Ȼ�ͱ�����������ж����Ŵ�200����)
				//m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext){
			//��ʾ��������ѡʵ������
			DrawComponent(m_SelectionContext);
		}

		ImGui::End();
	}

	//void SceneHierarchyPanel::DrawEntityNode(Entity& entity)	//chernoд����û��&�������Ǹ��������Ҿ��ü���&Ҳ���ԣ��ȷ��ź���
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string tag = entity.GetComponent<TagComponent>().Tag;

		//�жϵ�ǰ���Ľڵ��ʵ���Ƿ������ǵ�ǰ��ѡ�еĽڵ�
		//flag����һ��imgui����ı�׼
		ImGuiTreeNodeFlags flag = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flag |= ImGuiTreeNodeFlags_SpanAvailWidth;		//���е����ض����Ա�ѡ�У��������ͣ

		bool Node_Expanded_first = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tag.c_str());		//���ظ���״�ڵ��Ƿ�չ��
		if (ImGui::IsItemClicked())	//������һ��item�Ƿ񱻵��(������һ��TreeNode)
		{
			m_SelectionContext = entity;
		}

		//�����ǰ���ڵĿհ״�ȡ��ѡ��
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && 
			ImGui::IsWindowHovered(ImGuiHoveredFlags_None/*���������Ĭ�ϵ�*/))	//���ģʽ�½��ᱻpopupʽѡ���赲(treenode�Ǹ�����һ��)������˵���ͣ��popupʽѡ�����ǲ���hover�ڴ����ϵ�
		{									/*���������ܶ���������Ե��ȥ����*/
			m_SelectionContext = {};	//ȡ��ѡ��
		}

		bool delete_entity_flag = false;
		if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))	//����һ����� ��ӦItem�󵯳��Ĵ��ڣ�flagָ���˵���ĸ���
		{
			if (ImGui::MenuItem("Delete Entity")) {
				delete_entity_flag = true;
			}
			ImGui::EndPopup();
		}

		if (Node_Expanded_first)
		{
			//���µ���(��չ)�˵������Ա��պ�����Ӹ���	//�Ҹо����������	select��Ľ�һ���й�ϵ��������֪û������Ļ���select���ǲ����������
			//ImGui::TreePop();		
			ImGuiTreeNodeFlags flag = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow ;
			flag |= ImGuiTreeNodeFlags_SpanAvailWidth;

			bool Node_Expanded_second = ImGui::TreeNodeEx((void*)825006, flag, tag.c_str());	//������õ����Կ�
			if (Node_Expanded_second){
				ImGui::TreePop();
			}
			ImGui::TreePop();		//�ҵ�����Ƿ������棬���Ǳ�׼�Ƿ������棬��ʱ���һ�£�  ��������Ҳ���ã�����չ������Ȼ��������ƽʱ��Ҫ�����ֽ���ʽ��
		}

		if (delete_entity_flag) {
			m_Context->DeleteEntity(m_SelectionContext);	//ɾ��ѡ�е�Item��entity
			/*����е�bug����ѡ����һ����Ȼ���Ҽ���һ��Item,ɾ�����Ļ���ѡ�е����*/
			if (m_SelectionContext == entity) {			//��������벻����
				m_SelectionContext = {};				//�ͷŵ�ǰѡ�е�Item��ָ�루���ͷŻ���ַǷ���ָ�룩
			}
		}

	}



	/*cherno�Լ�д��һ�������������������DragFloat3�ĵģ���������Լ�ѧһ��
	* ����ȥImGui�ĸ��ֲ�������ģʽ�������и�Push��Pop�йصĲ�����þ�ʽջ�Ĳ���
	* ����������ɫ����һ����Ҫ����ɫ�Ͷ�Ӧ�Ķ���push��ջ���棬��һ��ʹ�ú���ʱ��ʹ��
	* ʹ������ٽ��Ǹ���ɫ�����ø�Pop���������Բ�pop�ᱨ��*/
	static void DrawVec3Controler(const std::string& lable, glm::vec3& value, float reset_value = 0.0f, float column_width = 100.0f)
	{
		ImGuiIO& imgui_io = ImGui::GetIO();
		auto boldFont = imgui_io.Fonts->Fonts[1];		//ѡ��洢��1������


		ImGui::PushID(lable.c_str());	//���������DragFloat()���lable�õĶ���X,Y,Z������ʲô����������ô����ʹ�����Lable��DragFloat����ͬʱ�仯��PushID�൱�ڿ�����һ��namespace����Ȼ���ҲҪPOP��

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, column_width);
		ImGui::Text(lable.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

									/*�����С*/
		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { LineHeight + 3.0f, LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.85f, 0.1f, 0.2f, 1.0f });			//(1)
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.90f, 0.3f, 0.4f, 1.0f });		//(2)
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.70f, 0.1f, 0.15f, 1.0f });		//(3)
		ImGui::PushFont(boldFont);
		if (ImGui::Button("x", buttonSize)){
			value.x = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);					//������3����ɫ����ȫ������

		ImGui::SameLine();			/*��Щ���ǳ���Ĭ��ֵ     С��λ��*/
		ImGui::DragFloat("##X", &value.x, 1.0f, 0.0f, 0.0f, "%.2f");		//����##�Ļ�������д���Ǵ�д����Сд��ʾ�Ķ���Сд������##����ʾ��д
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.85f, 0.2f, 1.0f });			//(1)
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.9f, 0.4f, 1.0f });		//(2)
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.70f, 0.15f, 1.0f });		//(3)
		ImGui::PushFont(boldFont);
		if (ImGui::Button("y", buttonSize)) {
			value.y = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);					//������3����ɫ����ȫ������

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.1f, 0.85f, 1.0f });			//(1)
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.4f, 0.3f, 0.90f, 1.0f });		//(2)
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.15f, 0.1f, 0.70f, 1.0f });		//(3)
		ImGui::PushFont(boldFont);
		if (ImGui::Button("z", buttonSize)) {
			value.z = reset_value;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);					//������3����ɫ����ȫ������

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::SameLine();		//�����ҪSameLine��....��Ȼ�Ļ���һ�����������Item�ͻ���Z��һ���ˡ�����

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	//�ѻ���ÿ��ʵ�������ĺ�����ģ�廯���������UIFunc��д��û������Ҫ��ʱ��ѧһ��
	template<typename ComponentT, typename UIfunc>
	static void BuildPerComponent(const std::string& name, Entity entity, UIfunc uiFunction)
	{
		const ImGuiTreeNodeFlags Guitreenodeflag_default = ImGuiTreeNodeFlags_DefaultOpen		|	//Ĭ��չ��״̬
														   ImGuiTreeNodeFlags_AllowOverlap		|	//����Item�ѵ�				//Chernoû��������Ҿ���Ҫ��
														   ImGuiTreeNodeFlags_Framed			|	//��������ʾ
														   ImGuiTreeNodeFlags_FramePadding		|	//Display a bullet instead of arrow		�е㿴��������ʲô��
														   ImGuiTreeNodeFlags_SpanAvailWidth	;	//�������촥������

		if (entity.HasComponent<ComponentT>())
		{
			auto& component = entity.GetComponent<ComponentT>();
			ImVec2	GuiRegionAvail = ImGui::GetContentRegionAvail();	/*��ȡ���ݣ���������*/

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });		//StyleVar������VarStyle����˼��
			float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;		//�����и�

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(ComponentT).hash_code(), Guitreenodeflag_default, name.c_str());	//�������׼��
			ImGui::PopStyleVar();
			//ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);	//ֱ�ӽ����ڴ�С�ù������ˣ��ӺŻᱻ��������ס
								/*ֱ���������ȡ�Ļ���������Ѿ����˵Ĳ��֣����Ի�ǰ��*/
			ImGui::SameLine(/*ImGui::GetContentRegionAvail().x*/GuiRegionAvail.x - LineHeight /* * 0.5 cherno��������������ҵ�û�У������ImGui�ָ�ϸ����*/);
			if (ImGui::Button("+", ImVec2{ LineHeight, LineHeight })) {
				ImGui::OpenPopup("Component Setting");
			}
			bool remove = false;
			if (ImGui::BeginPopup("Component Setting"))
			{
				if (ImGui::MenuItem("Remove Component")) {
					remove = true;
				}
				ImGui::EndPopup();
			}


			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (remove) {
				entity.RemoveComponent<ComponentT>();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponent(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())		//������һ��bug�������ڸı�Tag�Ĺ����к�Ȼ��;�л�����һ�����ʱ����ı���һ�������Tag
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];		//256��ImGui�����������ַ���������Ҫ���ô�һ��
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());		//��ʼ��������ı�,���Ҫ��strcpy����ȫ����Ϊָ���˴�С
								/*����##���Բ���ʾ��Tag������*/
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))		//�����ı�,�з����ı�ʱ����true
			{
				tag = std::string(buffer);		//�޸ı�ǩ
			}
		}


		ImGui::SameLine();
		ImGui::PushItemWidth(-1);					//���Ҫ�о�һ����ʲô��˼��
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				/*�����������ŵ�OnComponentAdding()����*/
				m_SelectionContext.GetComponent<TransformComponent>().Scale = glm::vec3{ 1.0f };	//����ʽ����Ļ���ʵ���СҪ���1����Ȼ�������ţ����е�ʵ�嶼�ᱻ�Ŵ���ʾ�ߴ籶��
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		const ImGuiTreeNodeFlags Guitreenodeflag_default = ImGuiTreeNodeFlags_DefaultOpen ;	//Ĭ��չ����������ѵ�

		BuildPerComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
		{
			auto& translation = component.Translation;
			auto& rotation = component.Rotation;
			glm::vec3 rotation_deg = glm::degrees(rotation);
			auto& scale = component.Scale;
			DrawVec3Controler("Position", translation);
			DrawVec3Controler("Rotate", rotation_deg);
			DrawVec3Controler("Scale", scale, 200.0f);
			rotation = glm::radians(rotation_deg);
		});

		BuildPerComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& Camera = component.Camera;

			ImGui::Checkbox("Main Camera", &component.Primary);

			const char* CameroProjectionTypestring[] = { "Perspective", "OrthoGraphic" };
			const char* CurrentCameroProjTypestring = CameroProjectionTypestring[(int)Camera.GetProjType()];
			/*��ʾ������*/
			if (ImGui::BeginCombo("Proection", CurrentCameroProjTypestring))
			{
				for (int i = 0; i < 2; i++)
				{
					bool IsSelected = CameroProjectionTypestring[i] == CurrentCameroProjTypestring;
					if (ImGui::Selectable(CameroProjectionTypestring[i], IsSelected))
					{
						CurrentCameroProjTypestring = CameroProjectionTypestring[i];	//�Ҿ;�����䲻��Ҫ
						Camera.SetProjectionType((SceneCamera::CameraProjectionType)i);
					}

					if (IsSelected){
						ImGui::SetItemDefaultFocus();	//ò��ʽ����һ��Ĭ��ѡ�е�ѡ��(Ϊ��һ����Ŀ)����Ȼ�����ڻ����Ǻ���⣬��ʱ���ٲ�һ��
					}
				}
				ImGui::EndCombo();
			}

			if (Camera.GetProjType() == SceneCamera::CameraProjectionType::Perspective)
			{
				float verticalFOV = glm::degrees(Camera.GetPerspectiveFOV());
				if (ImGui::DragFloat("VerticalFov", &verticalFOV)){
					Camera.SetPerspectiveFOV(glm::radians(verticalFOV));
				}

				float perspectiveNear = Camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("PerspectiveNear", &perspectiveNear)){
					Camera.SetPerspectiveNearClip(perspectiveNear);
				}

				float perspectiveFar = Camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("PerspectiveFar", &perspectiveFar)){
					Camera.SetPerspectiveFarClip(perspectiveFar);
				}

			}
			if (Camera.GetProjType() == SceneCamera::CameraProjectionType::OrthoGraphic)
			{
				float size = Camera.GetOrthoGraphicSize();
				if (ImGui::DragFloat("OrthoSize", &size)){
					Camera.SetOrthoGraphicSize(size);
				}

				float nearClip = Camera.GetOrthoGraphicNearClip();
				if (ImGui::DragFloat("OrthoNearClip", &nearClip)){
					Camera.SetOrthoGraphicNearClip(nearClip);
				}

				float farClip = Camera.GetOrthoGraphicFarClip();
				if (ImGui::DragFloat("OrthofarClip", &farClip)){
					Camera.SetOrthoGraphicFarClip(farClip);
				}

				ImGui::Checkbox("Camera Fix Aspect", &component.FixedAspectRatio);
			}
		});

		BuildPerComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
		{
			auto& renderer_color = component.Color;
			if (ImGui::ColorEdit4("Sprite Renderer", glm::value_ptr(renderer_color) ) )
			{}
		});
		
	}
}