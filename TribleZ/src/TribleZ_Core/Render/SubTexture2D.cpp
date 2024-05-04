#include "tzpch.h"

#include "SubTexture2D.h"

namespace TribleZ
{													/*这里我们拿到坐标只是为了将他赋值给我们的成员，并没有必要留下他或者复制一个的必要，用&就可以了*/
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& bottom_left, const glm::vec2& upper_right)
		:m_Texture2D(texture)
	{
		m_TextureCoords[0] = { bottom_left.x, bottom_left.y };
		m_TextureCoords[1] = { upper_right.x, bottom_left.y };
		m_TextureCoords[2] = { upper_right.x, upper_right.y };
		m_TextureCoords[3] = { bottom_left.x, upper_right.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& spriteSize, const glm::vec2& texture_size)
	{
		glm::vec2 bottom_left = { (coord.x * spriteSize.x) / texture->GetWidth(),  (coord.y * spriteSize.y) / texture->GetHeight() };
		glm::vec2 upper_right = { ((coord.x + texture_size.x) * spriteSize.x) / texture->GetWidth(), ((coord.y + texture_size.y) * spriteSize.y) / texture->GetHeight() };

		return CreatRef<SubTexture2D>(texture, bottom_left, upper_right);
	}

}