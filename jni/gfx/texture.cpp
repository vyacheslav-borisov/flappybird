#include "../common.h"
#include "../system/includes.h"

#include "texture.h"

namespace pegas
{
	Texture::Texture(android_app* application, const std::string& path)
		:m_resource(application, path), m_textureId(0)
	{
		LOGI("Texture constructor");
	}

	Texture::~Texture()
	{
		LOGI("Texture destructor");

		unload();
	}

	status Texture::load()
	{
		LOGI("Texture::load");

		uint8_t* imageBuffer = loadImage();
		if(imageBuffer == NULL)
		{
			LOGE("imageBuffer == NULL");
			return STATUS_KO;
		}

		GLenum errorResult;

		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0,
				m_format, GL_UNSIGNED_BYTE, imageBuffer);
		delete[] imageBuffer;

		if(glGetError() != GL_NO_ERROR)
		{
			LOGE("Error loading texture into OpenGL.");
			unload();

			return STATUS_KO;
		}

		LOGI("texture succefully created, texture ID: %d", m_textureId);

		return STATUS_OK;
	}

	void Texture::unload()
	{
		LOGI("Texture::unload");

		if(m_textureId != 0)
		{
			glDeleteTextures(1, &m_textureId);
			m_textureId = 0;
		}

		m_width = 0;
		m_height = 0;
		m_format = 0;
	}

	void Texture::apply()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
	}

	uint8_t* Texture::loadImage()
	{
		LOGI("Texture::loadImage");

		png_byte header[8];
		png_structp pngStruct = NULL;
		png_infop pngInfo = NULL;
		png_byte* imageBuffer = NULL;
		png_bytep* pngRows = NULL;
		png_int_32 rowSize;
		bool transparency;

		if(m_resource.load() != STATUS_OK)
		{
			LOGE("m_resource.load() != STATUS_OK");
			goto ERROR;
		}

		if(m_resource.read(header, sizeof(png_byte) * 8) != STATUS_OK)
		{
			LOGE("m_resource.read(header, sizeof(header)) != STATUS_OK");
			goto ERROR;
		}

		if(png_sig_cmp(header, 0, 8) != 0)
		{
			LOGE("png_sig_cmp(header, 0, 0) != 0");
			goto ERROR;
		}

		pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(!pngStruct)
		{
			LOGE("!pngStruct");
			goto ERROR;
		}

		pngInfo = png_create_info_struct(pngStruct);
		if(!pngInfo)
		{
			LOGE("!pngInfo");
			goto ERROR;
		}

		png_set_read_fn(pngStruct, &m_resource, callback_read);

		if(setjmp(png_jmpbuf(pngStruct)))
		{
			LOGE("setjmp(png_jmpbuf(pngStruct))");
			goto ERROR;
		}

		png_set_sig_bytes(pngStruct, 8);
		png_read_info(pngStruct, pngInfo);

		png_int_32 depth, colorType;
		png_uint_32 width, height;

		png_get_IHDR(pngStruct, pngInfo, &width, &height, &depth, &colorType, NULL, NULL, NULL);

		m_width = width;
		m_height = height;

		transparency = false;
		if(png_get_valid(pngStruct, pngInfo, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngStruct);
			transparency = true;
		}

		if(depth < 8)
		{
			png_set_packing(pngStruct);

		}else if(depth < 16)
		{
			png_set_strip_16(pngStruct);
		}

		switch(colorType)
		{
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(pngStruct);
			m_format = transparency ? GL_RGBA : GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGB:
			m_format = transparency ? GL_RGBA : GL_RGB;
			break;
		case PNG_COLOR_TYPE_RGBA:
			m_format = GL_RGBA;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(pngStruct);
			m_format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			break;
		case PNG_COLOR_TYPE_GA:
			png_set_expand_gray_1_2_4_to_8(pngStruct);
			m_format = GL_LUMINANCE_ALPHA;
			break;
		}

		png_read_update_info(pngStruct, pngInfo);



		rowSize = png_get_rowbytes(pngStruct, pngInfo);
		if(rowSize < 0)
		{
			LOGE("rowSize < 0");
			goto ERROR;
		}

		imageBuffer = new png_byte[rowSize * height];
		if(!imageBuffer)
		{
			LOGE("!imageBuffer");
			goto ERROR;
		}

		pngRows = new png_bytep[height];
		if(!pngRows)
		{
			LOGE("!pngRows");
			goto ERROR;
		}

		for(int32_t i = 0; i < height; i++)
		{
			pngRows[height - (i + 1)] = imageBuffer + i * rowSize;
		}

		png_read_image(pngStruct, pngRows);

		m_resource.unload();
		png_destroy_read_struct(&pngStruct, &pngInfo, NULL);

		delete[] pngRows;

		LOGI("PNG texture %s succefully loaded", m_resource.getPath().c_str());

		return imageBuffer;

	ERROR:
		LOGE("Error while reading PNG file");
		m_resource.unload();

		if(pngRows) delete[] pngRows;
		if(imageBuffer) delete[] imageBuffer;

		if(pngStruct != NULL)
		{
			png_infop* infoPtrP = pngInfo != NULL ? &pngInfo : NULL;
			png_destroy_read_struct(&pngStruct, infoPtrP, NULL);
		}

		return NULL;
	}

	void Texture::callback_read(png_structp pngStruct, png_bytep data, png_size_t size)
	{
		Resource* resource = (Resource*)png_get_io_ptr(pngStruct);
		if(resource->read(data, size) != STATUS_OK)
		{
			resource->unload();
			png_error(pngStruct, "Error while reading PNG file");
		}
	}
}


