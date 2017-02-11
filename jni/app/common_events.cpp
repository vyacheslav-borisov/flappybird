#include "../common.h"
#include "common_events.h"

namespace pegas
{
	const EventType Event_GUI_ButtonClick::k_type = "Event_GUI_ButtonClick";
	const EventType Event_GUI_StartFadein::k_type = "Event_GUI_StartFadein";
	const EventType Event_GUI_StartFadeout::k_type = "Event_GUI_StartFadeout";
	const EventType Event_GUI_FadeinComplete::k_type = "Event_GUI_FadeinComplete";
	const EventType Event_GUI_FadeoutComplete::k_type = "Event_GUI_FadeoutComplete";
	const EventType Event_GUI_FadeOn::k_type = "Event_GUI_FadeOn";
	const EventType Event_GUI_FadeOff::k_type = "Event_GUI_FadeOff";
	
	const EventType Event_Game_ChangeState::k_type = "Event_Game_ChangeState";
	const EventType Event_Game_ForwardToState::k_type = "Event_Game_ForwardToState";
	const EventType Event_Game_BackwardToPreviousState::k_type = "Event_Game_BackwardToPreviousState";

	const EventType Event_Game_Pause::k_type = "Event_Game_Pause";
	const EventType Event_Game_Resume::k_type = "Event_Game_Resume";
	const EventType Event_Game_Start::k_type = "Event_Game_Start";
	const EventType Event_Game_Restart::k_type = "Event_Game_Restart";
	const EventType Event_Game_Stop::k_type = "Event_Game_Stop";
}
