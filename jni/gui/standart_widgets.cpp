#include "../common.h"
#include "standart_widgets.h"

#include "../app/common_events.h"

using namespace pegas;

//-------------------------------------------------------------------------------
//	ButtonWidget class implementation
//------------------------------------------------------------------------------
ButtonWidget::ButtonWidget(WidgetID id): Widget(id)
{
	m_x = 0.0;
	m_y = 0.0;
	m_width = 10.0;
	m_height = 10.0;
	m_currentState = k_buttonStateNormal;
	m_isVisible = true;
	m_isFocused = false;

	m_styles[k_buttonStateActive]._textColor = 0xff00ff00;
	m_styles[k_buttonStateActive]._borderColor = 0xff00ff00;
	m_styles[k_buttonStateDisabled]._textColor = 0xffaaaaaa;
	m_styles[k_buttonStateDisabled]._borderColor = 0xffaaaaaa;
}

void ButtonWidget::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
	
	//recalcTextPosition();
}

void ButtonWidget::setSize(float width, float height)
{
	m_width = width;
	m_height = height;

	//recalcTextPosition();
}

void ButtonWidget::setCaption(const String& caption)
{
	m_caption = caption;

	//recalcTextPosition();
}

/*
void ButtonWidget::recalcTextPosition()
{
	if(!m_caption.empty())
	{
		CURCOORD width, height;
		for(int32 i = k_buttonStateNormal; i != k_buttonStateTotal; i++)
		{
			GrafManager::getInstance().getTextExtent(m_caption, m_styles[i]._font, width, height);
			m_styles[i]._left =	m_x + (m_width - (float)width) * 0.5;
			m_styles[i]._top = m_y + m_height - ((m_height - (float)height) * 0.5);
		}
	}//if(m_caption != "")
}*/

void ButtonWidget::setVisible(bool visible)
{
	m_isVisible = visible;
}

void ButtonWidget::setButtonStyle(int32 state, 
								  RESOURCEID font, 
								  RGBCOLOR textColor, 
								  RGBCOLOR borderColor, 
								  RGBCOLOR fillColor)
{
	if(state >= 0 && state < k_buttonStateTotal)
	{
		m_styles[state]._font = font;
		m_styles[state]._textColor = textColor;
		m_styles[state]._borderColor = borderColor;
		m_styles[state]._fillColor = fillColor;
	}

	//recalcTextPosition();
}

void ButtonWidget::enable()
{
	m_currentState = k_buttonStateNormal;
}

void ButtonWidget::disable()
{
	m_currentState = k_buttonStateDisabled;
}

void ButtonWidget::render(IPlatformContext* context)
{
	if(!m_isVisible) return;

	//TODO: ButtonWidget::render
	/*GrafManager::getInstance().drawRectangle((CURCOORD)m_x, (CURCOORD)m_y, (CURCOORD)m_width, (CURCOORD)m_height,
		m_styles[m_currentState]._borderColor, m_styles[m_currentState]._fillColor);

	if(!m_caption.empty())
	{
		TextParameters params;
		params._font = m_styles[m_currentState]._font;
		params._color = m_styles[m_currentState]._textColor;
		params._left = (CURCOORD)m_styles[m_currentState]._left;
		params._top = (CURCOORD)m_styles[m_currentState]._top;

		GrafManager::getInstance().drawText(m_caption, params);
	}*/
}

void ButtonWidget::setFocus()
{
	m_isFocused = true;
	m_currentState = k_buttonStateActive;
}

void ButtonWidget::killFocus()
{
	m_isFocused = false;
	m_currentState = k_buttonStateNormal;
}

void ButtonWidget::onKeyDown(KeyCode key, KeyFlags flags)
{
	if(key == IKeyboardController::k_keyCodeENTER)
	{
		m_currentState = k_buttonStatePressed;

		EventPtr evt(new Event_GUI_ButtonClick(this));
		TheEventMgr.triggerEvent(evt);
	}
}

void ButtonWidget::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	if(button == k_mouseButtonLeft)
	{
		m_currentState = k_buttonStatePressed;

		EventPtr evt(new Event_GUI_ButtonClick(this));
		TheEventMgr.triggerEvent(evt);
	}
}

void ButtonWidget::onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags)
{
	if(button == k_mouseButtonLeft)
	{
		m_currentState = m_isFocused ? k_buttonStateActive : k_buttonStateNormal;
	}
}

void ButtonWidget::onMouseMove(float x, float y, MouseFlags flags)
{
	if(isPointIn(x, y))
	{
		m_currentState = k_buttonStateActive;

	}else if(!m_isFocused)
	{
		m_currentState = k_buttonStateNormal;
	}
}


bool ButtonWidget::isPointIn(float x, float y)
{
	if(x < m_x || x > (m_x + m_width))
		return false;

	if(y < m_y || y > (m_y + m_height))
		return false;
		
	return true;
}
