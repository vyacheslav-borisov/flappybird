#ifndef PEGAS_GUI_STANDART_WIDGETS_H
#define PEGAS_GUI_STANDART_WIDGETS_H
#pragma once

#include "gui_layer.h"

namespace pegas
{
	enum ButtonStates
	{
		k_buttonStateNormal = 0,
		k_buttonStateActive,
		k_buttonStatePressed,
		k_buttonStateDisabled,
		k_buttonStateTotal
	};

	class ButtonWidget: public Widget 
	{
	public:
		ButtonWidget(WidgetID id);

		void setPosition(float x, float y);
		void setSize(float width, float height);
		void setCaption(const String& caption);
		void setVisible(bool visible);
		void enable();
		void disable();

		void setButtonStyle(int32 state, RESOURCEID font, RGBCOLOR textColor = 0xffffffff, 
			RGBCOLOR borderColor = 0xffffffff, RGBCOLOR fillColor = 0);

		virtual void render(IPlatformContext* context);
		virtual void setFocus();
		virtual void killFocus();
		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);
		virtual bool isPointIn(float x, float y);

	private:
		//void recalcTextPosition();

		struct ButtonStyle
		{
			ButtonStyle()
			{
				_font = 1;
				_textColor = 0xffffffff;
				_borderColor = 0xffffffff;
				_fillColor = 0;
				_left = _top = 0;
			}

			ButtonStyle(RESOURCEID font, RGBCOLOR textColor = 0xffffffff, RGBCOLOR borderColor = 0xffffffff, RGBCOLOR fillColor = 0)
			{
				_font = font;
				_textColor = textColor;
				_borderColor = borderColor;
				_fillColor = fillColor;
				_left = _top = 0;
			}	

			RESOURCEID _font;
			RGBCOLOR   _textColor;
			RGBCOLOR   _borderColor;
			RGBCOLOR   _fillColor;

			float _left, _top;
		};
		
		float m_x;
		float m_y;
		float m_width;
		float m_height;

		String m_caption;
				
		ButtonStyle m_styles[k_buttonStateTotal];
		int32 m_currentState;
		bool  m_isVisible;
		bool  m_isFocused;
	};

	class EditFieldWidget: public Widget {};
}

#endif
