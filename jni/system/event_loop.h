#ifndef PEGAS_EVENT_LOOP_H_
#define PEGAS_EVENT_LOOP_H_

#include "interfaces.h"

namespace pegas
{
	class EventLoop
	{
	public:
		EventLoop(android_app* application);

		void run(IActivityHandler* activityHandler, IInputHandler*  inputHandler);

	protected:
		void activate();
		void deactivate();

		void processActivityEvent(int32_t command);
		int32_t processInputEvent(AInputEvent* event);
		void processSensorEvent();
	private:
		static void activityCallback(android_app* application, int32_t command);
		static int32_t inputCallback(android_app* application, AInputEvent* event);

	private:
		IActivityHandler* m_activityHandler;
		IInputHandler*  m_inputHandler;

		android_app* m_application;
		bool m_enabled;
		bool m_quit;
	};
}

#endif /* EVENT_LOOP_H_ */
