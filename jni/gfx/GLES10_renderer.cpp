#include "../common.h"
#include "gfx.h"

#include "../system/log.h"
#include "texture.h"
#include "atlas.h"

#define GLES_ON_ERROR(...) LOGE(__VA_ARGS__); \
							destroy(); \
							return STATUS_KO;

namespace pegas
{
	class GLES10Renderer: public Gfx, public Singleton<GLES10Renderer>
	{
	public:
		GLES10Renderer(android_app* application);

		virtual status create();
		virtual void   destroy();

		virtual int32_t getCanvasWidth() const;
		virtual int32_t getCanvasHeight() const;
		virtual void clearCanvas(float r = 0.0f, float g = 0.0f, float b = 0.0f);
		virtual void beginDraw();
    	virtual status endDraw();
    	virtual void render(const RenderQueueItem& item);
    	virtual Texture* createTexture(const std::string& path);
    	virtual Atlas* createAtlas(const std::string& path);

    	virtual void setWorldMatrix(const Matrix4x4& mat);
    	virtual void setViewMatrix(const Matrix4x4& mat);
    	virtual void setProjectionMatrix(const Matrix4x4& mat);

	private:
    	typedef std::vector<RenderQueueItem> RenderQueue;
    	typedef RenderQueue::iterator RenderQueueIt;

		void drawItem(RenderQueueItem* item, RenderQueueItem* prevItem = NULL);

		android_app* m_application;

		EGLDisplay m_display;
		EGLSurface m_surface;
		EGLContext m_context;

		int32_t m_canvasWidth;
		int32_t m_canvasHeight;

		Matrix4x4 m_world;
		Matrix4x4 m_view;

		RenderQueue m_renderQueue;

	private:
		GLES10Renderer(const GLES10Renderer& other);
		GLES10Renderer& operator=(const GLES10Renderer& other);
	};

	//-----------------------------------------------------------------------------------
	//	instantiation
	//-----------------------------------------------------------------------------------
#ifdef PEGAS_USE_GLES_1x
	Gfx* Gfx::createInstance(android_app* context)
	{

		Gfx* gfx = new GLES10Renderer(context);
		if(gfx->create() != STATUS_OK)
		{
			delete gfx;
			return NULL;
		}

		return gfx;

	}
#endif

	//===================================================================================
	//	GLESv1GraphService implementation
	//===================================================================================
		GLES10Renderer::GLES10Renderer(android_app* application)
			:Singleton(*this),
			 m_application(application),
			 m_display(EGL_NO_DISPLAY),
			 m_surface(EGL_NO_DISPLAY),
			 m_context(EGL_NO_DISPLAY),
			 m_canvasWidth(0),
			 m_canvasHeight(0)
		{
			LOGI("GLESv1GraphService constructor");

			m_world.identity();
			m_view.identity();
		}

		status GLES10Renderer::create()
		{
			LOGI("GLESv1GraphService::create");

			EGLint format;
			EGLint numConfigs;
			EGLint errorResult;
			EGLConfig config;

			const EGLint attributes[] = {
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
					EGL_BLUE_SIZE, 5,
					EGL_GREEN_SIZE, 6,
					EGL_RED_SIZE, 5,
					EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
					EGL_NONE
			};

			m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if(m_display == EGL_NO_DISPLAY)
			{
				GLES_ON_ERROR("m_display == EGL_NO_DISPLAY");
			}

			if(!eglInitialize(m_display, NULL, NULL))
			{
				GLES_ON_ERROR("!eglInitialize");
			}

			if(!eglChooseConfig(m_display, attributes, &config, 1, &numConfigs))
			{
				GLES_ON_ERROR("!eglChooseConfig");
			}

			if(numConfigs <= 0)
			{
				GLES_ON_ERROR("numConfigs <= 0");
			}

			if(!eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format))
			{
				GLES_ON_ERROR("!eglGetConfigAttrib");
			}

			ANativeWindow_setBuffersGeometry(m_application->window, 0, 0, format);

			m_surface = eglCreateWindowSurface(m_display, config, m_application->window, NULL);
			if(m_surface == EGL_NO_SURFACE)
			{
				GLES_ON_ERROR("m_surface == EGL_NO_SURFACE");
			}

			m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, NULL);
			if(m_context == EGL_NO_CONTEXT)
			{
				GLES_ON_ERROR("m_context == EGL_NO_CONTEXT");
			}

			if(!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
			{
				GLES_ON_ERROR("!eglMakeCurrent");
			}

			if(!eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_canvasWidth)
					|| !eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_canvasHeight))
			{
				GLES_ON_ERROR("!eglQuerySurface");

			}

			glViewport(0, 0, m_canvasWidth, m_canvasHeight);

			return STATUS_OK;
		}

		void  GLES10Renderer::destroy()
		{
			if(m_display != EGL_NO_DISPLAY)
			{
				eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

				if(m_context != EGL_NO_CONTEXT)
				{
					eglDestroyContext(m_display, m_context);
					m_context = EGL_NO_CONTEXT;
				}

				if(m_surface != EGL_NO_SURFACE)
				{
					eglDestroySurface(m_display, m_surface);
					m_surface = EGL_NO_SURFACE;
				}

				eglTerminate(m_display);
				m_display = EGL_NO_DISPLAY;
			}
		}

		void GLES10Renderer::clearCanvas(float r, float g, float b)
		{
			glEnable (GL_DEPTH_TEST);

			glClearColor(r, g, b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}

		void GLES10Renderer::beginDraw()
		{

		}

		status GLES10Renderer::endDraw()
		{
			RenderQueueItem* prev = NULL;
			RenderQueueItem* current = NULL;

			std::sort(m_renderQueue.begin(), m_renderQueue.end());
			for(RenderQueueIt it = m_renderQueue.begin(); it != m_renderQueue.end(); ++it)
			{
				current = &(*it);
				drawItem(current, prev);
				prev = current;
			}
			m_renderQueue.clear();

			if(eglSwapBuffers(m_display, m_surface) != EGL_TRUE)
			{
				LOGE("!eglSwapBuffers, error code: %d", eglGetError());
				return STATUS_KO;
			}

			return STATUS_OK;
		}

		void GLES10Renderer::render(const RenderQueueItem& item)
		{
			m_renderQueue.push_back(item);
		}

		int32_t GLES10Renderer::getCanvasWidth() const
		{
			return m_canvasWidth;
		}

		int32_t GLES10Renderer::getCanvasHeight() const
		{
			return m_canvasHeight;
		}

		void GLES10Renderer::drawItem(RenderQueueItem* item, RenderQueueItem* prevItem)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);
			item->m_texture->apply();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			static GLushort indices[] = { 0, 1, 3, 1, 2, 3 };

			glVertexPointer(3, GL_FLOAT, 0, item->m_vertices);
			glTexCoordPointer(2, GL_FLOAT, 0, item->m_textureCoords);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		Texture* GLES10Renderer::createTexture(const std::string& path)
		{
			return new Texture(m_application, path);
		}

		Atlas* GLES10Renderer::createAtlas(const std::string& path)
		{
			return new Atlas(m_application, path);
		}

		void GLES10Renderer::setWorldMatrix(const Matrix4x4& mat)
		{
			m_world = mat;

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)m_view);
			glMultMatrixf((GLfloat*)m_world);

		}

		void GLES10Renderer::setViewMatrix(const Matrix4x4& mat)
		{
			m_view = mat;

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)m_view);
			glMultMatrixf((GLfloat*)m_world);
		}

		void GLES10Renderer::setProjectionMatrix(const Matrix4x4& mat)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf((const GLfloat*)mat);
		}
}




