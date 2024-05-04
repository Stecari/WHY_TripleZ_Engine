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
		m_SelectionContext = {};	//为了防止创建一个新的Scene的时候老的Scene的指针违法，所以要置空
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("SceneHierarchyPanel");

		/*
		* 新版的entt中的registry没有each函数了
		* 可以用m_Registry.view<entt::entity>()代替来遍历每一个entity，然后在调用each函数
		* 但这里由于我们实际上所有的entity默认有一个TagComponent和TransfoemComponent
		* 所以用现在这种写法也可以甚至更合理
		* 这里m_Registry没有each函数了，换了一种搞法*/
		//m_Context->m_Registry.view<entt::entity>().each([&](auto entityID) {});
		m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, TagComponent& TagCom)
		{
			Entity entity{ entityID, m_Context.get()};
			DrawEntityNode(entity);
		});
																						/*这个让他不要在Item上面显示*/
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))	//创建一个点击window后弹出的窗口，flag指定了点击哪个键
		{
			if (ImGui::MenuItem("Create New Entity")){
				m_Context->CreateEntity("Empty Entity").GetComponent<TransformComponent>().Scale = glm::vec3{ 200.0f };	//由于我的左右边框设置的是720和1280，所以不设置尺寸就看不到，但是假如我想要加入相机，那尺寸就要变回1.0(不然就变成这个相机所有东西放大200倍了)
				//m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext){
			//显示出我们所选实体的组件
			DrawComponent(m_SelectionContext);
		}

		ImGui::End();
	}

	//void SceneHierarchyPanel::DrawEntityNode(Entity& entity)	//cherno写的是没有&的下面那个，但是我觉得加上&也可以，先放着好了
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string tag = entity.GetComponent<TagComponent>().Tag;

		//判断当前画的节点的实体是否是我们当前被选中的节点
		//flag像是一个imgui里面的标准
		ImGuiTreeNodeFlags flag = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flag |= ImGuiTreeNodeFlags_SpanAvailWidth;		//整行的像素都可以被选中，被鼠标悬停

		bool Node_Expanded_first = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tag.c_str());		//返回该树状节点是否被展开
		if (ImGui::IsItemClicked())	//返回上一个item是否被点击(这里是一个TreeNode)
		{
			m_SelectionContext = entity;
		}

		//点击当前窗口的空白处取消选中
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && 
			ImGui::IsWindowHovered(ImGuiHoveredFlags_None/*这个参数是默认的*/))	//这个模式下将会被popup式选项阻挡(treenode那个就算一种)，简单来说鼠标停在popup式选项上是不算hover在窗口上的
		{									/*这里可以填很多参数，可以点进去看看*/
			m_SelectionContext = {};	//取消选中
		}

		bool delete_entity_flag = false;
		if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))	//创建一个点击 对应Item后弹出的窗口，flag指定了点击哪个键
		{
			if (ImGui::MenuItem("Delete Entity")) {
				delete_entity_flag = true;
			}
			ImGui::EndPopup();
		}

		if (Node_Expanded_first)
		{
			//向下弹出(扩展)菜单栏，以便日后添加子概念	//我感觉这个函数跟	select框的进一格有关系，但是已知没有这个的话，select框是不允许被点击的
			//ImGui::TreePop();		
			ImGuiTreeNodeFlags flag = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow ;
			flag |= ImGuiTreeNodeFlags_SpanAvailWidth;

			bool Node_Expanded_second = ImGui::TreeNodeEx((void*)825006, flag, tag.c_str());	//随便设置的试试看
			if (Node_Expanded_second){
				ImGui::TreePop();
			}
			ImGui::TreePop();		//我的理解是放在上面，但是标准是放在下面，到时候查一下？  放在上面也能用，但是展开后显然不是我们平时想要的这种阶梯式的
		}

		if (delete_entity_flag) {
			m_Context->DeleteEntity(m_SelectionContext);	//删除选中的Item的entity
			/*这个有点bug，我选择了一个，然后右键另一个Item,删除掉的还是选中的这个*/
			if (m_SelectionContext == entity) {			//这个是我想不到的
				m_SelectionContext = {};				//释放当前选中的Item的指针（不释放会出现非法空指针）
			}
		}

	}



	/*cherno自己写的一个控制组件，是他根据DragFloat3改的，这个可以自己学一下
	* 看上去ImGui的各种参数设置模式包括所有跟Push和Pop有关的操作因该就式栈的操作
	* 比如配置颜色，将一个需要的颜色和对应的对象push到栈里面，下一次使用函数时在使用
	* 使用完后再将那个颜色和设置给Pop出来，所以不pop会报错*/
	static void DrawVec3Controler(const std::string& lable, glm::vec3& value, float reset_value = 0.0f, float column_width = 100.0f)
	{
		ImGuiIO& imgui_io = ImGui::GetIO();
		auto boldFont = imgui_io.Fonts->Fonts[1];		//选择存储的1号字体


		ImGui::PushID(lable.c_str());	//由于下面的DragFloat()里的lable用的都是X,Y,Z，假如什么都不区分那么所有使用这个Lable的DragFloat都会同时变化。PushID相当于开辟了一个namespace，当然最后也要POP它

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, column_width);
		ImGui::Text(lable.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

									/*字体大小*/
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
		ImGui::PopStyleColor(3);					//把上面3个颜色设置全部弹出

		ImGui::SameLine();			/*这些都是炒的默认值     小数位数*/
		ImGui::DragFloat("##X", &value.x, 1.0f, 0.0f, 0.0f, "%.2f");		//不带##的话无论你写的是大写还是小写显示的都是小写，带上##就显示大写
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
		ImGui::PopStyleColor(3);					//把上面3个颜色设置全部弹出

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
		ImGui::PopStyleColor(3);					//把上面3个颜色设置全部弹出

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::SameLine();		//这个不要SameLine了....不然的话下一个画在下面的Item就画在Z这一行了。。。

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	//把绘制每个实体的组件的函数给模板化，就是这个UIFunc的写法没看懂，要抽时间学一下
	template<typename ComponentT, typename UIfunc>
	static void BuildPerComponent(const std::string& name, Entity entity, UIfunc uiFunction)
	{
		const ImGuiTreeNodeFlags Guitreenodeflag_default = ImGuiTreeNodeFlags_DefaultOpen		|	//默认展开状态
														   ImGuiTreeNodeFlags_AllowOverlap		|	//允许Item堆叠				//Cherno没用这个，我觉得要用
														   ImGuiTreeNodeFlags_Framed			|	//框框高亮显示
														   ImGuiTreeNodeFlags_FramePadding		|	//Display a bullet instead of arrow		有点看不出来有什么用
														   ImGuiTreeNodeFlags_SpanAvailWidth	;	//向右延伸触发区域

		if (entity.HasComponent<ComponentT>())
		{
			auto& component = entity.GetComponent<ComponentT>();
			ImVec2	GuiRegionAvail = ImGui::GetContentRegionAvail();	/*获取内容：可用区域*/

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });		//StyleVar可能是VarStyle的意思？
			float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;		//计算行高

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(ComponentT).hash_code(), Guitreenodeflag_default, name.c_str());	//视屏里标准的
			ImGui::PopStyleVar();
			//ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);	//直接将窗口大小拿过来用了，加号会被滚动条挡住
								/*直接在这里获取的话，会剪掉已经用了的部分，所以会前移*/
			ImGui::SameLine(/*ImGui::GetContentRegionAvail().x*/GuiRegionAvail.x - LineHeight /* * 0.5 cherno的有这个，但是我的没有，大概是ImGui又改细节了*/);
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
		if (entity.HasComponent<TagComponent>())		//这里有一个bug，当正在改变Tag的过程中忽然中途切换到另一个组件时，会改变另一个组件的Tag
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];		//256是ImGui中最大的输入字符量，我们要设置大一点
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());		//初始化输入的文本,这个要比strcpy更安全，因为指定了大小
								/*加上##可以不显示“Tag”文字*/
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))		//输入文本,有发生改变时返回true
			{
				tag = std::string(buffer);		//修改标签
			}
		}


		ImGui::SameLine();
		ImGui::PushItemWidth(-1);					//这个要研究一下是什么意思了
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
				/*下面这个我想放到OnComponentAdding()里面*/
				m_SelectionContext.GetComponent<TransformComponent>().Scale = glm::vec3{ 1.0f };	//假如式相机的话，实体大小要变回1，不然会变成缩放，所有的实体都会被放大显示尺寸倍数
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		const ImGuiTreeNodeFlags Guitreenodeflag_default = ImGuiTreeNodeFlags_DefaultOpen ;	//默认展开并且允许堆叠

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
			/*显示的内容*/
			if (ImGui::BeginCombo("Proection", CurrentCameroProjTypestring))
			{
				for (int i = 0; i < 2; i++)
				{
					bool IsSelected = CameroProjectionTypestring[i] == CurrentCameroProjTypestring;
					if (ImGui::Selectable(CameroProjectionTypestring[i], IsSelected))
					{
						CurrentCameroProjTypestring = CameroProjectionTypestring[i];	//我就觉得这句不需要
						Camera.SetProjectionType((SceneCamera::CameraProjectionType)i);
					}

					if (IsSelected){
						ImGui::SetItemDefaultFocus();	//貌似式设置一个默认选中的选项(为上一个项目)，显然我现在还不是很理解，到时候再查一下
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