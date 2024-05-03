#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace TribleZ
{
	class SubTexture2D
	{
	private:
		Ref<Texture2D> m_Texture2D;
		glm::vec2 m_TextureCoords[4];
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& bottom_left, const glm::vec2& Upper_right);

		const Ref<Texture2D> GetTextureSheet() const { return m_Texture2D; }
		const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& spriteSize, const glm::vec2& texture_size = { 1.0f, 1.0f });

	};
}