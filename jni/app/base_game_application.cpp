#include "../common.h"
#include "base_game_application.h"

#include "../system/log.h"


namespace pegas
{
	//----------------------------------------------------------------------------------------
	// constructors/destructors
	//----------------------------------------------------------------------------------------
	BaseGameApplication::BaseGameApplication()
		:m_gfx(NULL), m_isActive(false)
	{
		LOGI("GameApplication constructor");
	}

	BaseGameApplication::~BaseGameApplication()
	{
		LOGI("GameApplication destructor");
	}

	//----------------------------------------------------------------------------------------
	// Apllication Lifecicle
	//----------------------------------------------------------------------------------------
	bool BaseGameApplication::init()
	{
		LOGI("BaseGameApplication::init()");

		m_timer.reset();
		m_eventManager.init(&m_timer);
		m_processManager.init(&m_timer);
		m_gameStateManager.create(this);

		return true;
	}

	bool BaseGameApplication::run()
	{
		if(!m_isActive)
		{
			return false;
		}

		m_timer.update();

		static float prevTime = -1.0f;
		if(prevTime < 0.0f) prevTime = m_timer.now();
		float elapsed = m_timer.now() - prevTime;
		prevTime = m_timer.now();

		MILLISECONDS deltaTime = (MILLISECONDS)(elapsed * 1.0e3);
		/*if(deltaTime > 33)
		{
			deltaTime = 33;
		}*/

		m_eventManager.processEvents();
		m_processManager.updateProcesses(deltaTime);
		m_gameStateManager.update(this);

		return m_gameStateManager.isAboutToQuit();
	}

	void BaseGameApplication::cleanup()
	{
		LOGI("BaseGameApplication::cleanup()");

		m_gameStateManager.destroy(this);
		m_gfx->destroy();
	}

	void BaseGameApplication::addKeyboardController(IKeyboardController* controller)
	{
		LOGI("BaseGameApplication::addKeyboardController [controller: 0x%X]", controller);

		std::list<IKeyboardController*>::iterator found_it = m_keyboardInputHandlers.end();
		found_it = std::find(m_keyboardInputHandlers.begin(), m_keyboardInputHandlers.end(), controller);

		if(found_it == m_keyboardInputHandlers.end())
		{
			m_keyboardInputHandlers.push_back(controller);
		}
	}

	void BaseGameApplication::removeKeyboardController(IKeyboardController* controller)
	{
		LOGI("BaseGameApplication::removeKeyboardController [controller: 0x%X]", controller);

		std::list<IKeyboardController*>::iterator found_it = m_keyboardInputHandlers.end();
		found_it = std::find(m_keyboardInputHandlers.begin(), m_keyboardInputHandlers.end(), controller);

		if(found_it != m_keyboardInputHandlers.end())
		{
			m_keyboardInputHandlers.erase(found_it);
		}
	}

	void BaseGameApplication::addMouseController(IMouseController* controller)
	{
		LOGI("BaseGameApplication::addMouseController [controller: 0x%X]", controller);

		std::list<IMouseController*>::iterator found_it = m_mouseInputHandlers.end();
		found_it = std::find(m_mouseInputHandlers.begin(), m_mouseInputHandlers.end(), controller);

		if(found_it == m_mouseInputHandlers.end())
		{
			m_mouseInputHandlers.push_back(controller);
		}
	}

	void BaseGameApplication::removeMouseController(IMouseController* controller)
	{
		LOGI("BaseGameApplication::removeMouseController [controller: 0x%X]", controller);

		std::list<IMouseController*>::iterator found_it = m_mouseInputHandlers.end();
		found_it = std::find(m_mouseInputHandlers.begin(), m_mouseInputHandlers.end(), controller);

		if(found_it != m_mouseInputHandlers.end())
		{
			m_mouseInputHandlers.erase(found_it);
		}
	}
}



