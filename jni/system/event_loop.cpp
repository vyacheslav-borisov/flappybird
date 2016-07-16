#include "../common.h"

#include "event_loop.h"
#include "log.h"

namespace pegas
{
		EventLoop::EventLoop(android_app* application)
			:m_application(application),
			 m_activityHandler(NULL), m_inputHandler(NULL),
			 m_enabled(false), m_quit(false)
		{
			m_application->onAppCmd = activityCallback;
			m_application->onInputEvent = inputCallback;
			m_application->userData = this;
		}

		void EventLoop::run(IActivityHandler* activityHandler, IInputHandler*  inputHandler)
		{
			int32_t result;
			int32_t events;
			android_poll_source* source;

			app_dummy();
			m_activityHandler = activityHandler;
			m_inputHandler = inputHandler;

			LOGI("starting event loop");

			while(true)
			{
				while((result = ALooper_pollAll(m_enabled ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
				{
					if(source != NULL)
					{
						LOGI("processing an event");
						source->process(m_application, source);
					}

					if(m_application->destroyRequested)
					{
						LOGI("exiting event loop");
						return;
					}
				}

				if(m_enabled && !m_quit)
				{
					if(m_activityHandler->onStep() != STATUS_OK)
					{
						m_quit = true;
						ANativeActivity_finish(m_application->activity);
					}
				}
			}//while(true)
		}//EventLoop::run()

		void EventLoop::activate()
		{
			if(!m_enabled && m_application->window != NULL)
			{
				m_quit = false;
				m_enabled = true;

				if(m_activityHandler->onActivate() != STATUS_OK)
				{
					m_quit = true;
					ANativeActivity_finish(m_application->activity);
				}
			}
		}

		void EventLoop::deactivate()
		{
			if(m_enabled)
			{
				m_activityHandler->onDeactivate();
				m_enabled = false;
			}
		}

		void EventLoop::processActivityEvent(int32_t command)
		{
			switch(command)
			{
			case APP_CMD_CONFIG_CHANGED:
				m_activityHandler->onConfigurationChanged();
				break;
			case APP_CMD_INIT_WINDOW:
				m_activityHandler->onCreateWindow();
				break;
			case APP_CMD_DESTROY:
				m_activityHandler->onDestroy();
				break;
			case APP_CMD_GAINED_FOCUS:
				activate();
				m_activityHandler->onGainFocus();
				break;
			case APP_CMD_LOST_FOCUS:
				m_activityHandler->onLostFocus();
				deactivate();
				break;
			case APP_CMD_LOW_MEMORY:
				m_activityHandler->onLowMemory();
				break;
			case APP_CMD_PAUSE:
				m_activityHandler->onPause();
				deactivate();
				break;
			case APP_CMD_RESUME:
				m_activityHandler->onResume();
				break;
			case APP_CMD_SAVE_STATE:
				m_activityHandler->onSaveState(&m_application->savedState, &m_application->savedStateSize);
				break;
			case APP_CMD_START:
				m_activityHandler->onStart();
				break;
			case APP_CMD_STOP:
				m_activityHandler->onStop();
				break;
			case APP_CMD_TERM_WINDOW:
				m_activityHandler->onDestroyWindow();
				deactivate();
				break;
			default:
				break;
			}
		}

		int32_t EventLoop::processInputEvent(AInputEvent* event)
		{
			int32_t eventType = AInputEvent_getType(event);
			int32_t eventSource = AInputEvent_getSource(event);

			switch(eventType)
			{
			case AINPUT_EVENT_TYPE_MOTION:
				return m_inputHandler->onTouchEvent(event);
			case AINPUT_EVENT_TYPE_KEY:
				return m_inputHandler->onKeyEvent(event);
			}

			return 0;
		}

		void EventLoop::processSensorEvent()
		{

		}

		void EventLoop::activityCallback(android_app* application, int32_t command)
		{
			EventLoop* eventLoop = (EventLoop*)application->userData;
			eventLoop->processActivityEvent(command);
		}

		int32_t EventLoop::inputCallback(android_app* application, AInputEvent* event)
		{
			EventLoop* eventLoop = (EventLoop*)application->userData;

			return eventLoop->processInputEvent(event);
		}
}


