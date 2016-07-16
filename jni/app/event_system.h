#pragma once

#include "../core/singleton.h"
#include "../core/smart_pointer.h"
#include "../system/timer.h"
#include "interfaces.h"

namespace pegas
{
	
		class EventManager: public Singleton<EventManager> 
		{
		public:
            static const MILLISECONDS kNoTimeLimit = -1;

			EventManager():Singleton(*this), m_currentEventQueue(0), m_stage(0), m_timer(0) {};
			virtual ~EventManager() {};

			void init(Timer* timer) { m_timer = timer; }
			void addEventListener(EventListenerPtr listener, const EventType& eventType);
			void removeEventListener(EventListenerPtr listener, const EventType& eventType);
			void removeEventListener(EventListenerPtr listener);

			void pushEventToQueye(EventPtr evt);
			void triggerEvent(EventPtr evt);
			
			void processEvents(MILLISECONDS timeLimit = kNoTimeLimit);

		private:

			typedef std::queue<EventPtr> EventQueue;
			typedef std::set<EventListenerPtr> EventListenerSet;
			typedef SmartPointer<EventListenerSet> EventListenerSetPtr;
			typedef std::map<EventType, EventListenerSetPtr> EventListenerMap;

			EventQueue m_eventQueues[2];
			int m_currentEventQueue;
			int m_stage;
			
			EventListenerMap m_listeners;
			Timer*			 m_timer;

		private:
			EventManager(const EventManager& src);
			EventManager& operator=(const EventManager& src);
		};

		#define TheEventMgr EventManager::getInstance()
	
}//namespace pegas
