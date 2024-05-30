#pragma once
#include "TribleZ_Core/Core.h"
#include "string"

namespace TribleZ
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual bool operator==(const Texture& other) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual uint32_t  GetWidth() const = 0;
		virtual uint32_t  GetHeight() const = 0;
		virtual uint32_t  GetID() const = 0;
		virtual void SetData(void* data, unsigned int data_size) = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(unsigned int Width, unsigned int Height);
		static Ref<Texture2D> Create(const std::string& FilePath);
	};

	/*准备着，后面应该需要*/
	class Texture3D : public Texture
	{
	public:
		virtual ~Texture3D() = default;

		static Ref<Texture3D> Create(unsigned int Width, unsigned int Height);
		static Ref<Texture3D> Create(const std::string& FilePath);
	};
}