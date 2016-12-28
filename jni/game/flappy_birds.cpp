#include "../common.h"
#include "flappy_birds.h"

#include "../app/common_events.h"

namespace pegas
{
	struct Event_Create_NextColumn: public Event
	{
	public:
		Event_Create_NextColumn()
		{
			//LOGW_TAG("Pegas_debug", "Event_Create_NextColumn()");
		}

		virtual EventType getType() const { return k_type; }
		static const EventType k_type;
	};
	const EventType Event_Create_NextColumn::k_type = "Event_Create_NextColumn";

	//===============================================================================
	const std::string GameWorld::k_name = "game_world";

	float GameWorld::s_columnVelocity = 0.0f;
	float GameWorld::s_spriteScale = 0.0f;
	float GameWorld::s_bornLine = 0.0f;
	float GameWorld::s_deadLine = 0.0f;
	float GameWorld::s_columnWindowHeight = 0.0f;

	float GameWorld::getSpriteScale()
	{
		return s_spriteScale;
	}

	float GameWorld::getColumnVelocity()
	{
		return s_columnVelocity;
	}

	float GameWorld::getBornLine()
	{
		return s_bornLine;
	}

	float GameWorld::getDeadLine()
	{
		return s_deadLine;
	}

	float GameWorld::getColumnWindowHeight()
	{
		return s_columnWindowHeight;
	}

	////////////////////////////////////////////////////////////////////////////////////
	GameWorld::GameWorld()
		:m_gameStarted(false), m_columnsSpawned(0)
	{

	}

