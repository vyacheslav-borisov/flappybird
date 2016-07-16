#include "../common.h"
#include "../system/includes.h"

#include "sprite.h"
#include "../core/includes.h"

namespace pegas
{
	//------------------------------------------------------------------------
	// Sprite class implementation
	//------------------------------------------------------------------------
	Sprite::Sprite(Texture* texture)
		:m_texture(texture), m_currentFrame(-1), m_pivot(k_pivotCenter)
	{

	}

	void Sprite::addFrame(const Frame& frame)
	{
		m_frames.push_back(frame);

		if(m_currentFrame == -1)
		{
			m_currentFrame = m_frames.size() - 1;
		}
	}

	void Sprite::removeFrames()
	{
		m_frames.clear();
	}

	void Sprite::setCurrentFrame(int32 index)
	{
		if(index >= 0 && index < m_frames.size())
		{
			m_currentFrame = index;
		}
	}

	Sprite::Frame* Sprite::getCurrentFrame()
	{
		if(m_currentFrame == -1 && m_frames.size() > 0)
		{
			m_currentFrame = 0;
		}

		if(m_currentFrame != -1)
		{
			return &m_frames[m_currentFrame];
		}

		return NULL;
	}

	void Sprite::setPivot(int32 pivot)
	{
		m_pivot = pivot;
	}

	int32 Sprite::getPivot()
	{
		return m_pivot;
	}

	float Sprite::width()
	{
		Frame* frame = getCurrentFrame();
		if(frame && m_texture)
		{
			return m_texture->getWidth() * frame->width();
		}

		return 0.0f;
	}

	float Sprite::height()
	{
		Frame* frame = getCurrentFrame();
		if(frame && m_texture)
		{
			return m_texture->getHeight() * frame->height();
		}

		return 0.0f;
	}

	//----------------------------------------------------------------------
	// 	SpriteSceneNode class implementation
	//----------------------------------------------------------------------
	SpriteSceneNode::SpriteSceneNode(Sprite* sprite, SceneNode* parentNode)
		:SceneNode(parentNode), m_sprite(sprite), m_recalcAABB(false)
	{

	}

	void SpriteSceneNode::setTransfrom(const Matrix4x4& transform)
	{
		m_recalcAABB = true;

		SceneNode::setTransfrom(transform);
	}

	Rect2D SpriteSceneNode::getBoundBox()
	{
		if(m_recalcAABB)
		{
			recalcAABB();
		}

		return m_cachedAABB;
	}

	void SpriteSceneNode::recalcAABB()
	{
		m_recalcAABB = false;

		Matrix4x4 world = getWorldTransfrom();
		float zIndex = getZIndex();

		if(m_sprite->getPivot() == Sprite::k_pivotLeftTop)
		{
			m_cachedPoints[k_pointTopLeft] = Vector3(0.0f, 0.0f, zIndex) * world;
			m_cachedPoints[k_pointTopRight] = Vector3(1.0f, 0.0f, zIndex) * world;
			m_cachedPoints[k_pointBottomRight] = Vector3(1.0f, 1.0f, zIndex) * world;
			m_cachedPoints[k_pointBottomLeft] = Vector3(0.0f, 1.0f, zIndex) * world;
		}else
		{
			m_cachedPoints[k_pointTopLeft] = Vector3(-0.5f, -0.5f, zIndex) * world;
			m_cachedPoints[k_pointTopRight] = Vector3(0.5f, -0.5f, zIndex) * world;
			m_cachedPoints[k_pointBottomRight] = Vector3(0.5f, 0.5f, zIndex) * world;
			m_cachedPoints[k_pointBottomLeft] = Vector3(-0.5f, 0.5f, zIndex) * world;
		}

		float maxX, minX, maxY, minY;
		maxX = minX = m_cachedPoints[0]._x;
		maxY = minY = m_cachedPoints[0]._y;

		for(int i = 0; i < k_totalPoints; i++)
		{
			if(maxX < m_cachedPoints[i]._x)
			{
				maxX = m_cachedPoints[i]._x;
			}

			if(minX > m_cachedPoints[i]._x)
			{
				minX = m_cachedPoints[i]._x;
			}

			if(maxY < m_cachedPoints[i]._y)
			{
				maxY = m_cachedPoints[i]._y;
			}

			if(minY > m_cachedPoints[i]._y)
			{
				minY = m_cachedPoints[i]._y;
			}
		}

		#ifdef PEGAS_USE_SCREEN_COORDS
			m_cachedAABB._topLeft._x = minX;
			m_cachedAABB._topLeft._y = minY;
			m_cachedAABB._bottomRight._x = maxX;
			m_cachedAABB._bottomRight._y = maxY;
		#else
			m_cachedAABB._topLeft._x = minX;
			m_cachedAABB._topLeft._y = maxY;
			m_cachedAABB._bottomRight._x = maxX;
			m_cachedAABB._bottomRight._y = minY;
		#endif
	}

