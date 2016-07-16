#ifndef PEGAS_SPRITE_H_
#define PEGAS_SPRITE_H_

#include "scene.h"
#include "texture.h"
#include "gfx.h"

#include "../core/smart_pointer.h"
#include "../app/processes.h"

namespace pegas
{
	class Sprite
	{
	public:
		struct Frame
		{
			Frame()
			{
				_fromX = _toY = 0.0f;
				_fromY = _toX = 1.0f;
			}

			float width() { return std::abs(_toX - _fromX); }
			float height() { return std::abs(_toY - _fromY); }

			float _fromX;
			float _fromY;
			float _toX;
			float _toY;
		};

		enum Pivot
		{
			k_pivotCenter = 0,
			k_pivotLeftTop
		};

	public:
		Sprite(Texture* texture);

		void addFrame(const Frame& frame);
		void removeFrames();

		void setCurrentFrame(int32 index);
		Frame* getCurrentFrame();
		int32 getCurrentFrameIndex() const { return m_currentFrame; }
		int32 getNumFrames() const { return m_frames.size(); }

		void setPivot(int32 pivot);
		int32 getPivot();

		float width();
		float height();

		Texture* getTexture() { return m_texture; }
	protected:
		typedef std::vector<Frame> FrameList;

		Texture* m_texture;
		FrameList m_frames;
		int32  m_currentFrame;
		int32  m_pivot;
	};

	class SpriteSceneNode: public SceneNode
	{
	public:
		SpriteSceneNode(Sprite* sprite, SceneNode* parentNode = NULL);

		virtual void setTransfrom(const Matrix4x4& transform);
		virtual Rect2D getBoundBox();
		virtual void render(Gfx* gfx);
	private:
		void recalcAABB();

		enum
		{
			k_pointTopLeft,
			k_pointTopRight,
			k_pointBottomRight,
			k_pointBottomLeft,
			k_totalPoints
		};

		Sprite* m_sprite;
		Vector3 m_cachedPoints[4];
		Rect2D  m_cachedAABB;
		bool 	m_recalcAABB;
	};

	class SpriteAnimation: public Process
	{
	public:
		enum Mode
		{
			k_modeLooped = 1,
			k_modeBackward
		};

	public:
		SpriteAnimation(Sprite* sprite, int32 flags = k_modeLooped);
		~SpriteAnimation();

		void setNumFrames(int32 startFrame, int32 numFrames);
		void setFPS(int32 fps);
		void setFlags(int32 flags);

		virtual void update(MILLISECONDS deltaTime);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

	private:
		Sprite* m_sprite;
		int32 m_startFrame;
		int32 m_numFrames;
		int32 m_fps;
		int32 m_flags;
		MILLISECONDS m_frameLifeTime;
		MILLISECONDS m_ellapsedTime;
	};

	typedef SmartPointer<Sprite> SpritePtr;
	typedef SmartPointer<SpriteAnimation> SpriteAnimationPtr;
}

#endif /* SPRITE_H_ */