	void GameWorld::onCreate(IPlatformContext* context, void* pData)
	{
		GameObject::onCreate(context, pData);

		EventManager* eventManager = context->getEventManager();
		eventManager->addEventListener(this, Event_Game_Start::k_type);
		eventManager->addEventListener(this, Event_Create_NextColumn::k_type);

		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Background::k_name)));
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Ground::k_name)));
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Bird::k_name)));

		/*Rect2D screenRect = GameScreen::getScreenRect();
		Vector3 spawnPoint(screenRect.width() * 0.5f, screenRect.height() * 0.5f, 0.0f);
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Column::k_name, spawnPoint)));*/
	}

	void GameWorld::onDestroy(IPlatformContext* context)
	{
		EventManager* eventManager = context->getEventManager();
		eventManager->removeEventListener(this);

		GameObject::onDestroy(context);
	}

	void GameWorld::onCreateSceneNode(Atlas* atlas, SceneManager* sceneManager, const Vector3& spawnPoint)
	{
		Rect2D screenRect = GameScreen::getScreenRect();
		float screenWidth = screenRect.width();

		Sprite* background = atlas->getSprite("background_day");
		s_spriteScale = screenWidth / background->width();
		s_columnVelocity = -(screenWidth * 0.5f);

		Sprite* column = atlas->getSprite("column_green_up");
		float columnWidth = column->width() * s_spriteScale;
		m_offset = columnWidth * 4.0f;
		s_bornLine = screenRect.width() + columnWidth;
		s_deadLine = -columnWidth;
		m_spawnPosition._x = s_bornLine;

		Sprite* bird = atlas->getSprite("bird");
		s_columnWindowHeight = bird->height() * s_spriteScale * 3.5;
	}

	void GameWorld::handleEvent(EventPtr evt)
	{
		if(evt->getType() == Event_Game_Start::k_type)
		{
			m_gameStarted = true;

			for(int i = 0; i < 4; i++)
			{
				spawnNewColumn();
			}
		}

		if(evt->getType() == Event_Create_NextColumn::k_type)
		{
			spawnNewColumn();
		}
	}

	void GameWorld::update(MILLISECONDS deltaTime)
	{
		if(!m_gameStarted) return;

		float dt = deltaTime / 1000.0f;
		float offset = getColumnVelocity() * dt;
		m_spawnPosition._x += offset;
	}

	void GameWorld::spawnNewColumn()
	{
		//LOGW_TAG("Pegas_debug", "GameWorld::spawnNewColumn");

		static int k_columnsInPeriod = 10;

		Rect2D screenRect = GameScreen::getScreenRect();
		float borderDown = Ground::getGroundLevel() - getColumnWindowHeight();
		float borderUp = getColumnWindowHeight();

		float t = (m_columnsSpawned % k_columnsInPeriod) / (1.0f * k_columnsInPeriod);
		float phase = t * Math::PI * 2.0f;
		float amplitude = borderDown - borderUp;
		float noiseAmplitude = amplitude * 0.3f;
		float baseVerticalOffset = borderUp + (amplitude * std::sin(phase));
		float noise = Math::rand(-noiseAmplitude, noiseAmplitude);
		baseVerticalOffset += noise;

		if(baseVerticalOffset > borderDown)
		{
			baseVerticalOffset = borderDown;
		}

		if(baseVerticalOffset < borderUp)
		{
			baseVerticalOffset = borderUp;
		}

		m_spawnPosition._y = baseVerticalOffset;

		EventManager* eventManager = m_context->getEventManager();
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Column::k_name, m_spawnPosition)));

		m_spawnPosition._x += m_offset;
		m_columnsSpawned++;
	}

	//===============================================================================
	const std::string Background::k_name = "background";

	void Background::onCreateSceneNode(Atlas* atlas,
									   SceneManager* sceneManager,
									   const Vector3& spawnPoint)
	{
		LOGI("obtaining background sprite...");
		Sprite* background = atlas->getSprite("background_day");
		background->setPivot(Sprite::k_pivotLeftTop);

		Rect2D screenRect = GameScreen::getScreenRect();

		Matrix4x4 scale;
		scale.identity();
		scale.scale(screenRect.width(), screenRect.height(), 0.0f);

		LOGI("creating background scene node...");
		SpriteSceneNode* backgroundSceneNode = new SpriteSceneNode(background);
		backgroundSceneNode->setTransfrom(scale);
		backgroundSceneNode->setZIndex(-10.0f);
		SceneNode* rootNode = sceneManager->getRootNode();

		LOGI("put background node to scene...");
		rootNode->attachChild(backgroundSceneNode);
	}

	//===============================================================================
	const std::string Ground::k_name = "ground";

	float Ground::s_groundLevel = 0;
	float Ground::getGroundLevel()
	{
		return s_groundLevel;
	}

	void Ground::onCreateSceneNode(Atlas* atlas, SceneManager* sceneManager, const Vector3& spawnPoint)
	{
		LOGI("obtaining ground sprite...");
		Sprite* ground = atlas->getSprite("ground");
		ground->setPivot(Sprite::k_pivotCenter);

		float spriteWidth = ground->width() * GameWorld::getSpriteScale();
		float spriteHeight = ground->height() * GameWorld::getSpriteScale();
		Rect2D screenRect = GameScreen::getScreenRect();
		s_groundLevel = screenRect.height() - spriteHeight;

		Matrix4x4 scale, translate, world;

		scale.identity();
		scale.scale(spriteWidth, spriteHeight, 1.0f);

		translate.identity();
		translate.translate((screenRect.width() * 0.5f), (screenRect.height() - (spriteHeight * 0.5f)), 0.0f);
		world = scale * translate;

		LOGI("creating ground scene node #1...");
		m_sceneNodes[0] = new SpriteSceneNode(ground);
		m_sceneNodes[0]->setZIndex(-8.0f);
		m_sceneNodes[0]->setTransfrom(world);


		SceneNode* rootNode = sceneManager->getRootNode();
		LOGI("put ground scene node #1 to scene...");
		rootNode->attachChild(m_sceneNodes[0]);

		translate.identity();
		translate.translate(spriteWidth, 0.0f, 0.0f);
		world = world * translate;

		LOGI("creating ground scene node #2...");
		m_sceneNodes[1] = new SpriteSceneNode(ground);
		m_sceneNodes[1]->setZIndex(-8.0f);
		m_sceneNodes[1]->setTransfrom(world);

		LOGI("put ground scene node #2 to scene...");
		rootNode->attachChild(m_sceneNodes[1]);

		m_isMoving = true;
	}

	void Ground::update(MILLISECONDS deltaTime)
	{
		if(!m_isMoving) return;

		float dt = (deltaTime * 1.0f) / 1000.0f;
		float offset = GameWorld::getColumnVelocity() * dt;

		Matrix4x4 translate;
		translate.identity();
		translate.translate(offset, 0.0f, 0.0f);

		Matrix4x4 transform = m_sceneNodes[0]->getLocalTransform();
		transform = transform * translate;
		m_sceneNodes[0]->setTransfrom(transform);

		translate.identity();
		translate.translate(transform._11, 0.0f, 0.0f);
		transform = transform * translate;
		m_sceneNodes[1]->setTransfrom(transform);

		Rect2D screenRect = GameScreen::getScreenRect();
		Rect2D aabb = m_sceneNodes[0]->getBoundBox();
		if(aabb._bottomRight._x <= screenRect._topLeft._x)
		{
			transform = transform * translate;
			m_sceneNodes[0]->setTransfrom(transform);

			SceneNode* temp = m_sceneNodes[0];
			m_sceneNodes[0] = m_sceneNodes[1];
			m_sceneNodes[1] = temp;
		}
	}

	//===============================================================================
	const std::string Column::k_name = "column";

	Column::Column()
		: m_isAboutToDestroy(false), m_isMoving(false)
	{
		//LOGW_TAG("Pegas_debug", "Column constructor");

		m_sceneNodes[k_up] = 0;
		m_sceneNodes[k_down] = 0;
	}

	void Column::onCreateSceneNode(Atlas* atlas, SceneManager* sceneManager, const Vector3& spawnPoint)
	{
		//LOGW_TAG("Pegas_debug", "Column::onCreateSceneNode");

		m_currentPosition = spawnPoint;

		//загружаем из атласа спрайты колонн-преп€тсвий
		LOGI("obtaining column sprites...");
		//нижн€€ колонна
		Sprite* spriteColumnUP = atlas->getSprite("column_green_up");
		spriteColumnUP->setPivot(Sprite::k_pivotCenter);
		//верхн€€ колонна
		Sprite* spriteColumnDown = atlas->getSprite("column_green_down");
		spriteColumnDown->setPivot(Sprite::k_pivotCenter);

		float spriteWidth = spriteColumnUP->width() * GameWorld::getSpriteScale();
		float spriteHeight = spriteColumnUP->height() * GameWorld::getSpriteScale();
		float windowHeight = GameWorld::getColumnWindowHeight();

		Matrix4x4 scale, translate, world;
		//матрица масштаба - задаем размеры колонн
		scale.identity();
		scale.scale(spriteWidth, spriteHeight, 1.0f);

		//размещаем верхнюю колонну
		translate.identity();
		translate.translate(spawnPoint._x, (spawnPoint._y - (spriteHeight * 0.5f) - (windowHeight * 0.5f)), 0.0f);
		world = scale * translate;

		//создаем узел сцены дл€ колонны
		LOGI("creating column scene node k_up...");
		m_sceneNodes[k_up] = new SpriteSceneNode(spriteColumnUP);
		m_sceneNodes[k_up]->setZIndex(-9.0f);
		m_sceneNodes[k_up]->setTransfrom(world);

		translate.identity();
		translate.translate(spawnPoint._x, (spawnPoint._y + (spriteHeight * 0.5f) + (windowHeight * 0.5f)), 0.0f);
		world = scale * translate;

		//размещаем нижнюю колонну
		LOGI("creating column scene node k_down...");
		m_sceneNodes[k_down] = new SpriteSceneNode(spriteColumnDown);
		m_sceneNodes[k_down]->setZIndex(-9.0f);
		m_sceneNodes[k_down]->setTransfrom(world);

		//помещаем узлы на сцену
		SceneNode* rootNode = sceneManager->getRootNode();
		LOGI("put column scene node k_up to scene...");
		rootNode->attachChild(m_sceneNodes[k_up]);


		LOGI("put column scene node k_down to scene...");
		rootNode->attachChild(m_sceneNodes[k_down]);

		//создаем суррогатный узел сцены дл€ окна в колонне, через которое будет пролетать птичка
		//на его основе будет создан игровой объект-тригер при пересечении которого будут засчитыватьс€ очки
		scale.identity();
		scale.scale(spriteWidth * 0.5f, windowHeight, 1.0f);
		translate.identity();
		translate.translate(spawnPoint._x, spawnPoint._y, 0.0f);
		world = scale * translate;

		LOGI("creating column scene node k_window...");
		m_sceneNodes[k_window] = new SceneNode(rootNode);
		m_sceneNodes[k_window]->setZIndex(-9.0f);
		m_sceneNodes[k_window]->setTransfrom(world);

		LOGI("put column scene node k_window to scene...");
		rootNode->attachChild(m_sceneNodes[k_window]);

		//создаем дочерние игровые объекты: два статических преп€тсви€
		//и одну зону-тригер
		EventManager* eventManager = m_context->getEventManager();
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Obstacle::k_name, (void*)m_sceneNodes[k_up])));
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Obstacle::k_name, (void*)m_sceneNodes[k_down])));
		eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Trigger::k_name, (void*)m_sceneNodes[k_window])));

		m_isMoving = true;
	}

	void Column::update(MILLISECONDS deltaTime)
	{
		//LOGW_TAG("Pegas_debug", "Column::update");

		if(!m_isMoving) return;
		if(m_isAboutToDestroy) return;

		float dt = (deltaTime * 1.0f) / 1000.0f;
		float offset = GameWorld::getColumnVelocity() * dt;

		Matrix4x4 translate;
		translate.identity();
		translate.translate(offset, 0.0f, 0.0f);

		for(int i = 0; i < k_max; i++)
		{
			Matrix4x4 matTransform = m_sceneNodes[i]->getLocalTransform();
			matTransform = matTransform * translate;
			m_sceneNodes[i]->setTransfrom(matTransform);
		}

		m_currentPosition = m_currentPosition * translate;
		if(m_currentPosition._x < GameWorld::getDeadLine())
		{
			m_isAboutToDestroy = true;

			EventManager* eventManager = m_context->getEventManager();
			eventManager->pushEventToQueye(EventPtr(new Event_Create_NextColumn()));

			killMe();
		}
	}

	void Column::onDestroy(IPlatformContext* context)
	{
		if(m_sceneNodes[k_down] && m_sceneNodes[k_up])
		{
			SceneNode* rootNode = m_sceneNodes[k_down]->getParentNode();
			if(rootNode)
			{
				for(int i = 0; i < k_max; i++)
				{
					rootNode->removeChild(m_sceneNodes[i], true);
					m_sceneNodes[i] = NULL;
				}
			}
		}

		GameObject::onDestroy(context);
	}


	//======================================================================================
	const std::string Bird::k_name = "bird";

	Bird::Bird()
		:  m_radius(0.0f)
		 , m_impulsVelocity(0.0f)
		 , m_fallVelocity(0.0f)
		 , m_impulsAngle(0.0f)
		 , m_fallAngle(0.0f)
		 , m_velocity(0.0f)
		 , m_gravity(0.0f)
		 , m_birdNode(NULL)
		 , m_mode(0)
		 , m_physicsManager(NULL)
	{

	}

	void Bird::onCreateSceneNode(Atlas* atlas,
								 SceneManager* sceneManager,
								 const Vector3& spawnPoint)
	{
		m_gravity = Ground::getGroundLevel();
		m_impulsVelocity = -(Ground::getGroundLevel() / 2.0f);
		m_fallVelocity = -m_impulsVelocity;
		m_impulsAngle	= Math::PI / 6;
		m_fallAngle = -Math::PI / 2;
		m_impulsAngle = 0.0f;

		Rect2D screenRect = GameScreen::getScreenRect();
		m_position._x = screenRect.width() * 0.3f;
		m_position._y = Ground::getGroundLevel() * 0.6f;

		Sprite* spriteBird = atlas->getSprite("bird");
		spriteBird->setPivot(Sprite::k_pivotCenter);

		float spriteWidth = spriteBird->width() * GameWorld::getSpriteScale();
		float spriteHeight = spriteBird->height() * GameWorld::getSpriteScale();
		m_radius = std::min(spriteWidth, spriteHeight) * 0.5f;
		m_mode = k_modeIdle;

		Matrix4x4 matPosition, matTransform;

		m_size.identity();
		m_size.scale(spriteWidth, spriteHeight, 0.0f);

		matPosition.identity();
		matPosition.translate(m_position._x, m_position._y, 0.0f);

		matTransform = m_size * matPosition;

		m_birdNode = new SpriteSceneNode(spriteBird);
		m_birdNode->setZIndex(-9.0f);
		m_birdNode->setTransfrom(matTransform);

		SceneNode* rootNode = sceneManager->getRootNode();
		rootNode->attachChild(m_birdNode);

		SpriteAnimation* animation = new SpriteAnimation(spriteBird);
		animation->setNumFrames(0, 4);
		animation->setFPS(8);

		m_animation = ProcessPtr(animation);
		ProcessManager* processManager = m_context->getProcessManager();
		processManager->attachProcess(m_animation);
	}

	void Bird::onCreateCollisionHull(CollisionManager* physicsManager)
	{
		m_physicsManager = physicsManager;
		m_physicsManager->registerCircle((int32)this, 2, Vector3(), m_radius);
	}

	void Bird::onCollission(GameObject* other)
	{
		if(m_mode == k_modeIdle || m_mode == k_modeShock)
		{
			return;
		}

		if(other->getName() == Obstacle::k_name)
		{
			LOGW_TAG("Pegas_debug", "OBSTACLE!");
		}

		if(other->getName() == Trigger::k_name)
		{
			LOGW_TAG("Pegas_debug", "TRIGGER!");
		}
	}

	void Bird::update(MILLISECONDS deltaTime)
	{
		static float elapsed = 0.0f;
		float dt = (deltaTime * 1.0f) / 1000.0f;
		elapsed += dt;

		float offset;
		if(m_mode == k_modeIdle)
		{
			const float k_deviation = 20.0f;
			offset = k_deviation * std::sin(Math::PI * elapsed);

			updateNodePosition(offset, true);

		}else if(m_mode != k_modeDead)
		{
			m_velocity += m_gravity * dt;

			if(m_velocity >= m_fallVelocity)
			{
				m_mode = k_modeFall;
				m_animation->suspend();

				setAngle(m_fallAngle);
			}else
			{
				float angle = interpollateAngle(m_velocity);
				setAngle(angle);
			}

			offset = m_velocity * dt;


			if(isOnTheGround(offset))
			{
				m_mode = k_modeDead;
			}

			offset = checkUpBound(offset);
			offset = checkDownBound(offset);
			m_position._y += offset;

			updateNodePosition(offset);
		}
	}

	void Bird::impuls()
	{
		if(m_mode == k_modeIdle || m_mode == k_modeFall || m_mode == k_modeFly)
		{
			m_velocity = m_impulsVelocity;

			if(m_animation->getStatus() == k_processStatusSuspended)
			{
				m_animation->resume();
			}

			m_mode = k_modeFly;
			setAngle(m_impulsAngle);
		}
	}

	bool  Bird::isOnTheGround(float offset)
	{
		float nextPosition = m_position._y + offset;
		float facingPoint = nextPosition + m_radius;

		if(facingPoint >= Ground::getGroundLevel())
		{
			return true;
		}

		return false;
	}

	float Bird::checkUpBound(float offset)
	{
		float rightOffset = offset;
		float nextPosition = m_position._y + offset;
		float facingPoint = nextPosition - m_radius;
		if(facingPoint <= 0)
		{
			nextPosition = m_radius;
			rightOffset = nextPosition - m_position._y;
		}

		return rightOffset;
	}

	float Bird::checkDownBound(float offset)
	{
		float rightOffset = offset;
		float nextPosition = m_position._y + offset;
		float facingPoint = nextPosition + m_radius;
		if(facingPoint >= Ground::getGroundLevel())
		{
			nextPosition = Ground::getGroundLevel() - m_radius;
			rightOffset = nextPosition - m_position._y;
		}

		return rightOffset;
	}

	void  Bird::updateNodePosition(float offset, bool absolute)
	{
		Matrix4x4 matTransform = m_birdNode->getLocalTransform();
		if(absolute)
		{
			matTransform._42 = m_position._y + offset;
		}else
		{
			Matrix4x4 matPosition;
			matPosition.identity();
			matPosition.translate(0.0f, offset, 0.0f);

			matTransform = matTransform * matPosition;
		}
		m_birdNode->setTransfrom(matTransform);
		m_physicsManager->transformObject((int32)this, matTransform);
	}

	void Bird::setAngle(float angle)
	{
		Matrix4x4 rotation;
		rotation.identity();
		rotation.rotateZ(angle);

		Matrix4x4 matTransform = m_birdNode->getLocalTransform();

		Matrix4x4 position;
		position.identity();
		position.translate(m_position._x, m_position._y, 0.0f);

		matTransform = m_size * rotation * position;
		m_birdNode->setTransfrom(matTransform);
	}

	float Bird::interpollateAngle(float velocity)
	{
		float angle = m_impulsAngle;
		//float k = (velocity - m_impulsVelocity) / (m_fallVelocity - m_impulsVelocity);
		if(velocity > 0.0f)
		{
			float k = velocity / m_fallVelocity;
			angle = m_impulsAngle + (m_fallAngle - m_impulsAngle) * k;
		}

		return angle;
	}

	void Bird::onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags)
	{
		//TODO: это временный код.
		//старт игры должен производить слой экрана HUD, которого пока нет
		//потом надо будет убрать
		if(m_mode == k_modeIdle)
		{
			EventManager* eventManager = m_context->getEventManager();
			eventManager->pushEventToQueye(EventPtr(new Event_Game_Start()));
		}

		impuls();
	}

	void Bird::onCreate(IPlatformContext* context, void* pData)
	{
		GameObject::onCreate(context, pData);

		context->addMouseController(this);
	}

	void Bird::onDestroy(IPlatformContext* context)
	{
		context->removeMouseController(this);
		m_physicsManager->unregisterCollisionHull((int32)this);

		GameObject::onDestroy(context);
	}

	//===============================================================================
	const std::string Obstacle::k_name = "Obstacle";
	const std::string Trigger::k_name = "Trigger";

	CollidableObject::CollidableObject()
		:m_physicsManager(0)
	{

	}

	void CollidableObject::onCreate(IPlatformContext* context, void* pData)
	{
		GameObject::onCreate(context, pData);

		SceneNode* sceneNode = reinterpret_cast<SceneNode*>(pData);
		sceneNode->addListener(this);
	}

	void CollidableObject::onCreateCollisionHull(CollisionManager* physicsManager)
	{
		GameObject::onCreateCollisionHull(physicsManager);

		m_physicsManager = physicsManager;

		CollisionManager::PointList points;
		points.push_back(Vector3(-0.5f, -0.5f, 0.0f));
		points.push_back(Vector3(0.5f, -0.5f, 0.0f));
		points.push_back(Vector3(0.5f, 0.5f, 0.0f));
		points.push_back(Vector3(-0.5f, 0.5f, 0.0f));

		m_physicsManager->registerPoligon((int32)this, 1, points);
	}

	void CollidableObject::onDestroy(IPlatformContext* context)
	{
		GameObject::onDestroy(context);

		m_physicsManager->unregisterCollisionHull((int32)this);
	}

	void CollidableObject::onTransfromChanged(SceneNode* sender)
	{
		//LOGW_TAG("Pegas_debug", "CollidableObject::onTransfromChanged");

		Matrix4x4 m = sender->getWorldTransfrom();
		m_physicsManager->transformObject((int32)this, m);
	}

	void CollidableObject::onNodeRemoved(SceneNode* sender)
	{
		killMe();
	}

	//===============================================================================
	class FlappyBirdsFactory: public GameObjectFactory
	{
	public:
		virtual GameObject* create(const std::string& name)
		{
			//LOGW_TAG("Pegas_debug", "FlappyBirdsFactory::create: %s", name.c_str());

			if(name == GameWorld::k_name)
			{
				return new GameWorld();
			}

			if(name == Background::k_name)
			{
				return new Background();
			}

			if(name == Ground::k_name)
			{
				return new Ground();
			}

			if(name == Bird::k_name)
			{
				return new Bird();
			}

			if(name == Column::k_name)
			{
				return new Column();
			}

			if(name == Trigger::k_name)
			{
				return new Trigger();
			}

			if(name == Obstacle::k_name)
			{
				return new Obstacle();
			}

			return NULL;
		}
	};

	static FlappyBirdsFactory gs_game_object_factory;
}