	void SpriteSceneNode::render(Gfx* gfx)
	{
		if(m_recalcAABB)
		{
			recalcAABB();
		}

		SceneNode::render(gfx);

		RenderQueueItem item;
		item.m_texture = m_sprite->getTexture();

		item.m_vertices[0] = m_cachedPoints[k_pointTopLeft]._x;
		item.m_vertices[1] = m_cachedPoints[k_pointTopLeft]._y;
		item.m_vertices[2] = m_cachedPoints[k_pointTopLeft]._z;

		item.m_vertices[3] = m_cachedPoints[k_pointTopRight]._x;
		item.m_vertices[4] = m_cachedPoints[k_pointTopRight]._y;
		item.m_vertices[5] = m_cachedPoints[k_pointTopRight]._z;

		item.m_vertices[6] = m_cachedPoints[k_pointBottomRight]._x;
		item.m_vertices[7] = m_cachedPoints[k_pointBottomRight]._y;
		item.m_vertices[8] = m_cachedPoints[k_pointBottomRight]._z;

		item.m_vertices[9] = m_cachedPoints[k_pointBottomLeft]._x;
		item.m_vertices[10] = m_cachedPoints[k_pointBottomLeft]._y;
		item.m_vertices[11] = m_cachedPoints[k_pointBottomLeft]._z;

		Sprite::Frame* frame = m_sprite->getCurrentFrame();
		item.m_textureCoords[0] = frame->_fromX;
		item.m_textureCoords[1] = frame->_fromY;

		item.m_textureCoords[2] = frame->_toX;
		item.m_textureCoords[3] = frame->_fromY;

		item.m_textureCoords[4] = frame->_toX;
		item.m_textureCoords[5] = frame->_toY;

		item.m_textureCoords[6] = frame->_fromX;
		item.m_textureCoords[7] = frame->_toY;

		item.m_zIndex = getZIndex();

		gfx->render(item);
	}

	//---------------------------------------------------------------------------------------------------
	//	SpriteAnimation class implementation
	//---------------------------------------------------------------------------------------------------
	SpriteAnimation::SpriteAnimation(Sprite* sprite, int32 flags)
		:m_sprite(sprite),
		 m_flags(flags),
		 m_startFrame(0),
		 m_numFrames(0),
		 m_fps(32),
		 m_frameLifeTime(0),
		 m_ellapsedTime(0)
	{
		m_numFrames = sprite->getNumFrames();
		m_frameLifeTime = (MILLISECONDS)(1000.0 / m_fps);
	}

	SpriteAnimation::~SpriteAnimation()
	{

	}

	void SpriteAnimation::setNumFrames(int32 startFrame, int32 numFrames)
	{
		m_startFrame = startFrame;
		m_numFrames = numFrames;
	}

	void SpriteAnimation::setFPS(int32 fps)
	{
		m_fps = fps;
		m_frameLifeTime = (MILLISECONDS)(1000.0 / m_fps);
		m_ellapsedTime = 0;
	}

	void SpriteAnimation::setFlags(int32 flags)
	{
		m_flags = flags;
	}

	void SpriteAnimation::update(MILLISECONDS deltaTime)
	{
		int32 currentFrame = m_sprite->getCurrentFrameIndex();
		int32 lastFrame = m_startFrame + m_numFrames - 1;

		m_ellapsedTime+= deltaTime;
		if(m_ellapsedTime >= m_frameLifeTime)
		{
			m_ellapsedTime = 0;

			if(m_flags & k_modeBackward)
			{
				currentFrame--;
			}else
			{
				currentFrame++;
			}

			if(currentFrame > lastFrame)
			{
				if(m_flags & k_modeLooped)
				{
					currentFrame = m_startFrame;
				}else
				{
					terminate();
					return;
				}
			}

			if(currentFrame < m_startFrame)
			{
				if(m_flags & k_modeLooped)
				{
					currentFrame = lastFrame;
				}else
				{
					terminate();
					return;
				}
			}
		}

		if(currentFrame <= lastFrame && currentFrame >= m_startFrame)
		{
			m_sprite->setCurrentFrame(currentFrame);
		}
	}

	void SpriteAnimation::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_sprite->setCurrentFrame(m_startFrame);
		m_ellapsedTime = 0;
	}
}



