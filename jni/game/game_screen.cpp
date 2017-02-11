#include "../common.h"
#include "game_screen.h"

#include "../app/game_state_manager.h"
#include "../app/event_system.h"
#include "../app/common_events.h"

namespace pegas
{
	const EventType Event_Create_GameObject::k_type = "Event_Create_GameObject";
	const EventType Event_Destroy_GameObject::k_type = "Event_Destroy_GameObject";

	//=============================================================================
	// GameScreen
	//=============================================================================
	Rect2D	GameScreen::s_screenRect;
	Rect2D GameScreen::getScreenRect()
	{
		return s_screenRect;
	}

	GameScreen::GameScreen()
		:BaseScreenLayer(_text("game"), 1, false), m_context(NULL), m_gamePaused(false), m_prevTime(-1)
	{
		LOGI("GameScreen constructor");
	}

	void GameScreen::create(IPlatformContext* context)
	{
		LOGI("GameScreen::create");

		m_prevTime = context->getTimer()->now();

		LOGI("setup perspective...");
		m_context = context;
		Gfx* gfx = context->getGFX();

		float canvasWidth = gfx->getCanvasWidth();
		float canvasHeight = gfx->getCanvasHeight();
		float viewCenterX = canvasWidth * 0.5f;
		float viewCenterY = canvasHeight * 0.5f;

		m_viewMatrix.lookAt(Vector3(viewCenterX, viewCenterY, 1.0f),
			Vector3(viewCenterX, viewCenterY, 0.0f), Vector3(0.0f, -1.0f, 0.0f));
		m_projectionMatrix.orthoLH(canvasWidth, canvasHeight, 0.5f, 100.0f);

		LOGI("loading atlas...");
		m_atlas = SmartPointer<Atlas>(gfx->createAtlas("atlas.xml"));
		m_atlas->load();

		Rect2D worldArea;
		worldArea._topLeft = Point2D(-5000, -5000);
		worldArea._bottomRight = Point2D(5000, 5000);

		LOGI("setup scene manager...");
		m_sceneManager.create(worldArea);

		LOGI("setup physics manager...");
		m_physicsManager.create(worldArea);

		LOGI("setup process manager...");
		m_processManager.init(context->getTimer());

		s_screenRect._topLeft = Point2D(0.0f, 0.0f);
		s_screenRect._bottomRight = Point2D(canvasWidth, canvasHeight);

		EventManager* eventManager = context->getEventManager();
		eventManager->addEventListener(this, Event_Create_GameObject::k_type);
		eventManager->addEventListener(this, Event_Destroy_GameObject::k_type);
		eventManager->addEventListener(this, Event_Game_Pause::k_type);
		eventManager->addEventListener(this, Event_Game_Resume::k_type);

		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject("game_world")));
	}

	void GameScreen::destroy(IPlatformContext* context)
	{
		LOGI("GameScreen::destroy");

		m_processManager.terminateAllProcesses();

		LOGI("deleting scene manager");
		m_sceneManager.destroy();

		LOGI("deleting physics manager...");
		m_physicsManager.destroy();

		LOGI("unloading atlas");
		m_atlas->unload();

		EventManager* eventManager = context->getEventManager();
		eventManager->removeEventListener(this);
	}

	void GameScreen::update(IPlatformContext* context)
	{
		if(m_gamePaused) return;

		Timer* timer = context->getTimer();

		float currentTime = timer->now();
		float elapsed = currentTime - m_prevTime;
		MILLISECONDS deltaTime = (MILLISECONDS)(elapsed * 1000.0f);


		if(true /*deltaTime > 16*/)
		{
			LOGI_LOOP("current time: %.3f", currentTime);
			LOGI_LOOP("prev time: %.3f", m_prevTime);
			LOGI_LOOP("elapsed: %.3f", elapsed);

			m_prevTime = currentTime;

			m_processManager.updateProcesses(deltaTime);

			m_physicsManager.update();
			IPhysics::CollisionPairList& pairs = m_physicsManager.getCollidedPairs();
			for(IPhysics::CollisionPairListIt it = pairs.begin(); it != pairs.end(); ++it)
			{
				GameObject* a = (GameObject*)it->first;
				GameObject* b = (GameObject*)it->second;
				a->onCollission(b);
				b->onCollission(a);
			}
		}
	}

	void GameScreen::render(IPlatformContext* context)
	{
		Gfx* gfx = context->getGFX();
		gfx->setViewMatrix(m_viewMatrix);
		gfx->setProjectionMatrix(m_projectionMatrix);

		m_sceneManager.render(gfx, s_screenRect);
	}

	void GameScreen::onKeyDown(KeyCode key, KeyFlags flags)
	{
		if(key == 4)
		{
			m_context->getGameStateManager()->shutdownGame();
		}
	}

	void GameScreen::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Create_GameObject::k_type)
		{
			//LOGW_TAG("Pegas_debug", "processing Event_Create_GameObject");

			Event_Create_GameObject* pEvent = evt->cast<Event_Create_GameObject>();
			GameObject* actor = GameObjectFactory::getInstance().create(pEvent->_name);
			if(actor)
			{
				m_processManager.attachProcess(ProcessPtr(actor));
				actor->onCreate(m_context, pEvent->_pData);
				actor->onCreateSceneNode(m_atlas.get(), &m_sceneManager, pEvent->_spawnPoint);
				actor->onCreateCollisionHull(&m_physicsManager);
			}
		}

		if(evt->getType() == Event_Destroy_GameObject::k_type)
		{
			Event_Destroy_GameObject* pEvent = evt->cast<Event_Destroy_GameObject>();
			m_processManager.terminateProcess(pEvent->_id);
		}

		if(evt->getType() == Event_Game_Pause::k_type)
		{
			m_gamePaused = true;
		}

		if(evt->getType() == Event_Game_Resume::k_type)
		{
			m_gamePaused = false;
		}
	}

	ListenerType GameScreen::getListenerName()
	{
		return "GameScreen";
	}

	//========================================================================================
	void GameObject::killMe()
	{
		EventManager* eventManager = m_context->getEventManager();
		eventManager->pushEventToQueye(EventPtr(new Event_Destroy_GameObject(m_handle)));
	}
}



