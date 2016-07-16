#ifndef PEGAS_TEXTURE_H_
#define PEGAS_TEXTURE_H_

#include "../core/smart_pointer.h"
#include "../system/resource.h"

namespace pegas
{
	class Texture
	{
	public:
		Texture(android_app* application, const std::string& path);
		virtual ~Texture();

		int32_t getHeight() const { return m_width; }
		int32_t getWidth() const { return m_height; }

		status load();
		void unload();
		void apply();

	protected:
		uint8_t* loadImage();

	private:
		static void callback_read(png_structp pngStruct, png_bytep data, png_size_t size);

		AssetResource m_resource;
		GLuint m_textureId;
		GLint m_format;
		int32_t m_width;
		int32_t m_height;
	};

	typedef SmartPointer<Texture> TexturePtr;
}

#endif /* TEXTURE_H_ */
