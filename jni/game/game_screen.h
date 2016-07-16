#ifndef GAME__H_
#define GAME__H_

#include "../core/includes.h"
#include "../gfx/includes.h"
#include "../physics/collision_checker.h"
#include "../app/interfaces.h"
#include "../app/processes.h"
#include "../app/default_game_state.h"
#include "../system/log.h"

namespace pegas
{
	class GameObject: public Process
	{
	public:
		GameObject(): m_context(NULL) {}
		virtual ~GameObject() {}

		virtual std::string getName() = 0;

		virtual void onCreate(IPlatformContext* context)
		{
			m_context = context;
		}

		virtual void onCreateSceneNode(Atlas* atlas,
				SceneManager* sceneManager, const Vector3& spawnPoint) {}
		virtual void onCreateCollisionHull(CollisionManager* physicsManager) {}
		virtual void onCollission(GameObject* other) {}
		virtual void onDestroy(IPlatformContext* context) {}
		virtual void update(MILLISECONDS deltaTime) {}

		virtual void terminate()
		{
			onDestroy(m_context);

			Process::terminate();
		}

	protected:
		IPlatformContext* m_context;
	};

	class GameObjectFactory: public Singleton<GameObjectFactory>
	{
	public:
		GameObjectFactory(): Singleton<GameObjectFactory>(*this) {}
		virtual ~GameObjectFactory() {};

		  virtual GameObject* create(const std::string& name) = 0;
	};

	class GameScreen : public BaseScreenLayer, public IEventListener
	{
	public:
		GameScreen();

		virtual void create(IPlatformContext* context);
		virtual void destroy(IPlatformContext* context);
		virtual void update(IPlatformContext* context);
		virtual void render(IPlatformContext* context);

		virtual void onKeyDown(KeyCode key, KeyFlags flags);

		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	private:
		IPlatformContext*	m_context;
		CollisionManager    m_physicsManager;
		ProcessManager		m_processManager;
		SceneManager 		m_sceneManager;
		SmartPointer<Atlas> m_atlas;

		Rect2D				m_renderRect;
		Matrix4x4 			m_viewMatrix;
		Matrix4x4 			m_projectionMatrix;

		bool				m_gamePaused;
		float 				m_prevTime;
	};

	struct Event_Create_GameObject: public Event
	{
	public:
		Event_Create_GameObject(const std::string& name)
					: _name(name)
		{
			LOGI("Event_Create_GameObject");
		}

		Event_Create_GameObject(const std::string& name, const Vector3& spawnPoint)
			: _name(name), _spawnPoint(spawnPoint)
		{
			LOGI("Event_Create_GameObject");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		std::string _name;
		Vector3		_spawnPoint;
	};

	struct Event_Destroy_GameObject: public Event
	{
	public:
		Event_Destroy_GameObject(ProcessHandle id): _id(id)
		{
			LOGI("Event_Destroy_GameObject");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		ProcessHandle _id;
	};
}


#endif /* GAME__H_ */
