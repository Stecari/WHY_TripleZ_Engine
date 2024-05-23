#include "tzpch.h"
#include "Scene.h"

#include "TribleZ_Core/Render/Renderer2D.h"

#include "Entity.h"
#include "Component.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(͸�ӣ�ͶӰ��ϵ������)


namespace TribleZ
{
	static void TransformFunc(entt::registry& registry, entt::entity entity){}
	static void DoMath(glm::mat4 transform){}

	Scene::Scene()
	{
		//m_Registry.on_construct<CameraComponent>().connect<&Function>();		//���Խ�ĳ�������һ�������Զ�����
#ifdef old
		/*
		TransformComponent Transform_block;		//�任�����
		DoMath(Transform_block);		//��ʽת��
		*/
		entt::entity entity = m_Registry.create();		//����ʵ��ID

		/*����ڶ�����������Ӧ�ø�ģ�庯���е�ָ������
		 *�����������������Ǹ���ʽת���Ĳ�����
		 *����ֱ����һ��glm::mat4�Ϳ��Ա�������ʽת����TransformComponent
		 *�����ǲ���һ��ָ������,���ز�����Ǹ����� */
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		//m_Registry.remove<>();

		if (m_Registry.all_of<TransformComponent>(entity))//��Ƶ���õ���has���������ڸ����ˣ����þ��Ǽ���Ƿ����
		{
			TransformComponent& trans = m_Registry.get<TransformComponent>(entity);		//�����壬��ȡ������
		}

		m_Registry.on_construct<TransformComponent>().connect<&TransformFunc>();

		auto view = m_Registry.view<TransformComponent>();//��ȡһ���ڲ���ͼ
		for(auto entity : view)		//������ͼ����ʵ��
		{
			//auto& trans = m_Registry.get<TransformComponent>(entity);
			auto& trans = view.get<TransformComponent>(entity);		//Ч�ʸ��ߣ�view�����������Ϊ�˸������
		}
#endif			
	}

	Scene::~Scene()
	{

	}

	void Scene::OnUpdataEditor(TimeStep timestep, Editor_Camera Edi_Camera)
	{
		Renderer2D::SceneBegin(Edi_Camera);
		/*�������ʲô����ʱ���һ��*/
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);//��ȡһ���ڲ� ��,ͬʱ����ӵ�����������������
		for (auto entity : group)		//���� �� ����ʵ��
		{
			auto&& [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);	//C++17 �����ԣ������tuple
			//Renderer2D::DrawQuad(trans.GetTransform(), sprite);
			Renderer2D::DrawSprite(trans.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::SceneEnd();
	}

	void Scene::OnUpdataRuntime(TimeStep timestep)
	{
		//���� �ű�����
		{
			//�����ʱ����Ҫ�ƶ���OnScenePlay
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& NativeScriptCom)
			{
				if (!NativeScriptCom.Entity_Instence)
				{
					NativeScriptCom.Entity_Instence = NativeScriptCom.InstentiateScript();
					NativeScriptCom.Entity_Instence->m_Entity = Entity{ entity, this };
					NativeScriptCom.Entity_Instence->OnCreate();
				}

				NativeScriptCom.Entity_Instence->OnUpdata(timestep);

			});
		}


		BaseCamera* main_camera = nullptr;
		TransformComponent* camera_trans = nullptr;
		{
			/*
			*������group�ᱨ����Ϊ����ı���������group����entt�����ߵĻ���˵����
			*They are not. Groups have an initialization step that you don't want to pay all the times. 
			*Therefore, you undergo this cost once (when the pools are likely empty or almost empty) and that's it.
			*������һֱʹ��view��ֱ�������ڴ�������⣬�Ҿ��ÿ��Ե��� ջ����(view) �� �ѷ���(group) �����������
			auto group = m_Registry.group<TransformComponent, CameraComponent>();*/
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)		//���� �� ����ʵ��
			{
				auto&& [trans, camera] = view.get<TransformComponent, CameraComponent>(entity);	//C++17 �����ԣ������tuple

				if (camera.Primary)
				{
					main_camera = &camera.Camera;
					camera_trans = &trans;
					break;
				}
				//Renderer2D::DrawQuad(camera, trans);
			}
		}
		
		//�����ҵ������������Ϊ�գ�
		if (main_camera)
		{
			//��ʼ����BeginScene
			Renderer2D::SceneBegin( *main_camera, camera_trans->GetTransform());
															/*�������ʲô����ʱ���һ��*/
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);//��ȡһ���ڲ� ��,ͬʱ����ӵ�����������������
			for (auto entity : group)		//���� �� ����ʵ��
			{
				auto&& [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);	//C++17 �����ԣ������tuple
				//Renderer2D::DrawQuad(trans.GetTransform(), sprite);
				Renderer2D::DrawSprite(trans.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::SceneEnd();
		}
															
	}

	void Scene::ResizeView(uint32_t width, uint32_t height)
	{
		m_ViewWidth = width;
		m_ViewHeight = height;

		//�Գ���Ȳ��̶���������г��������
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)		//�ж��Ƿ�Ϊ����Ȳ��̶������
			{
				camera.Camera.OnResizeView(width, height);
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		//ÿ��ʵ�����һ��entt::entity,��Scene*
		Entity entity = { m_Registry.create(), this };//this���Ƿ���ʹ�øú�����ʵ����ָ��
		//Ĭ��ÿ��ʵ����һ���任�����Tag���
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	//void Scene::DeleteEntity(const Entity& entity)		//��ϲ������
	//{
	//	m_Registry.destroy(entity);		//�Լ�д����ʽת��
	//}
	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity);		//�Լ�д����ʽת��
	}

	/*����������......*/
	/*-------�����ֶΣ���ģ���ļ�������cpp�ļ���Ϊ��ͬ�Ķ���ר�Ű����˶�Ӧ�ĺ���-------------------------*/
	template<typename T>
	void Scene::OnComponentAdding(Entity entity, T& component)
	{
		//static_assert(false);		//��������⣬һֱ����
	}
	/*------�൱���������Ǹ�ģ�庯������չ��ʽ-----------------------------------------------------------*/
	template<>
	void Scene::OnComponentAdding<TagComponent>(Entity entity, TagComponent& component)
	{}

	template<>
	void Scene::OnComponentAdding<TransformComponent>(Entity entity, TransformComponent& component)
	{}

	template<>
	void Scene::OnComponentAdding<CameraComponent>(Entity entity, CameraComponent& component)
	{
		//component.Camera.OnResizeView(m_ViewWidth, m_ViewHeight);
		if (m_ViewWidth > 0 && m_ViewHeight > 0){
			component.Camera.OnResizeView(m_ViewWidth, m_ViewHeight);
		}
	}

	template<>
	void Scene::OnComponentAdding<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{}

	template<>
	void Scene::OnComponentAdding<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{}
	/*----------------------------------------------------------------------------------------------------*/

	//��������
	Entity Scene::GetPrimaryCameraEntity()
	{
		auto View = m_Registry.view<CameraComponent>();
		for (auto entity : View)
		{
			const auto& camera = View.get<CameraComponent>(entity);
			if (camera.Primary){
				return Entity{ entity, this };
			}
		}
		return {};
	}
}