#include "../common.h"
#include "gui_layer.h"

using namespace pegas;

//-------------------------------------------------------------------------------------------------
//	WidgetSceneNode implementation
//-------------------------------------------------------------------------------------------------
WidgetSceneNode::WidgetSceneNode(Widget* widget, SceneNode* parentNode)
	:SceneNode(parentNode), m_widget(widget)
{

}

void WidgetSceneNode::setTransfrom(const Matrix4x4& transform)
{
	SceneNode::setTransfrom(transform);

	Matrix4x4 world = getWorldTransfrom();
	Vector3 topLeft = Vector3(0.0f, 0.0f, 0.0f) * world;
	Vector3 bottomRight = Vector3(1.0f, 1.0f, 0.0f) * world;

	m_cachedBoundBox._topLeft._x = topLeft._x;
	m_cachedBoundBox._topLeft._y = topLeft._y;
	m_cachedBoundBox._bottomRight._x = bottomRight._x;
	m_cachedBoundBox._bottomRight._y = bottomRight._y;
}

Matrix4x4  WidgetSceneNode::getLocalTransform()
{
	Matrix4x4 result;
	result.identity();

	if(m_widget)
	{
		Matrix4x4 scale, translate;

		Rect2D boundBox = m_widget->getBoundBox();
		translate.identity();
		translate.translate(boundBox._topLeft._x, boundBox._topLeft._y, 0.0f);

		Point2D size  = boundBox._bottomRight - boundBox._topLeft;
		scale.identity();
		scale.scale(size._x, size._y, 1.0f);

		result = scale * translate;
	}

	Matrix4x4 local = SceneNode::getLocalTransform();
	result = result * local;

	return result;
}

void WidgetSceneNode::render(Gfx* gfx)
{
	Matrix4x4 world = getWorldTransfrom();
	gfx->setWorldMatrix(world);
	if(m_widget)
	{
		m_widget->render(gfx);
	}
}

//-------------------------------------------------------------------------------------------------
//	GUILayer implementation
//-------------------------------------------------------------------------------------------------
WidgetPtr GUILayer::getFocusedWidget()
{
	if(m_widgets.size() > 0 && m_focusedWidget != -1)
	{
		return m_widgets[m_focusedWidget];
	}

	return WidgetPtr(); 
}

void GUILayer::addWidget(WidgetPtr widget, WidgetID parentWidget)
{
	WidgetID id = widget->getID();
	if(m_widgetsNodeMap.count(id) == 0)
	{
		m_widgets.push_back(widget);

		SceneNode* parentNode = NULL;
		if(m_widgetsNodeMap.count(parentWidget))
		{
			parentNode = m_widgetsNodeMap[parentWidget];
		}else
		{
			parentNode = m_sceneManager.getRootNode();
		}

		WidgetSceneNode* widgetNode = new WidgetSceneNode(widget.get());
		parentNode->attachChild(widgetNode);

		m_widgetsNodeMap[id] = widgetNode;
	}
}

void GUILayer::changeFocusNext()
{
	if(m_widgets.size() == 0) return;

	if(m_focusedWidget != -1)
	{
		m_widgets[m_focusedWidget]->killFocus();
	}

	m_focusedWidget++;
	if(m_focusedWidget >= m_widgets.size())
	{
		m_focusedWidget = 0;
	}

	m_widgets[m_focusedWidget]->setFocus();	
}

void GUILayer::changeFocusPrev()
{
	if(m_widgets.size() == 0) return;

	if(m_focusedWidget != -1)
	{
		m_widgets[m_focusedWidget]->killFocus();
	}

	m_focusedWidget--;
	if(m_focusedWidget < 0)
	{
		m_focusedWidget = m_widgets.size() - 1;
	}

	m_widgets[m_focusedWidget]->setFocus();
}

void GUILayer::render(IPlatformContext* context)
{
	Gfx* gfx = context->getGFX();

	Rect2D area;
	area._bottomRight._x = gfx->getCanvasWidth();
	area._bottomRight._y = gfx->getCanvasHeight();

	m_sceneManager.render(gfx, area);
}

void GUILayer::create(IPlatformContext* context)
{
	m_focusedWidget = -1;

	Gfx* gfx = context->getGFX();

	Rect2D area;
	area._bottomRight._x = gfx->getCanvasWidth();
	area._bottomRight._y = gfx->getCanvasHeight();

	m_sceneManager.create(area);
}

void GUILayer::destroy(IPlatformContext* context)
{
	m_sceneManager.destroy();
	m_widgets.clear();
}

void GUILayer::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	if(isActive())
	{
		WidgetPtr focused = getFocusedWidget(); 
		for(int32 i = 0; i < m_widgets.size(); i++)
		{
			WidgetPtr current = m_widgets[i];

			if(current->isPointIn(x, y))
			{
				current->setFocus();
				current->onMouseButtonDown(button, x, y, flags);

				if(focused.IsValid())
				{
					focused->killFocus();
				}

				m_focusedWidget = i;
				break;

			}//if((*it)->isPointIn(x, y))
		}//for(int32 i = 0; i < m_widgets.size(); i++)		
	}//if(isActive())

	

	BaseScreenLayer::onMouseButtonDown(button, x, y, flags);
}

void GUILayer::onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags)
{
	WidgetPtr focused = getFocusedWidget();

	if(isActive())
	{
		
		for(int32 i = 0; i < m_widgets.size(); i++)
		{
			WidgetPtr current = m_widgets[i];
		
			if(focused.IsValid() && current == focused) continue;

			if(current->isPointIn(x, y))
			{
				current->onMouseButtonUp(button, x, y, flags);
				break;
			}//if((*it)->isPointIn(x, y))
		}//for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	}//if(isActive())

	if(focused.IsValid())
	{
		focused->onMouseButtonUp(button, x, y, flags);
	}

	BaseScreenLayer::onMouseButtonUp(button, x, y, flags);
}

void GUILayer::onMouseMove(float x, float y, MouseFlags flags)
{
	if(isActive())
	{
		for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
		{
			(*it)->onMouseMove(x, y, flags);
		}//for(WidgetListIt it = m_widgets.begin(); it != m_widgets.end(); ++it)
	}//if(isActive())

	BaseScreenLayer::onMouseMove(x, y, flags);
}

void GUILayer::onMouseWheel(NumNothes wheel, MouseFlags flags)
{
	WidgetPtr focused = getFocusedWidget();
	if(focused.IsValid())
	{
		focused->onMouseWheel(wheel, flags);
	}

	BaseScreenLayer::onMouseWheel(wheel, flags);
}

void GUILayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	WidgetPtr focused = getFocusedWidget();
	if(focused.IsValid())
	{
		focused->onKeyDown(key, flags);
	}	

	BaseScreenLayer::onKeyDown(key, flags);
}

void GUILayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	WidgetPtr focused = getFocusedWidget();
	if(focused.IsValid())
	{
		focused->onKeyUp(key, flags);
	}	

	BaseScreenLayer::onKeyUp(key, flags);
}

void GUILayer::onChar(tchar ch)
{
	WidgetPtr focused = getFocusedWidget();
	if(focused.IsValid())
	{
		focused->onChar(ch);
	}	

	BaseScreenLayer::onChar(ch);
}


