#pragma once

#include "../core/types.h"
#include "../core/smart_pointer.h"
#include "../system/interfaces.h"

namespace pegas
{
		typedef std::string EventType;

		struct Event;
		typedef SmartPointer<Event> EventPtr;

		struct Event
		{
		public:
			virtual ~Event() {};
			virtual EventType getType() const = 0;

			template<class T>
			T* cast()
			{
				return static_cast<T*>(this);
			}
		};

		
		typedef std::string ListenerType;
		class IEventListener
		{
		public:
			virtual ~IEventListener() {}
			virtual void handleEvent(EventPtr evt) = 0;
			virtual ListenerType getListenerName() = 0;
		};

		typedef IEventListener* EventListenerPtr;

		class Process;
		typedef int ProcessHandle;
		typedef SmartPointer<Process> ProcessPtr;


		class IUpdate
		{
		public:
			virtual ~IUpdate() {}
			virtual void update(float deltaTime, float timeLimit) = 0;
		};

		class IRenderable
		{
		public:
			virtual ~IRenderable() {}
			virtual void render() = 0;
		};



		//----------------------------------------------------------------------------------------------------------
		//	IPlatformContext;
		//----------------------------------------------------------------------------------------------------------
		typedef int32 GameStateID;
		
		class IGameState;
		typedef SmartPointer<IGameState> GameStatePtr;

		class Gfx;
		class Timer;
		class ProcessManager;
		class EventManager;
		class GameStateManager;

		class IPlatformContext
		{
		public:
			virtual ~IPlatformContext() {}

			virtual void addKeyboardController(IKeyboardController* controller) = 0;
			virtual void removeKeyboardController(IKeyboardController* controller) = 0;
			virtual void addMouseController(IMouseController* controller) = 0;
			virtual void removeMouseController(IMouseController* controller) = 0;
			
			virtual Gfx* getGFX() = 0;
			virtual Timer* getTimer() = 0;
			virtual ProcessManager* getProcessManager() = 0;
			virtual EventManager* getEventManager() = 0;
			virtual GameStateManager* getGameStateManager() = 0;
		};


		//----------------------------------------------------------------------------------------------------
		//	IGameState
		//----------------------------------------------------------------------------------------------------
		
		class IGameState
		{
		public:
			IGameState(const GameStateID& id): m_id(id) {};
			virtual ~IGameState() {};

			GameStateID getID() { return m_id; };
			
			virtual void enter(IPlatformContext* context) = 0;
			virtual void leave(IPlatformContext* context) = 0;
			virtual void update(IPlatformContext* context) = 0;
			virtual void render(IPlatformContext* context) = 0;

		protected:
			GameStateID	m_id;
		};

		//---------------------------------------------------------------------------------------------------
		//	IScreenLayer
		//---------------------------------------------------------------------------------------------------
		typedef String LayerType;
		typedef int32 LayerId;
		
		
		class IScreenLayer
		{
		public:
			IScreenLayer(const LayerType& type, const LayerId& id): 
			  m_type(type), m_id(id),  m_isActive(false), m_successor(0) {};

			virtual ~IScreenLayer() {};

			LayerId getID() { return m_id; };
			LayerType getType() { return m_type; };

			virtual void update(IPlatformContext* context) = 0;
			virtual void render(IPlatformContext* context) = 0;
			virtual void create(IPlatformContext* context) = 0;
			virtual void destroy(IPlatformContext* context) = 0;
			virtual void onActivate(bool isActive) {}
			
			void setActivity(bool active) 
			{ 
				m_isActive = active;
				onActivate(m_isActive);
			}

			bool isActive() const { return m_isActive; }

			void setSuccessor(IScreenLayer* successor) { m_successor = successor; };
			IScreenLayer* getSuccessor() const { return m_successor; };

		protected:
			LayerId			m_id;
			LayerType		m_type;	
			IScreenLayer*	m_successor;
			bool			m_isActive;
		};

		typedef SmartPointer<IScreenLayer> ScreenLayerPtr;
	
}
