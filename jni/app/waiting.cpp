#include "../common.h"
#include "waiting.h"

#include "event_system.h"
#include "common_events.h"

using namespace pegas;

//------------------------------------------------------------------------------------
//	Waiting class
//------------------------------------------------------------------------------------
Waiting::Waiting(float seconds, bool stopOnPause)
{
	assert(seconds > 0.0f && "invalid time fo waiting");
	m_remainTime = seconds * 1000.0f;
	m_stopOnPause = stopOnPause;
}

void Waiting::update(MILLISECONDS deltaTime)
{
	m_remainTime-= deltaTime;
	if(m_remainTime <= 0)
	{
		for(std::list<EventPtr>::iterator it = m_events.begin(); it != m_events.end(); ++it)
		{
			TheEventMgr.pushEventToQueye(*it);
		}
		terminate();
	}
}

void Waiting::start(ProcessHandle myHandle, ProcessManagerPtr owner)
{
	Process::start(myHandle, owner);

	if(m_stopOnPause)
	{
		TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
		TheEventMgr.addEventListener(this, Event_Game_Resume::k_type);
	}
}

void Waiting::terminate()
{
	Process::terminate();

	TheEventMgr.removeEventListener(this);
}

void Waiting::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_Game_Pause::k_type)
	{
		suspend();
	}

	if(evt->getType() == Event_Game_Resume::k_type)
	{
		resume();
	}
}

void Waiting::addFinalEvent(EventPtr evt)
{
	m_events.push_back(evt);
}

	
		
