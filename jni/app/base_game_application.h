/*
 * game_application.h
 *
 *  Created on: 19 дек. 2015 г.
 *      Author: Borisov
 */

#ifndef PEGAS_GAME_APPLICATION_H_
#define PEGAS_GAME_APPLICATION_H_

#include "interfaces.h"
#include "../system/timer.h"
#include "../gfx/gfx.h"
#include "event_system.h"
#include "processes.h"
#include "game_state_manager.h"

namespace pegas
{
	class BaseGameApplication : public IPlatformContext
	{
	public:
		BaseGameApplication();
		virtual ~BaseGameApplication();

	public:
		virtual bool init();
		virtual bool run();
		virtual void cleanup();
		void activate(bool bActive) { m_isActive = bActive; };

	public:
		//IPlatformContext
		virtual Gfx* getGFX() { return m_gfx.get(); }
		virtual Timer* getTimer() { return &m_timer; }
		virtual ProcessManager* getProcessManager() { return &m_processManager; }
		virtual EventManager* getEventManager() { return &m_eventManager; }
		virtual GameStateManager* getGameStateManager() { return &m_gameStateManager; }

		virtual void addKeyboardController(IKeyboardController* controller);
		virtual void removeKeyboardController(IKeyboardController* controller);
		virtual void addMouseController(IMouseController* controller);
		virtual void removeMouseController(IMouseController* controller);

	protected:
		EventManager m_eventManager;
		ProcessManager m_processManager;
		GameStateManager m_gameStateManager;
		SmartPointer<Gfx> m_gfx;
		Timer m_timer;
		bool m_isActive;

		std::list<IKeyboardController*> m_keyboardInputHandlers;
		std::list<IMouseController*> m_mouseInputHandlers;
	private:
		BaseGameApplication(const BaseGameApplication& other);
		BaseGameApplication& operator=(const BaseGameApplication& other);
	};
}

#endif /* GAME_APPLICATION_H_ */
