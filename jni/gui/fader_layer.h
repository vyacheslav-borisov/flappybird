#ifndef PEGAS_GUI_FADER_LAYER_H
#define PEGAS_GUI_FADER_LAYER_H
#pragma once

#include "../app/includes.h"

namespace pegas
{
	class FaderLayer: public BaseScreenLayer, public IEventListener
	{
	public:
		FaderLayer(const LayerId& id);

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit);
		virtual void render(IPlatformContext* context);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName() { return "FadeLayer"; }

	private:
		RGBCOLOR		m_color;
		MILLISECONDS	m_fadeLength;
		MILLISECONDS	m_ellapsedTime;
		CURCOORD		m_width;
		CURCOORD		m_height;
		bool			m_fadein;	
	};

	template <class StartEvent, class ComleteEvent>
	class FaderProcess: public Process, public IEventListener
	{
	public:
		FaderProcess(float timeInSeconds = 1.0)
		{
			m_time = timeInSeconds;
			TheEventMgr.addEventListener(this, ComleteEvent::k_type);
		}

		virtual ~FaderProcess()
		{
			TheEventMgr.removeEventListener(this);
		}

		virtual void handleEvent(EventPtr evt)
		{
			if(evt->getType() == ComleteEvent::k_type)
			{
				terminate();
			}
		}

		virtual ListenerType getListenerName() { return "FaderProcess"; }

		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner)
		{
			Process::start(myHandle, owner);
			TheEventMgr.triggerEvent(EventPtr(new StartEvent(m_time)));
		}

		virtual void update(MILLISECONDS deltaTime) {};

	private:
		float m_time;
	};

	typedef FaderProcess<Event_GUI_StartFadein, Event_GUI_FadeinComplete> Fadein;
	typedef FaderProcess<Event_GUI_StartFadeout, Event_GUI_FadeoutComplete> Fadeot;

	class ChangeStateTask: public Process
	{
	public:
		ChangeStateTask(IPlatformContext* platform, GameStateID	newStateId)
		{
			m_platform = platform;
			m_newStateId = newStateId;
		}

		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner)
		{
			Process::start(myHandle, owner);
			m_platform->getGameStateManager()->changeState(m_newStateId);
		}

		virtual void update(MILLISECONDS deltaTime) {}

	private:
		IPlatformContext* m_platform;
		GameStateID	m_newStateId;
	};
}

#endif
