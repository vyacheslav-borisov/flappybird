#ifndef PEGAS_WAITING_H
#define PEGAS_WAITING_H
#pragma once

#include "interfaces.h"
#include "processes.h"

namespace pegas
{
	class Waiting: public Process, public IEventListener 
	{
	public:
		Waiting(float seconds, bool stopOnPause = true);
		virtual void update(MILLISECONDS deltaTime);
		virtual void terminate();

		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName() { return "Waiting"; }

		void addFinalEvent(EventPtr evt);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

	private:
		MILLISECONDS m_remainTime;
		std::list<EventPtr> m_events;
		bool m_stopOnPause;
	};

}

#endif
