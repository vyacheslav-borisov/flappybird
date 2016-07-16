/*
 * test.h
 *
 *  Created on: 12 џэт. 2016 у.
 *      Author: Borisov
 */

#ifndef GAME_TEST_H_
#define GAME_TEST_H_

#include "../app/default_game_state.h"
#include "../core/includes.h"
#include "../gfx/includes.h"

namespace pegas
{
	class TestScreen : public BaseScreenLayer
	{
	public:
		TestScreen();
		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);;
		virtual void update(IPlatformContext* context);
		virtual void render(IPlatformContext* context);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);

	private:
		IPlatformContext* m_context;
		Texture* m_texture;
		Sprite* m_sprite;
		SpriteSceneNode* m_sceneNode;
		Matrix4x4 m_world;

		float m_prevX;
		float m_prevY;
	};
}

#endif /* GAME_TEST_H_ */
