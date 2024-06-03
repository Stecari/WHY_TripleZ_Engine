#include "tzpch.h"
#include "Scene.h"

#include "TribleZ_Core/Render/Renderer2D.h"
#include "ScriptableEntity.h"

#include "Entity.h"
#include "Component.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(͸�ӣ�ͶӰ��ϵ������)

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace TribleZ
{
	static b2BodyType RigidBodyTypeToBox2DType(RigidBody2DComponent::BodyType RBType)
	{
		switch (RBType)
		{
			case RigidBody2DComponent::BodyType::Static:	return b2BodyType::b2_staticBody;	
			case RigidBody2DComponent::BodyType::Dynamic:	return b2BodyType::b2_dynamicBody;
			case RigidBody2DComponent::BodyType::Kinematic:	return b2BodyType::b2_kinematicBody;
		}

		TZ_CORE_ASSERT("UNKOWN BODY TYPE!");
		return b2BodyType::b2_staticBody;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////������������ߺ���//////////////////////////////////////////////////////////////////////////////////////////////////////
	//���ע������������Ǩ�ƣ��������Ϊ����ע���
	template<typename Component>
	static void CopyComponent(const entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto& enti : view)
		{
			UUID uuid = src.get<IDComponent>(enti).ID;
			TZ_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity entiHandle = enttMap.at(uuid);

			auto& component = src.get<Component>(entiHandle);	//�õ� ָ�������ʵ�� �� ָ�����
			dst.emplace_or_replace<Component>(entiHandle, component);
		}
	}
	//���ʵ�����������Ǩ��
	template<typename Component>
	static void CopyComponentIfExists(Entity src, Entity dst)
	{
		if (src.HasComponent<Component>()) {
			dst.AddOrRepalceComponent<Component>(src.GetComponent<Component>());
		}
	}
	///////������������ߺ���//////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	{}

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

		/*---------------------��������-------------------------------------------------------------*/
		const int32_t velocityIterations = 6;	//�е������ݣ��������Ϊ�ٶȣ���С�˸����״�������
		const int32_t positionIterations = 2;	//������������ɢ��֮���

		m_PhysicsWorld->Step(timestep, velocityIterations, positionIterations);	//�������������Ϊ����һ��

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto& ent : view)
		{
			Entity entity = { ent, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;

			const auto& position = body->GetPosition();
			transform.Rotation.z = body->GetAngle();
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;
		}
		/*---------------------��������-------------------------------------------------------------*/


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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////��ʼ���У���������ϵͳ//////////////////////////////////////////////////////////////////////////////////////////////////
	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		b2BodyDef bodydef;

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto& ent : view)
		{
			Entity entity (ent, this);
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();	
			
			bodydef.type = RigidBodyTypeToBox2DType(rb2d.m_BodyType);
			//bodydef.fixedRotation = rb2d.FixRotation;
			bodydef.position.Set(transform.Translation.x, transform.Translation.y);
			bodydef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodydef);
			body->SetFixedRotation(rb2d.FixRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;

				const auto& size = bc2d.Size;
				boxShape.SetAsBox(size.x * transform.Scale.x, size.y * transform.Scale.y);

				/*---------�оߣ������������������԰󶨵�һ��body��-------------------------*/
				b2FixtureDef fixturedef;
				fixturedef.shape = &boxShape;
				fixturedef.density = bc2d.Density;
				fixturedef.friction = bc2d.friction;
				fixturedef.restitution = bc2d.Restitution;
				fixturedef.restitutionThreshold = bc2d.RestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixturedef);
				/*---------�оߣ������������������԰󶨵�һ��body��-------------------------*/

				bc2d.RuntimeFixture = fixture;
			}
		}
	}
	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}
	///////��ʼ���У���������ϵͳ//////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////����,���ƣ�ɾ��ʵ��//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Entity Scene::CreateEntity(UUID id, const std::string& name)
	{
		//ÿ��ʵ�����һ��entt::entity,��Scene*
		Entity entity = { m_Registry.create(), this };//this���Ƿ���ʹ�øú�����ʵ����ָ��
		//ÿ��ʵ��Ĭ���� �任��� Tag UUID
		entity.AddComponent<IDComponent>(id);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}
	/*
	Entity Scene::CreateEntity(const std::string& name)
	{
		//ÿ��ʵ�����һ��entt::entity,��Scene*
		Entity entity = { m_Registry.create(), this };//this���Ƿ���ʹ�øú�����ʵ����ָ��
		//ÿ��ʵ��Ĭ���� �任��� Tag UUID
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<IDComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	*/
	Entity Scene::CreateEntityWithUUID(UUID id, const std::string& name)
	{
		return CreateEntity(id, name);
	}
	Entity Scene::DuplicateEntity(Entity entity)		//void Scene::DuplicateEntity(Entity entity)	//Chernoд�ģ���ʵû���
	{
		auto& name = entity.GetName();
		Entity newEntity = CreateEntity(name);

		CopyComponentIfExists<TransformComponent>(entity, newEntity);
		CopyComponentIfExists<SpriteRendererComponent>(entity, newEntity);
		CopyComponentIfExists<CameraComponent>(entity, newEntity);
		CopyComponentIfExists<NativeScriptComponent>(entity, newEntity);
		CopyComponentIfExists<RigidBody2DComponent>(entity, newEntity);
		CopyComponentIfExists<BoxCollider2DComponent>(entity, newEntity);

		return newEntity;
	}
	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity);		//�Լ�д����ʽת��
	}
	///////������ɾ��ʵ��//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	

	Ref<Scene> Scene::Copy(const Ref<Scene>& otherScene)
	{
		Ref<Scene> newScene = CreatRef<Scene>();

		newScene->m_ViewWidth = otherScene->m_ViewWidth;
		newScene->m_ViewHeight = otherScene->m_ViewHeight;

		auto& srcRegistry = otherScene->m_Registry;
		auto& dstRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		//�ռ�ԭ�����и���ʵ���UUID����Ŀ�곡���д�������ô��ʵ��
		auto entView = srcRegistry.view<IDComponent>();
		for (auto& enti : entView)
		{
			UUID uuid = srcRegistry.get<IDComponent>(enti).ID;
			std::string name = srcRegistry.get<TagComponent>(enti).Tag;
			Entity newEntity= newScene->CreateEntityWithUUID(uuid, name);

			enttMap[uuid] = enti;
		}

		//�������������ID��Tag
		CopyComponent<TransformComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<CameraComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<RigidBody2DComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(srcRegistry, dstRegistry, enttMap);

		return newScene;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////������ʱ�Ļص���������������������У���Ȼ�޷�����//////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////�����ֶΣ���ģ���ļ�������cpp�ļ���Ϊ��ͬ�Ķ���ר�Ű����˶�Ӧ�ĺ���///////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*����������......*/
	template<typename T>
	void Scene::OnComponentAdding(Entity entity, T& component)
	{
		//static_assert(false);		//��������⣬һֱ����
	}
	/*------�൱���������Ǹ�ģ�庯������չ��ʽ-----------------------------------------------------------*/
	template<>
	void Scene::OnComponentAdding<IDComponent>(Entity entity, IDComponent& component) {}

	template<>
	void Scene::OnComponentAdding<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdding<TransformComponent>(Entity entity, TransformComponent& component)	{}

	template<>
	void Scene::OnComponentAdding<CameraComponent>(Entity entity, CameraComponent& component)
	{
		//component.Camera.OnResizeView(m_ViewWidth, m_ViewHeight);
		if (m_ViewWidth > 0 && m_ViewHeight > 0){
			component.Camera.OnResizeView(m_ViewWidth, m_ViewHeight);
		}
	}

	template<>
	void Scene::OnComponentAdding<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template<>
	void Scene::OnComponentAdding<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	void Scene::OnComponentAdding<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component){}

	template<>
	void Scene::OnComponentAdding<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component){}
	/*----------------------------------------------------------------------------------------------------*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	


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