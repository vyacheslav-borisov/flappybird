#ifndef PEGAS_GUI_GUILAYER_H
#define PEGAS_GUI_GUILAYER_H
#pragma once

#include "../core/includes.h"
#include "../app/includes.h"
#include "../gfx/gfx.h"
#include "../gfx/scene.h"

namespace pegas
{
	typedef int32 WidgetID;
	const  WidgetID NULL_WIDGET_ID = -1;

	class Widget
	{
	public:
		Widget(WidgetID id): m_id(id) {}
		virtual ~Widget() {}

		void setBoundbox(const Rect2D& boundBox) { m_boundBox = boundBox; }
		Rect2D getBoundBox() { return m_boundBox; }
		WidgetID getID() const { return m_id; }

		bool isPointIn(float x, float y) { return false; }

		virtual void setFocus() {}
		virtual void killFocus() {}

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags) {}
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags) {}
		virtual void onMouseMove(float x, float y, MouseFlags flags) {}
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags) {}

		virtual void onKeyDown(KeyCode key, KeyFlags flags) {}
		virtual void onKeyUp(KeyCode key, KeyFlags flags) {}
		virtual void onChar(tchar ch) {}

		virtual void render(Gfx* gfx) {}

	protected:
		WidgetID m_id;
		Rect2D   m_boundBox;
	};
	
	typedef SmartPointer<Widget> WidgetPtr;


	class WidgetSceneNode: public SceneNode
	{
	public:
		WidgetSceneNode(Widget* widget, SceneNode* parentNode = NULL);

		virtual void setTransfrom(const Matrix4x4& transform);
		virtual Matrix4x4  getLocalTransform();
		virtual void render(Gfx* gfx);
		virtual Rect2D getBoundBox() { return m_cachedBoundBox; }

	private:
		Widget* m_widget;
		Rect2D m_cachedBoundBox;
	};

	class GUILayer: public BaseScreenLayer
	{
	public:
		GUILayer(const LayerId& id, bool modal): 
		  BaseScreenLayer(_text("GUI"), id, modal)  
		{
			m_focusedWidget = -1;
		}

		void addWidget(WidgetPtr widget, WidgetID parentWidget = NULL_WIDGET_ID);
		void changeFocusNext();
		void changeFocusPrev();

		WidgetPtr getFocusedWidget();

		virtual void render(IPlatformContext* context);
		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseMove(float x, float y, MouseFlags flags);
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);
		virtual void onKeyUp(KeyCode key, KeyFlags flags);
		virtual void onChar(tchar ch);


	protected:
		typedef std::vector<WidgetPtr> WidgetList;
		typedef WidgetList::iterator WidgetListIt;
		typedef std::map<WidgetID, WidgetSceneNode*> WidgetNodeMap;
		typedef  WidgetNodeMap::iterator  WidgetNodeMapIt;

		SceneManager    m_sceneManager;
		WidgetList		m_widgets;
		WidgetNodeMap   m_widgetsNodeMap;
		int32			m_focusedWidget;
	};
}

#endif
