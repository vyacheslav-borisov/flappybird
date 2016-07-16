#include "../common.h"
#include "default_game_state.h"

#include "../gfx/includes.h"

using namespace pegas;

//--------------------------------------------------------------------------------------------------
//	BaseScreenLayer class implementation
//--------------------------------------------------------------------------------------------------
void BaseScreenLayer::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onMouseButtonDown(button, x, y, flags);
		}
	}
}

void BaseScreenLayer::onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onMouseButtonUp(button, x, y, flags);
		}
	}
}

void BaseScreenLayer::onMouseMove(float x, float y, MouseFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onMouseMove(x, y, flags);
		}
	}
}

void BaseScreenLayer::onMouseWheel(NumNothes wheel, MouseFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onMouseWheel(wheel, flags);
		}
	}
}

void BaseScreenLayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onKeyDown(key, flags);
		}
	}
}

void BaseScreenLayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onKeyUp(key, flags);
		}
	}
}

void BaseScreenLayer::onChar(tchar ch)
{
	if(m_successor)
	{
		if(!(m_isActive && m_modal))
		{
			((BaseScreenLayer*)m_successor)->onChar(ch);
		}
	}
}

//-----------------------------------------------------------------------------------------
//	DefaultGameState class implementation
//-----------------------------------------------------------------------------------------
void DefaultGameState::pushLayer(BaseScreenLayerPtr layer)
{
	if(m_layers.size() > 0)
	{
		layer->setSuccessor(m_layers.back().get());		
	}

	m_layers.push_back(layer);
}

BaseScreenLayerPtr DefaultGameState::getLayer(const LayerId& id)
{
	for(std::list<BaseScreenLayerPtr>::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if((*it)->getID() == id)
		{
			return (*it);
		}
	}

	return BaseScreenLayerPtr();
}

void DefaultGameState::enter(IPlatformContext* context)
{
	context->addKeyboardController(this);
	context->addMouseController(this);

	for(std::list<BaseScreenLayerPtr>::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		(*it)->create(context);
	}
}

void DefaultGameState::leave(IPlatformContext* context)
{
	context->removeKeyboardController(this);
	context->removeMouseController(this);

	for(std::list<BaseScreenLayerPtr>::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		(*it)->destroy(context);
	}
}

void DefaultGameState::update(IPlatformContext* context)
{
	for(std::list<BaseScreenLayerPtr>::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if((*it)->isActive())
		{
			(*it)->update(context);
		}
	}
}

void DefaultGameState::render(IPlatformContext* context)
{
	Gfx* gfx = context->getGFX();
	gfx->beginDraw();
	gfx->clearCanvas();
	
	for(std::list<BaseScreenLayerPtr>::iterator it = m_layers.begin(); it != m_layers.end(); ++it)
	{
		if((*it)->isActive())
		{
			(*it)->render(context);
		}
	}

	gfx->endDraw();
}

void DefaultGameState::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onMouseButtonDown(button, x, y, flags);
	}
}

void DefaultGameState::onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onMouseButtonUp(button, x, y, flags);
	}
}

void DefaultGameState::onMouseMove(float x, float y, MouseFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onMouseMove(x, y, flags);
	}
}

void DefaultGameState::onMouseWheel(NumNothes wheel, MouseFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onMouseWheel(wheel, flags);
	}
}

void DefaultGameState::onKeyDown(KeyCode key, KeyFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onKeyDown(key, flags);
	}
}

void DefaultGameState::onKeyUp(KeyCode key, KeyFlags flags)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onKeyUp(key, flags);
	}
}

void DefaultGameState::onChar(tchar ch)
{
	if(m_layers.size() > 0)
	{
		m_layers.back()->onChar(ch);
	}
}


