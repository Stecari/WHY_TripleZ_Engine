#pragma once
#include "TribleZ_Core/Render/SceneCamera.h"
#include "TribleZ_Core/UUID.h"
#include "TribleZ_Core/Render/Texture.h"

//#include "Scene/ScriptableEntity.h"
//这里记录一个bug问题：递归包含,直接编译会报一个“TribleZ::ScriptableEntity::m_Entity”使用未定义的 class“TribleZ::Entity”的错
//由于Component.h里面包含了ScriptableEntity.h，ScriptableEntity.h里面又用到了Entity,但是Entity.h中又包含了Component.h
//所以最后这个Component.h复制进来的时候，ScriptableEntity写在了Entity的上面，就未定义了
//一种解决方法是不在Component.h中包含完整的ScriptableEntity.h
// 改用对ScriptableEntity的声明，Component.h中也就不存在对Entity的定义了
//这种解决方式是基于我们在这个文件里使用的是ScriptableEntity的指针，不然是不行的

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace TribleZ
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID id) : ID(id) {}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture = nullptr;
		float Tilingfactor = 1.0;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}

		operator const glm::vec4& () const { return Color; }
		operator glm::vec4& () { return Color; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translate)
			:Translation(translate) {}

		glm::mat4 GetTransform() const
		{
#ifdef OLD_CODE_MAKE_ROTATION
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0 })
							   * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0 }) 
							   * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0 });
#endif
			//实验性代码，利用欧拉角和四元数转换，不一定会快，可能只是看上去会干净一点
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			glm::mat4 transform = glm::translate(glm::mat4(1.0), Translation)
								* rotation
								* glm::scale(glm::mat4(1.0f), Scale);
			return transform;
		}


	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
		//做两个隐式转换用的操作符
		operator const std::string& () const { return Tag; }
		operator std::string& () { return Tag; }
	};

	struct CameraComponent
	{
		 SceneCamera Camera;
		 /*是否为你的主相机*/
		 bool Primary = true;//在场景中切换不同相机  考虑一下是不是要放在场景里面再变
		 bool FixedAspectRatio = false;	//是否固定长宽比，一般就是不固定的，是随着窗口变化而变化的

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator const BaseCamera& () const { return Camera; }
		operator BaseCamera& () { return Camera; }
	};

	//应该就是实现wasd移动，滚轮缩放等等功能(脚本)的组件
	class ScriptableEntity;
	struct NativeScriptComponent
	{
		//实体
		ScriptableEntity* Entity_Instence = nullptr;

		//实体初始化和销毁函数
		//std::function<void()> InstentiateFunction;		//没意义，只是保留一个C++风格的函数指针来备注一下加深印象
		ScriptableEntity* (*InstentiateScript)();
		//下面也是函数指针的两种写法
		//typedef ScriptableEntity* (*InstentScriptFunc)();
		//InstentScriptFunc InstentiateScript;
		// 
		//using InstentScriptFunc = ScriptableEntity*(*)() ;
		//InstentScriptFunc InstentiateScript;
		//std::function<void(NativeScriptComponent*)> DestroyInstenceFunction;		//没意义，只是保留一个C++风格的函数指针来备注一下加深印象
		void (*DestroyScript)(NativeScriptComponent*);

		/*		假如ScriptableEntity类中没有虚函数的话就需要这三个函数变量来通过赋值lambda来确定脚本函数
		std::function<void(ScriptableEntity* )> OnCreateFunction;
		std::function<void(ScriptableEntity* )> OnDestroyFunction;
		std::function<void(ScriptableEntity*, TimeStep)> OnUpdataFunction;
		*/
		
		template<typename T>
		void Bind()
		{
			InstentiateScript = []() 
			{ 
					return static_cast<ScriptableEntity*>(new T());
			};

			DestroyScript = [](NativeScriptComponent* NativeScriptCom) 
			{
				delete (NativeScriptCom->Entity_Instence);	//为了删掉它似乎我们还需要一个默认析构函数
				NativeScriptCom->Entity_Instence = nullptr; 
			};

			/*		假如ScriptableEntity类中没有虚函数的话就要做下面这个操作来确定我们需要的脚本函数
														//这里不转据说会有一点问题
			OnCreateFunction = [](ScriptableEntity* instence) { ((T*)instence)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instence) { ((T*)instence)->OnDestroy(); };
			OnUpdataFunction = [](ScriptableEntity* instence, TimeStep timestep) { ((T*)instence)->OnUpdata(timestep); };
			*/
		}
		
	};

	//刚体
	struct RigidBody2DComponent
	{						 /*静态*/	  /*动态*/ /*运动学*/
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType m_BodyType = BodyType::Static;

		bool FixRotation = false;

		void* RuntimeBody = nullptr;	//向前存储Box2Dbody的数据

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	//碰撞箱与其物理属性
	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };	//中心点位置
		glm::vec2 Size = { 0.5f, 0.5f };	//相对于中心点的大小，真实尺寸因该为1 * 1

		//以后可能会移动到像是“物理材质”这种东西里面
		float Density = 1.0;				//密度
		float friction = 0.5;				//摩擦
		float Restitution = 0.0;			//碰撞的能量反馈系数，1 - 能量损失系数
		float RestitutionThreshold = 0.5;	//能量反馈系数的最大值

		void* RuntimeFixture = nullptr;	//向前存储Box2DFixture夹具的数据

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};
}