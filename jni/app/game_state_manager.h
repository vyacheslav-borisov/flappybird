#ifndef PEGAS_APP_GAME_STATE_MANAGER_H_
#define PEGAS_APP_GAME_STATE_MANAGER_H_

#include "interfaces.h"

namespace pegas
{
	class GameStateManager: public IEventListener
	{
	public:
		GameStateManager(): m_shutdownGame(false), m_context(NULL) {}

		void create(IPlatformContext* context);
		void destroy(IPlatformContext* context);
		void update(IPlatformContext* context);

		void addGameState(GameStatePtr state);
		void removeState(GameStateID id);
		void changeState(GameStateID newStateId);
		void forwardToState(GameStateID newStateId);
		void backwardToPreviousState();
		void shutdownGame();
		bool isAboutToQuit() const { return m_shutdownGame; }

	public:
		//IEventListener
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();

	private:
		void _changeState(GameStateID newStateId);
		void _forwardToState(GameStateID newStateId);
		void _backwardToPreviousState();

	private:
		std::map<GameStateID, GameStatePtr> m_statesMap;
		std::stack<GameStatePtr> m_statesStack;
		IPlatformContext* m_context;
		bool m_shutdownGame;
	};
}

#endif /* APP_GAME_STATE_MANAGER_H_ */
