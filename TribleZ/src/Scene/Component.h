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
		//��������ʽת���õĲ�����
		operator const std::string& () const { return Tag; }
		operator std::string& () { return Tag; }
	};

	struct CameraComponent
	{
		 SceneCamera Camera;
		 /*�Ƿ�Ϊ��������*/
		 bool Primary = true;//�ڳ������л���ͬ���  ����һ���ǲ���Ҫ���ڳ��������ٱ�
		 bool FixedAspectRatio = false;	//�Ƿ�̶�����ȣ�һ����ǲ��̶��ģ������Ŵ��ڱ仯���仯��

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator const BaseCamera& () const { return Camera; }
		operator BaseCamera& () { return Camera; }
	};

	//Ӧ�þ���ʵ��wasd�ƶ����������ŵȵȹ���(�ű�)�����
	struct NativeScriptComponent
	{
		//ʵ��
		ScriptableEntity* Entity_Instence = nullptr;

		//ʵ���ʼ�������ٺ���
		//std::function<void()> InstentiateFunction;		//û���壬ֻ�Ǳ���һ��C++���ĺ���ָ������עһ�¼���ӡ��
		ScriptableEntity* (*InstentiateScript)();
		//����Ҳ�Ǻ���ָ�������д��
		//typedef ScriptableEntity* (*InstentScriptFunc)();
		//InstentScriptFunc InstentiateScript;
		// 
		//using InstentScriptFunc = ScriptableEntity*(*)() ;
		//InstentScriptFunc InstentiateScript;
		//std::function<void(NativeScriptComponent*)> DestroyInstenceFunction;		//û���壬ֻ�Ǳ���һ��C++���ĺ���ָ������עһ�¼���ӡ��
		void (*DestroyScript)(NativeScriptComponent*);

		/*		����ScriptableEntity����û���麯���Ļ�����Ҫ����������������ͨ����ֵlambda��ȷ���ű�����
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
				delete (NativeScriptCom->Entity_Instence);	//Ϊ��ɾ�����ƺ����ǻ���Ҫһ��Ĭ����������
				NativeScriptCom->Entity_Instence = nullptr; 
			};

			/*		����ScriptableEntity����û���麯���Ļ���Ҫ���������������ȷ��������Ҫ�Ľű�����
														//���ﲻת��˵����һ������
			OnCreateFunction = [](ScriptableEntity* instence) { ((T*)instence)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instence) { ((T*)instence)->OnDestroy(); };
			OnUpdataFunction = [](ScriptableEntity* instence, TimeStep timestep) { ((T*)instence)->OnUpdata(timestep); };
			*/
		}
		
	};
}