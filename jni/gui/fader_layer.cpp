#include "../common.h"
#include "fader_layer.h"

using namespace pegas;

FaderLayer::FaderLayer(const LayerId& id)
	:BaseScreenLayer(_text("fader"), id, true)
{
	m_color = 0;
	m_fadeLength = m_ellapsedTime = 0;
	m_fadein = false;
	m_width = m_height = 0;
}

void FaderLayer::create(IPlatformContext* context)
{
	TheEventMgr.addEventListener(this, Event_GUI_StartFadein::k_type);
	TheEventMgr.addEventListener(this, Event_GUI_StartFadeout::k_type);
	TheEventMgr.addEventListener(this, Event_GUI_FadeOn::k_type);
	TheEventMgr.addEventListener(this, Event_GUI_FadeOff::k_type);

	m_width = context->getGFX()->getCanvasWidth();
	m_height = context->getGFX()->getCanvasHeight();
}

void FaderLayer::destroy(IPlatformContext* context)
{
	TheEventMgr.removeEventListener(this); 
}

void FaderLayer::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{
	if(m_fadeLength <= 0) return;

	int32 alpha = (int32)(255.0f * (m_ellapsedTime * 1.0f) / m_fadeLength);
	
	if(alpha > 255) alpha = 255;

	if(!m_fadein)
	{
		alpha = 255 - alpha;
	}

	m_color = alpha << 24;

	m_ellapsedTime+= deltaTime;
	if(m_ellapsedTime >= m_fadeLength)
	{
		m_fadeLength = -1;
		if(m_fadein)
		{
			EventPtr evt(new Event_GUI_FadeinComplete());
			TheEventMgr.pushEventToQueye(evt);
		}else
		{
			setActivity(false);
			EventPtr evt(new Event_GUI_FadeoutComplete());
			TheEventMgr.pushEventToQueye(evt);
		}
	}
}

void FaderLayer::render(IPlatformContext* context)
{
	//TODO: FaderLayer::render
	//GrafManager::getInstance().drawRectangle(0, 0, m_width, m_height, m_color, m_color);
}

void FaderLayer::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_GUI_StartFadein::k_type)
	{
		Event_GUI_StartFadein* pEvent = evt->cast<Event_GUI_StartFadein>();
		m_fadeLength = (MILLISECONDS)(pEvent->_lengthInSeconds * 1000.0);
		m_ellapsedTime = 0;
		m_fadein = true;

		setActivity(true);
	}

	if(evt->getType() == Event_GUI_StartFadeout::k_type)
	{
		Event_GUI_StartFadeout* pEvent = evt->cast<Event_GUI_StartFadeout>();
		m_fadeLength = (MILLISECONDS)(pEvent->_lengthInSeconds * 1000.0);
		m_ellapsedTime = 0;
		m_fadein = false;

		setActivity(true);
	}

	if(evt->getType() == Event_GUI_FadeOn::k_type)
	{
		Event_GUI_FadeOn* pEvent = evt->cast<Event_GUI_FadeOn>();
		assert(pEvent->_fadePercent >= 0 && "Event_GUI_FadeOn invalid argument: percent");
		assert(pEvent->_fadePercent <= 100 && "Event_GUI_FadeOn invalid argument: percent");

		int32 alpha = (int32)((pEvent->_fadePercent * 255.0f) / 100.0f);
		m_color = alpha << 24;

		setActivity(true);
	}

	if(evt->getType() == Event_GUI_FadeOff::k_type)
	{
		m_color = 0;
		setActivity(false);
	}
}
