#pragma once

#include "TribleZ_Core/Core.h"

//#include "TribleZ_core/Render/Camera.h"
#include "TribleZ_Core/OrthoGraphicCameraContraller.h"
#include "TribleZ_Core/Render/BaseCamera.h"
#include "TribleZ_Core/Render/Texture.h"
#include "TribleZ_Core/Render/SubTexture2D.h"


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

		static void SceneBegin(const OrthoGraphicCamera& camera);
		static void SceneBegin(const BaseCamera& camera, const glm::mat4& transform);	//用相机和相机的变换来创建场景
		static void SceneEnd();
		static void Flush();

		/*----------------------------渲染图元---------------------------------------------------------------------------------------------------------------------*/
		//基本操作
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		//扩展API
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));

		//旋转矩阵的开销要更大，所以单独做四个API
		//rotation为了和glm::rotate()统一所以要输入弧度，就是glm::radian过的
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor = 1.0f, const glm::vec4& tintcolor = glm::vec4(1.0f));
		/*----------------------------渲染图元---------------------------------------------------------------------------------------------------------------------*/

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