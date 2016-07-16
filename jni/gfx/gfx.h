#ifndef PEGAS_GFX_H_
#define PEGAS_GFX_H_

#include "../core/includes.h"

namespace pegas
{
	class Texture;
	class Atlas;

	struct RenderQueueItem
    {
    public:
    	Texture* m_texture;
    	GLfloat   m_vertices[12];
    	GLfloat   m_textureCoords[8];
    	float	  m_zIndex;

    	bool operator<(const RenderQueueItem& other) const
    	{
    		if(m_zIndex <= other.m_zIndex)
    		{
    			return true;
    		}

    		return false;
    	}
    };

    class Gfx
	{
	public:
		virtual ~Gfx() { }

		virtual status create() = 0;
		virtual void   destroy() = 0;

    	virtual int32_t getCanvasWidth() const = 0;
    	virtual int32_t getCanvasHeight() const = 0;
    	virtual void clearCanvas(float r = 0.0f, float g = 0.0f, float b = 0.0f) = 0;
    	virtual void beginDraw() = 0;
    	virtual status endDraw() = 0;
    	virtual void render(const RenderQueueItem& item) = 0;
    	virtual Texture* createTexture(const std::string& path) = 0;
    	virtual Atlas* createAtlas(const std::string& path) = 0;

    	virtual void setWorldMatrix(const Matrix4x4& mat) = 0;
    	virtual void setViewMatrix(const Matrix4x4& mat) = 0;
    	virtual void setProjectionMatrix(const Matrix4x4& mat) = 0;

	public:
#ifdef _WIN32
    	static Gfx* createInstance(HWND context);
#endif
#ifdef ANDROID
    	static Gfx* createInstance(android_app* context);
#endif
	};
}

#endif
