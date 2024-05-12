#pragma once

#include "TribleZ_Core/Core.h"

//#include "TribleZ_core/Render/Camera.h"
#include "TribleZ_Core/OrthoGraphicCameraContraller.h"
#include "TribleZ_Core/Render/Editor_Camera.h"
#include "TribleZ_Core/Render/BaseCamera.h"
#include "TribleZ_Core/Render/Texture.h"
#include "TribleZ_Core/Render/SubTexture2D.h"

#include "Scene/Component.h"


namespace TribleZ
{

	class Renderer2D
	{
	private:

	public:
		struct Renderable_Camera
		{
			glm::mat4 Projection;
			glm::mat4 View;
		};

		static void Init();
		static void ShutDown();

		static void SceneBegin(const Editor_Camera& camera);
		static void SceneBegin(const OrthoGraphicCamera& camera);
		static void SceneBegin(const BaseCamera& camera, const glm::mat4& transform);	//�����������ı任����������
		static void SceneEnd();

		static void StartBatch();
		static void Flush();

		/*----------------------------��ȾͼԪ---------------------------------------------------------------------------------------------------------------------*/
		//��������
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f), int entityID = -1);
		//��չAPI
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));

		//��ת����Ŀ���Ҫ�������Ե������ĸ�API
		//rotationΪ�˺�glm::rotate()ͳһ����Ҫ���뻡�ȣ�����glm::radian����
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		
		//����ʵ��ID
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& SpriteRenderComponent, int entityID = -1);
		/*----------------------------��ȾͼԪ---------------------------------------------------------------------------------------------------------------------*/

		struct Statistics
		{
			uint32_t DrawCallCount = 0;
			uint32_t QuadCount = 0;

			uint32_t GetVertexCount() { return QuadCount * 4; }
			uint32_t GetIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
		static void ResetAndFlush();
	};
}