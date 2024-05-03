#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "TribleZ_Core/Render/SceneCamera.h"
#include "Scene/ScriptableEntity.h"

namespace TribleZ
{
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
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
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0 })
							   * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0 }) 
							   * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0 });

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
}