#include "../common.h"
#include "android_game_application.h"

#include "../system/log.h"

namespace pegas
{
	AndroidGameApplication::AndroidGameApplication(android_app* context)
		:m_androidAppContext(context)
	{

	}

	bool AndroidGameApplication::init()
	{
		LOGI("AndroidGameApplication::init");

		if(!BaseGameApplication::init())
		{
			return false;
		}

		m_gfx = Gfx::createInstance(m_androidAppContext);
		if(!m_gfx.IsValid())
		{
			return false;
		}

		return true;
	}

	//------------------------------------------------------------------------------------
	//IActivityHandler implementation
	//------------------------------------------------------------------------------------
	status AndroidGameApplication::onActivate()
	{
		LOGI("AndroidGameApplication::onActivate");

		if(!init())
		{
			return STATUS_KO;
		}

		return STATUS_OK;
	}

	void AndroidGameApplication::onDeactivate()
	{
		LOGI("AndroidGameApplication::onDeactivate");

		cleanup();
	}

	status AndroidGameApplication::onStep()
	{
		if(run())
		{
			return STATUS_KO;
		}

		return STATUS_OK;
	}

	void AndroidGameApplication::onStart()
	{
		LOGI("AndroidGameApplication::onStart");
	}

	void AndroidGameApplication::onResume()
	{
		LOGI("AndroidGameApplication::onResume");
	}

	void AndroidGameApplication::onPause()
	{
		LOGI("AndroidGameApplication::onPause");
	}

	void AndroidGameApplication::onStop()
	{
		LOGI("AndroidGameApplication::onStop");
	}

	void AndroidGameApplication::onDestroy()
	{
		LOGI("AndroidGameApplication::onDestroy");
	}

	void AndroidGameApplication::onSaveState(void** data, size_t* size)
	{
		LOGI("AndroidGameApplication::onSaveState");
	}

	void AndroidGameApplication::onConfigurationChanged()
	{
		LOGI("AndroidGameApplication::onConfigurationChanged");
	}

	void AndroidGameApplication::onLowMemory()
	{
		LOGI("AndroidGameApplication::onLowMemory");
	}

	void AndroidGameApplication::onCreateWindow()
	{
		LOGI("AndroidGameApplication::onCreateWindow");
	}

	void AndroidGameApplication::onDestroyWindow()
	{
		LOGI("AndroidGameApplication::onDestroyWindow");
	}

	void AndroidGameApplication::onGainFocus()
	{
		LOGI("AndroidGameApplication::onGainFocus");

		activate(true);
	}

	void AndroidGameApplication::onLostFocus()
	{
		LOGI("AndroidGameApplication::onLostFocus");

		activate(false);
	}

	//--------------------------------------------------------------------------------------
	// IInputHandler implementation
	//--------------------------------------------------------------------------------------
	bool AndroidGameApplication::onTouchEvent(AInputEvent* event)
	{
		int32 action = AMotionEvent_getAction(event);
		int32 state = AMotionEvent_getMetaState(event);
		int32 pointer_index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK);
		pointer_index = pointer_index >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

		float x = AMotionEvent_getX(event, pointer_index);
		float y = AMotionEvent_getY(event, pointer_index);

		MouseButton button = pointer_index;
		MouseFlags flags = 0;
		if(state & AMETA_ALT_ON == AMETA_ALT_ON)
		{
			flags = flags | k_keyFlagAlt;
		}

		if(state & AMETA_SHIFT_ON == AMETA_SHIFT_ON)
		{
			flags = flags | k_keyFlagShift;
		}

		switch(action)
		{
		case AMOTION_EVENT_ACTION_DOWN:
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			{
				if(action == AMOTION_EVENT_ACTION_DOWN)
				{
					LOGI("AMOTION_EVENT_ACTION_DOWN [pointer_index = %d, x = %.2f, y = %.2f, state = 0x%X]",
							pointer_index, x, y, state);
				}

				if(action == AMOTION_EVENT_ACTION_POINTER_DOWN)
				{
					LOGI("AMOTION_EVENT_ACTION_POINTER_DOWN [pointer_index = %d, x = %.2f, y = %.2f, state = 0x%X]",
							pointer_index, x, y, state);
				}

				for(std::list<IMouseController*>::iterator it = m_mouseInputHandlers.begin();
						it != m_mouseInputHandlers.end(); ++it)
				{
					(*it)->onMouseButtonDown(button, x, y, flags);
				}
			}
			break;
		case AMOTION_EVENT_ACTION_UP:
		case AMOTION_EVENT_ACTION_POINTER_UP:
			{
				if(action == AMOTION_EVENT_ACTION_UP)
				{
					LOGI("AMOTION_EVENT_ACTION_UP [pointer_index = %d, x = %.2f, y = %.2f, state = 0x%X]",
							pointer_index, x, y, state);
				}

				if(action == AMOTION_EVENT_ACTION_POINTER_UP)
				{
					LOGI("AMOTION_EVENT_ACTION_POINTER_UP [pointer_index = %d, x = %.2f, y = %.2f, state = 0x%X]",
							pointer_index, x, y, state);
				}

				for(std::list<IMouseController*>::iterator it = m_mouseInputHandlers.begin();
										it != m_mouseInputHandlers.end(); ++it)
				{
					(*it)->onMouseButtonUp(button, x, y, flags);
				}
			}
			break;
		case AMOTION_EVENT_ACTION_MOVE:
			{
				if(action == AMOTION_EVENT_ACTION_MOVE)
				{
					LOGI("AMOTION_EVENT_ACTION_MOVE [pointer_index = %d, x = %.2f, y = %.2f, state = 0x%X]",
											pointer_index, x, y, state);
				}

				for(std::list<IMouseController*>::iterator it = m_mouseInputHandlers.begin();
														it != m_mouseInputHandlers.end(); ++it)
				{
					(*it)->onMouseMove(x, y, flags);
				}
			}
			break;
		};

		return true;
	}

	bool AndroidGameApplication::onKeyEvent(AInputEvent* event)
	{
		int32 action = AKeyEvent_getAction(event);
		int32 flags = AKeyEvent_getFlags(event);
		int32 keyCode = AKeyEvent_getKeyCode(event);
		int32 scanCode = AKeyEvent_getScanCode(event);
		int32 state =  AKeyEvent_getMetaState(event);
		int32 repeatCount = AKeyEvent_getRepeatCount(event);

		LOGI("AndroidGameApplication::onKeyEvent");

		switch(action)
		{
		case AKEY_EVENT_ACTION_DOWN:
			{
				LOGI("AKEY_EVENT_ACTION_DOWN [keyCode = %d, scanCode = %d, repeatCount = %d, flags = 0x%X, state = 0x%X]",
						keyCode, scanCode, repeatCount, flags, state);
				for(std::list<IKeyboardController*>::iterator it = m_keyboardInputHandlers.begin();
						it != m_keyboardInputHandlers.end(); ++it)
				{
					(*it)->onKeyDown(keyCode, flags);
				}
			}
			break;
		case AKEY_EVENT_ACTION_UP:
			{
				LOGI("AKEY_EVENT_ACTION_DOWN [keyCode = %d, scanCode = %d, repeatCount = %d, flags = 0x%X, state = 0x%X]",
										keyCode, scanCode, repeatCount, flags, state);
				for(std::list<IKeyboardController*>::iterator it = m_keyboardInputHandlers.begin();
								it != m_keyboardInputHandlers.end(); ++it)
				{
					(*it)->onKeyUp(keyCode, flags);
				}
			}
			break;
		}

		return true;
	}
}




