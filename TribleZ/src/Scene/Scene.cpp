#include "tzpch.h"
#include "Scene.h"

#include "TribleZ_Core/Render/Renderer2D.h"
#include "ScriptableEntity.h"

#include "Entity.h"
#include "Component.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(透视，投影关系、比例)

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
	///////复制组件，工具函数//////////////////////////////////////////////////////////////////////////////////////////////////////
	//针对注册表间的组件复制迁移，可以理解为复制注册表
	template<typename Component>
	static void CopyComponent(const entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto& enti : view)
		{
			UUID uuid = src.get<IDComponent>(enti).ID;
			TZ_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity entiHandle = enttMap.at(uuid);

			auto& component = src.get<Component>(entiHandle);	//拿到 指定句柄的实体 的 指定组件
			dst.emplace_or_replace<Component>(entiHandle, component);
		}
	}
	//针对实体间的组件复制迁移
	template<typename Component>
	static void CopyComponentIfExists(Entity src, Entity dst)
	{
		if (src.HasComponent<Component>()) {
			dst.AddOrRepalceComponent<Component>(src.GetComponent<Component>());
		}
	}
	///////复制组件，工具函数//////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	Scene::Scene()
	{
		//m_Registry.on_construct<CameraComponent>().connect<&Function>();		//可以将某个组件绑定一个函数自动调用
#ifdef old
		/*
		TransformComponent Transform_block;		//变换组件块
		DoMath(Transform_block);		//隐式转换
		*/
		entt::entity entity = m_Registry.create();		//生成实体ID

		/*这里第二个参数本来应该给模板函数中的指定类型
		 *但是由于我们做了那个隐式转换的操作符
		 *所以直接输一个glm::mat4就可以被反向隐式转换成TransformComponent
		 *这里是插入一个指定类型,返回插入的那个变量 */
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		//m_Registry.remove<>();

		if (m_Registry.all_of<TransformComponent>(entity))//视频里用的是has函数们现在改名了，作用就是检查是否存在
		{
			TransformComponent& trans = m_Registry.get<TransformComponent>(entity);		//字面义，获取这个组件
		}

		m_Registry.on_construct<TransformComponent>().connect<&TransformFunc>();

		auto view = m_Registry.view<TransformComponent>();//获取一个内部视图
		for(auto entity : view)		//根据视图遍历实体
		{
			//auto& trans = m_Registry.get<TransformComponent>(entity);
			auto& trans = view.get<TransformComponent>(entity);		//效率更高，view设计起来就是为了干这个的
		}
#endif			
	}

	Scene::~Scene()
	{}

	void Scene::OnUpdataEditor(TimeStep timestep, Editor_Camera Edi_Camera)
	{
		Renderer2D::SceneBegin(Edi_Camera);
		/*这参数表什么鬼，到时候查一下*/
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);//获取一个内部 组,同时满足拥有这两个组件的数组
		for (auto entity : group)		//根据 组 遍历实体
		{
			auto&& [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);	//C++17 新特性，因该是tuple
			//Renderer2D::DrawQuad(trans.GetTransform(), sprite);
			Renderer2D::DrawSprite(trans.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::SceneEnd();
	}

	void Scene::OnUpdataRuntime(TimeStep timestep)
	{
		//更新 脚本程序
		{
			//这个到时候还是要移动到OnScenePlay
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

		/*---------------------物理引擎-------------------------------------------------------------*/
		const int32_t velocityIterations = 6;	//有点难形容，可能理解为速度？调小了更容易穿过物体
		const int32_t positionIterations = 2;	//这个大概是粒子散度之类的

		m_PhysicsWorld->Step(timestep, velocityIterations, positionIterations);	//步进，可以理解为计算一次

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
		/*---------------------物理引擎-------------------------------------------------------------*/


		BaseCamera* main_camera = nullptr;
		TransformComponent* camera_trans = nullptr;
		{
			/*
			*这里用group会报错，因为下面的遍历依旧是group，用entt开发者的话来说就是
			*They are not. Groups have an initialization step that you don't want to pay all the times. 
			*Therefore, you undergo this cost once (when the pools are likely empty or almost empty) and that's it.
			*他建议一直使用view，直到出现内存分配问题，我觉得可以当作 栈分配(view) 和 堆分配(group) 的区别来理解
			auto group = m_Registry.group<TransformComponent, CameraComponent>();*/
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)		//根据 组 遍历实体
			{
				auto&& [trans, camera] = view.get<TransformComponent, CameraComponent>(entity);	//C++17 新特性，因该是tuple

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
			//开始场景BeginScene
			Renderer2D::SceneBegin( *main_camera, camera_trans->GetTransform());
															/*这参数表什么鬼，到时候查一下*/
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);//获取一个内部 组,同时满足拥有这两个组件的数组
			for (auto entity : group)		//根据 组 遍历实体
			{
				auto&& [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);	//C++17 新特性，因该是tuple
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

		//对长宽比不固定的相机进行长宽比重置
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.FixedAspectRatio)		//判断是否为长宽比不固定的相机
			{
				camera.Camera.OnResizeView(width, height);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////开始运行，创建物理系统//////////////////////////////////////////////////////////////////////////////////////////////////
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

				/*---------夹具，用来将各种物理属性绑定到一个body上-------------------------*/
				b2FixtureDef fixturedef;
				fixturedef.shape = &boxShape;
				fixturedef.density = bc2d.Density;
				fixturedef.friction = bc2d.friction;
				fixturedef.restitution = bc2d.Restitution;
				fixturedef.restitutionThreshold = bc2d.RestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixturedef);
				/*---------夹具，用来将各种物理属性绑定到一个body上-------------------------*/

				bc2d.RuntimeFixture = fixture;
			}
		}
	}
	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}
	///////开始运行，创建物理系统//////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////创建,复制，删除实体//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Entity Scene::CreateEntity(UUID id, const std::string& name)
	{
		//每个实体包含一个entt::entity,和Scene*
		Entity entity = { m_Registry.create(), this };//this就是返回使用该函数的实例的指针
		//每个实体默认有 变换组件 Tag UUID
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
		//每个实体包含一个entt::entity,和Scene*
		Entity entity = { m_Registry.create(), this };//this就是返回使用该函数的实例的指针
		//每个实体默认有 变换组件 Tag UUID
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
	Entity Scene::DuplicateEntity(Entity entity)		//void Scene::DuplicateEntity(Entity entity)	//Cherno写的，其实没差别
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
		m_Registry.destroy(entity);		//自己写的隐式转换
	}
	///////创建和删除实体//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	

	Ref<Scene> Scene::Copy(const Ref<Scene>& otherScene)
	{
		Ref<Scene> newScene = CreatRef<Scene>();

		newScene->m_ViewWidth = otherScene->m_ViewWidth;
		newScene->m_ViewHeight = otherScene->m_ViewHeight;

		auto& srcRegistry = otherScene->m_Registry;
		auto& dstRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		//收集原场景中各个实体的UUID，在目标场景中创建出那么多实体
		auto entView = srcRegistry.view<IDComponent>();
		for (auto& enti : entView)
		{
			UUID uuid = srcRegistry.get<IDComponent>(enti).ID;
			std::string name = srcRegistry.get<TagComponent>(enti).Tag;
			Entity newEntity= newScene->CreateEntityWithUUID(uuid, name);

			enttMap[uuid] = enti;
		}

		//复制组件，除了ID和Tag
		CopyComponent<TransformComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<CameraComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<NativeScriptComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<RigidBody2DComponent>(srcRegistry, dstRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(srcRegistry, dstRegistry, enttMap);

		return newScene;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////添加组件时的回调函数，所有组件都必须有，不然无法编译//////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////特殊手段，将模板文件放在了cpp文件，为不同的对象专门安排了对应的函数///////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*好像有问题......*/
	template<typename T>
	void Scene::OnComponentAdding(Entity entity, T& component)
	{
		//static_assert(false);		//这个有问题，一直触发
	}
	/*------相当于是上面那个模板函数的扩展形式-----------------------------------------------------------*/
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
	


	//辅助功能
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