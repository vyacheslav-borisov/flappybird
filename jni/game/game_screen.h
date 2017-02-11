#ifndef GAME__H_
#define GAME__H_

#include "../core/includes.h"
#include "../gfx/includes.h"
#include "../physics/base_physics.h"
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

		virtual void onCreate(IPlatformContext* context, void* pData)
		{
			m_context = context;
		}

		virtual void onCreateSceneNode(Atlas* atlas,
				SceneManager* sceneManager, const Vector3& spawnPoint) {}
		virtual void onCreateCollisionHull(IPhysics* physicsManager) {}
		virtual void onCollission(GameObject* other) {}
		virtual void onDestroy(IPlatformContext* context) {}
		virtual void update(MILLISECONDS deltaTime) {}

		virtual void terminate()
		{
			onDestroy(m_context);

			Process::terminate();
		}

	protected:
		void killMe();

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

	public:
		static Rect2D getScreenRect();

	private:
		IPlatformContext*	m_context;
		BasePhysics2   		m_physicsManager;
		ProcessManager		m_processManager;
		SceneManager 		m_sceneManager;
		SmartPointer<Atlas> m_atlas;

		Matrix4x4 			m_viewMatrix;
		Matrix4x4 			m_projectionMatrix;

		bool				m_gamePaused;
		float 				m_prevTime;

	private:
		static Rect2D				s_screenRect;
	};

	struct Event_Create_GameObject: public Event
	{
	public:
		Event_Create_GameObject(const std::string& name, void* pData = 0)
					: _name(name), _pData(pData)
		{
			LOGI("Event_Create_GameObject");
		}

		Event_Create_GameObject(const std::string& name, const Vector3& spawnPoint, void* pData = 0)
			: _name(name), _spawnPoint(spawnPoint), _pData(pData)
		{
			LOGI("Event_Create_GameObject");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;

		std::string _name;
		Vector3		_spawnPoint;
		void*		_pData;
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
