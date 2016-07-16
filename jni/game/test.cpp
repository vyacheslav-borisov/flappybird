#include "../common.h"
#include "test.h"

#include "../app/game_state_manager.h"
#include "../system/log.h"

namespace pegas
{
	TestScreen::TestScreen()
		:BaseScreenLayer(_text("test"), 1, false),
		 m_texture(NULL),
		 m_sprite(NULL),
		 m_sceneNode(NULL),
		 m_context(NULL),
		 m_prevX(0.0f), m_prevY(0.0f)
	{

	}

	void TestScreen::create(IPlatformContext* context)
	{
		m_context = context;

		Gfx* gfx = context->getGFX();

		m_texture = gfx->createTexture("test.png");
		m_texture->load();

		m_sprite = new Sprite(m_texture);
		m_sprite->addFrame(Sprite::Frame());

		Matrix4x4 trans, scale, rotate;
		Matrix4x4 matView, matProj;

		rotate.identity();
		rotate.rotateZ(Math::PI * 0.25);

		bool useScreenCoords = true;
		bool useLH = true;
		if(useScreenCoords)
		{
			float canvasWidth = gfx->getCanvasWidth();
			float canvasHeight = gfx->getCanvasHeight();
			float viewCenterX = canvasWidth * 0.5f;
			float viewCenterY = canvasHeight * 0.5f;
			float spriteWidth = canvasWidth * 0.5f;
			float spriteHeight = canvasHeight * 0.5f;
			float offsetX = (canvasWidth - spriteWidth) * 0.5f;
			float offsetY = (canvasHeight- spriteHeight) * 0.5f;

			if(useLH)
			{
				matView.lookAt(Vector3(viewCenterX, viewCenterY, 1.0f),
						Vector3(viewCenterX, viewCenterY, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
				matProj.orthoLH(canvasWidth, canvasHeight, 0.5f, 100.0f);
			}else
			{
				matView.lookAt(Vector3(viewCenterX, viewCenterY, -1.0f),
							Vector3(viewCenterX, viewCenterY, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
				matProj.orthoRH(canvasWidth, canvasHeight, 0.5f, 100.0f);
			}

			gfx->setViewMatrix(matView);
			gfx->setProjectionMatrix(matProj);

			scale.identity();
			scale.scale(spriteWidth, spriteHeight, 1.0f);

			trans.identity();
			trans.translate(viewCenterX, viewCenterY, 0.0f);
		}
		else
		{
			m_sprite->setPivot(Sprite::k_pivotCenter);

			if(useLH)
			{
				matView.lookAt(Vector3(0, 0, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
				matProj.orthoLH(2.0f, 2.0f, 0.5f, 100.0f);
			}else
			{
				matView.lookAt(Vector3(0, 0, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
				matProj.orthoRH(2.0f, 2.0f, 0.5f, 100.0f);
			}

			gfx->setViewMatrix(matView);
			gfx->setProjectionMatrix(matProj);

			scale.identity();
			trans.identity();
		}

		m_world = scale * rotate * trans;
		gfx->setWorldMatrix(m_world);

		Matrix4x4 empty;
		empty.identity();
		m_sceneNode = new SpriteSceneNode(m_sprite);
		m_sceneNode->setTransfrom(empty);
	}

	void TestScreen::destroy(IPlatformContext* context)
	{
		delete m_sceneNode;
		delete m_sprite;
		delete m_texture;
	}

	void TestScreen::update(IPlatformContext* context)
	{

	}

	void TestScreen::render(IPlatformContext* context)
	{
		m_sceneNode->render(context->getGFX());
	}

	void TestScreen::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
	{
		m_prevX = x;
		m_prevY = y;

		Matrix4x4 matRotate;
		matRotate.identity();
		matRotate.rotateZ(-Math::PI * 0.05f);

		m_world = m_world * matRotate;
		m_context->getGFX()->setWorldMatrix(m_world);
	}

	void TestScreen::onMouseMove(float x, float y, MouseFlags flags)
	{
		float cx = x - m_prevX;
		float cy = y - m_prevY;

		Vector3 vMotion(cx, cy, 0.0f);
		float d = vMotion.length();

		if(d > 3.0f)
		{
			m_prevX = x;
			m_prevY = y;



			Matrix4x4 matMotion;
			matMotion.identity();
			matMotion.translate(cx, cy, 0.0f);

			m_world = m_world * matMotion;
			m_context->getGFX()->setWorldMatrix(m_world);
		}
	}

	void TestScreen::onKeyDown(KeyCode key, KeyFlags flags)
	{
		if(key == 4)
		{
			m_context->getGameStateManager()->shutdownGame();
		}
	}
}



