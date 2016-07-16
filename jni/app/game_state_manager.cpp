#include "../common.h"
#include "game_state_manager.h"

#include "event_system.h"
#include "common_events.h"
#include "../system/log.h"

namespace pegas
{
	void GameStateManager::create(IPlatformContext* context)
	{
		LOGI("GameStateManager::create");

		m_context = context;

		EventManager* eventMgr = context->getEventManager();

		eventMgr->addEventListener(this, Event_Game_ChangeState::k_type);
		eventMgr->addEventListener(this, Event_Game_ForwardToState::k_type);
		eventMgr->addEventListener(this, Event_Game_BackwardToPreviousState::k_type);
	}

	void GameStateManager::destroy(IPlatformContext* context)
	{
		LOGI("GameStateManager::destroy");

		while(!m_statesStack.empty())
		{
			m_statesStack.top()->leave(context);
			m_statesStack.pop();
		}

		EventManager* eventMgr = context->getEventManager();
		eventMgr->removeEventListener(this);
	}

	void GameStateManager::update(IPlatformContext* context)
	{
		if(!m_statesStack.empty())
		{
			m_statesStack.top()->update(context);
			m_statesStack.top()->render(context);
		}
	}

	void GameStateManager::addGameState(GameStatePtr state)
	{
		LOGI("GameStateManager::addGameState [state ID: %d]", state->getID());

		if(m_statesMap.count(state->getID()) == 0)
		{
			m_statesMap[state->getID()] = state;
		}
	}

	void GameStateManager::removeState(GameStateID id)
	{
		LOGI("GameStateManager::removeState  [state ID: %d]", id);

		if(m_statesMap.count(id) != 0)
		{
			std::stack<GameStatePtr> temp;
			while(!m_statesStack.empty())
			{
				if(m_statesStack.top()->getID() != id)
					temp.push(m_statesStack.top());

				m_statesStack.pop();
			}
			m_statesStack = temp;
			m_statesMap.erase(id);
		}
	}

	void GameStateManager::changeState(GameStateID newStateId)
	{
		LOGI("GameStateManager::changeState  [state ID: %d]", newStateId);

		EventPtr evt(new Event_Game_ChangeState(newStateId));
		TheEventMgr.pushEventToQueye(evt);
	}

	void GameStateManager::forwardToState(GameStateID newStateId)
	{
		LOGI("GameStateManager::forwardToState  [state ID: %d]", newStateId);

		EventPtr evt(new Event_Game_ForwardToState(newStateId));
		TheEventMgr.pushEventToQueye(evt);
	}

	void GameStateManager::backwardToPreviousState()
	{
		LOGI("GameStateManager::backwardToPreviousState");

		EventPtr evt(new Event_Game_BackwardToPreviousState());
		TheEventMgr.pushEventToQueye(evt);
	}

	void GameStateManager::shutdownGame()
	{
		LOGI("GameStateManager::shutdownGame");

		m_shutdownGame = true;
	}

	//-------------------------------------------------------------------------------------------
	// IEventListener implementation
	//-------------------------------------------------------------------------------------------
	void GameStateManager::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Game_ChangeState::k_type)
		{
			Event_Game_ChangeState* pEvent = evt->cast<Event_Game_ChangeState>();
			_changeState(pEvent->_id);
		}

		if(evt->getType() == Event_Game_ForwardToState::k_type)
		{
			Event_Game_ForwardToState* pEvent = evt->cast<Event_Game_ForwardToState>();
			_forwardToState(pEvent->_id);
		}

		if(evt->getType() == Event_Game_BackwardToPreviousState::k_type)
		{
			_backwardToPreviousState();
		}
	}

	ListenerType GameStateManager::getListenerName()
	{
		return "GameStateManager";
	}

	//-----------------------------------------------------------------------------------
	//	private functions
	//-----------------------------------------------------------------------------------
	void GameStateManager::_changeState(GameStateID newStateId)
	{
		if(m_statesMap.count(newStateId) == 0)
		{
			shutdownGame();
			return;
		}

		if(!m_statesStack.empty())
		{
			if(m_statesStack.top()->getID() == newStateId)
			{
				return;
			}

			m_statesStack.top()->leave(m_context);
			m_statesStack.pop();
		}

		m_statesStack.push(m_statesMap[newStateId]);
		m_statesStack.top()->enter(m_context);
	}

	void GameStateManager::_forwardToState(GameStateID newStateId)
	{
		if(m_statesMap.count(newStateId) == 0)
		{
			shutdownGame();
			return;
		}

		if(!m_statesStack.empty())
		{
			if(m_statesStack.top()->getID() == newStateId)
			{
				return;
			}
			m_statesStack.top()->leave(m_context);
		}
		m_statesStack.push(m_statesMap[newStateId]);
		m_statesStack.top()->enter(m_context);
	}

	void GameStateManager::_backwardToPreviousState()
	{
		if(m_statesStack.size() < 2)
		{
			shutdownGame();
			return;
		}

		m_statesStack.top()->leave(m_context);
		m_statesStack.pop();
		m_statesStack.top()->enter(m_context);
	}
}




