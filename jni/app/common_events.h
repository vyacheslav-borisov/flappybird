#ifndef PEGAS_COMMON_EVENTS_H
#define PEGAS_COMMON_EVENTS_H
#pragma once

#include "interfaces.h"
#include "../system/log.h"

namespace pegas
{
	//-----------------------------------------------------------------------------------------
	//	GUI events;
	//-----------------------------------------------------------------------------------------
	class Widget;
	struct Event_GUI_ButtonClick: public Event
	{
	public:
		Event_GUI_ButtonClick(Widget* button): m_button(button) {}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		Widget* m_button;
	};

	//--------------------------------------------------------------------------------------
	//	Fader events;
	//--------------------------------------------------------------------------------------
	struct Event_GUI_StartFadein: public Event
	{
	public:
		Event_GUI_StartFadein(float lengthInSeconds = 1.0f): 
		  _lengthInSeconds(lengthInSeconds) 
		  {
			  LOGI("Event_GUI_StartFadein [time = %.4f]", lengthInSeconds);
		  }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		float _lengthInSeconds;
	};

	struct Event_GUI_StartFadeout: public Event
	{
	public:
		Event_GUI_StartFadeout(float lengthInSeconds = 1.0f): 
		  _lengthInSeconds(lengthInSeconds)  
		  {
			  LOGI("Event_GUI_StartFadeout [time = %.4f]", lengthInSeconds);
		  }

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		float _lengthInSeconds;
	};

	struct Event_GUI_FadeOn: public Event
	{
	public:
		Event_GUI_FadeOn(int32 fadePercent): _fadePercent(fadePercent) 
		{
			LOGI("Event_GUI_FadeOn [percent = %d]", fadePercent);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		int32 _fadePercent;
	};

	struct Event_GUI_FadeOff: public Event
	{
	public:
		Event_GUI_FadeOff() 
		{
			LOGI("Event_GUI_FadeOff");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_GUI_FadeinComplete: public Event
	{
	public:
		Event_GUI_FadeinComplete()  
		{
			LOGI("Event_GUI_FadeinComplete");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_GUI_FadeoutComplete: public Event
	{
	public:
		Event_GUI_FadeoutComplete()  
		{
			LOGI("Event_GUI_FadeoutComplete");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	/*************************************************************************************************************
		Global Game events
	**************************************************************************************************************/
	struct Event_Game_ChangeState: public Event
	{
	public:
		Event_Game_ChangeState(const GameStateID& id): _id(id)  
		{
			LOGI("Event_Game_ChangeState [id = %d]", id);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		GameStateID _id;
	};

	struct Event_Game_ForwardToState: public Event
	{
	public:
		Event_Game_ForwardToState(const GameStateID& id): _id(id)  
		{
			LOGI("Event_Game_ForwardToState [id = %d]", id);
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		GameStateID _id;
	};

	struct Event_Game_BackwardToPreviousState: public Event
	{
	public:
		Event_Game_BackwardToPreviousState() 
		{
			LOGI("Event_Game_BackwardToPreviousState");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_Game_Pause: public Event
	{
	public:
		Event_Game_Pause()  
		{
			LOGI("Event_Game_Pause");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_Game_Resume: public Event
	{
	public:
		Event_Game_Resume()  
		{
			LOGI("Event_Game_Resume");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;		
	};

	struct Event_Game_Start: public Event
	{
	public:
		Event_Game_Start()
		{
			LOGI("Event_Game_Start");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;
	};

	struct Event_Game_Restart: public Event
	{
	public:
		Event_Game_Restart()
		{
			LOGI("Event_Game_Restart");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;
	};

	struct Event_Game_Stop: public Event
	{
	public:
		Event_Game_Stop()
		{
			LOGI("Event_Game_Stop");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;
	};
}

#endif
