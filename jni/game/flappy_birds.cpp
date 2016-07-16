#include "../common.h"
#include "game_screen.h"

#include "../app/event_system.h"

namespace pegas
{
	//тестовый коммит
	//===============================================================================
	class Background: public GameObject
	{
	public:
		static const std::string k_name;
	public:
		virtual std::string getName() { return k_name; }

		virtual void onCreateSceneNode(Atlas* atlas,
									   SceneManager* sceneManager,
									   const Vector3& spawnPoint)
		{
			LOGI("obtaining background sprite...");
			Sprite* background = atlas->getSprite("background_day");
			background->setPivot(Sprite::k_pivotLeftTop);

			Gfx* gfx = m_context->getGFX();
			float canvasWidth = gfx->getCanvasWidth();
			float canvasHeight = gfx->getCanvasHeight();

			Matrix4x4 scale;
			scale.identity();
			scale.scale(canvasWidth, canvasHeight, 0.0f);

			LOGI("creating background scene node...");
			SpriteSceneNode* backgroundSceneNode = new SpriteSceneNode(background);
			backgroundSceneNode->setTransfrom(scale);
			backgroundSceneNode->setZIndex(-10.0f);

			SceneNode* rootNode = sceneManager->getRootNode();
			LOGI("put background node to scene...");
			rootNode->attachChild(backgroundSceneNode);
		}
	};

	const std::string Background::k_name = "background";

	//===============================================================================
	class Ground: public GameObject
	{
	public:
		static const std::string k_name;
	public:
		Ground(): m_isMoving(false), m_velosity(0.0f) {}

		virtual std::string getName() { return k_name; }
		virtual void onCreateSceneNode(Atlas* atlas, SceneManager* sceneManager, const Vector3& spawnPoint);
		virtual void update(MILLISECONDS deltaTime);

	private:
		SceneNode*  m_sceneNodes[2];
		Rect2D 		m_screenRect;
		bool		m_isMoving;
		float       m_velosity;
	};

	const std::string Ground::k_name = "ground";

	void Ground::onCreateSceneNode(Atlas* atlas, SceneManager* sceneManager, const Vector3& spawnPoint)
	{
		Gfx* gfx = m_context->getGFX();
		float screenWidth = gfx->getCanvasWidth();
		float screenHeight = gfx->getCanvasHeight();
		m_screenRect = Rect2D(0.0f, 0.0f, screenWidth, screenHeight);

		LOGI("obtaining background sprite...");
		Sprite* background = atlas->getSprite("background_day");

		LOGI("obtaining ground sprite...");
		Sprite* ground = atlas->getSprite("ground");
		ground->setPivot(Sprite::k_pivotCenter);

		float spriteWidth = ground->width();
		float spriteHeight = ground->height();

		float spriteScale = screenWidth / background->width();
		spriteWidth = spriteWidth * spriteScale;
		spriteHeight = spriteHeight * spriteScale;

		Matrix4x4 scale, translate, world;
		scale.identity();
		scale.scale(spriteWidth, spriteHeight, 1.0f);

		translate.identity();
		translate.translate((screenWidth * 0.5f),
				(screenHeight - (spriteHeight * 0.5f)), 0.0f);
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
		m_velosity = screenWidth * 0.5f;
	}

	void Ground::update(MILLISECONDS deltaTime)
	{
		if(!m_isMoving) return;

		float dt = (deltaTime * 1.0f) / 1000.0f;
		float offset = -(m_velosity * dt);

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

		Rect2D aabb = m_sceneNodes[0]->getBoundBox();
		if(aabb._bottomRight._x <= m_screenRect._topLeft._x)
		{
			transform = transform * translate;
			m_sceneNodes[0]->setTransfrom(transform);

			SceneNode* temp = m_sceneNodes[0];
			m_sceneNodes[0] = m_sceneNodes[1];
			m_sceneNodes[1] = temp;
		}
	}

	//======================================================================================
	class Bird: public GameObject, public IMouseController
	{
	public:
		static const std::string k_name;
	public:
		Bird();

		virtual std::string getName() { return k_name; }

		virtual void onCreate(IPlatformContext* context);
		virtual void onDestroy(IPlatformContext* context);
		virtual void onCreateSceneNode(Atlas* atlas,
									   SceneManager* sceneManager,
									   const Vector3& spawnPoint);
		virtual void update(MILLISECONDS deltaTime);

		virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags);
		virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags) {}
		virtual void onMouseMove(float x, float y, MouseFlags flags) {}
		virtual void onMouseWheel(NumNothes wheel, MouseFlags flags) {}
	private:
		void impuls();
		void  updateNodePosition(float offset, bool absolute = false);
		void  setAngle(float angle);
		float interpollateAngle(float velocity);
		float checkUpBound(float offset);
		float checkDownBound(float offset);
		bool  isOnTheGround(float offset);

		enum
		{
			k_modeIdle = 0,
			k_modeFly,
			k_modeFall,
			k_modeShock,
			k_modeDead
		};

		Vector3 		m_position;
		Matrix4x4		m_size;

		float 	    	m_radius;
		float			m_groundLevel;
		float 			m_impulsVelocity;
		float			m_fallVelocity;
		float			m_impulsAngle;
		float			m_fallAngle;
		float 			m_velocity;
		float 			m_gravity;

		SceneNode* 		m_birdNode;
		ProcessPtr		m_animation;
		int 			m_mode;
	};

	const std::string Bird::k_name = "bird";

	Bird::Bird()
		:  m_radius(0.0f)
		 , m_groundLevel(0.0f)
		 , m_impulsVelocity(0.0f)
		 , m_fallVelocity(0.0f)
		 , m_impulsAngle(0.0f)
		 , m_fallAngle(0.0f)
		 , m_velocity(0.0f)
		 , m_gravity(0.0f)
		 , m_birdNode(NULL)
		 , m_mode(0)
	{

	}

	void Bird::onCreateSceneNode(Atlas* atlas,
								 SceneManager* sceneManager,
								 const Vector3& spawnPoint)
	{
		Gfx* gfx = m_context->getGFX();

		float screenWidth = gfx->getCanvasWidth();
		float screenHeight = gfx->getCanvasHeight();

		Sprite* spriteBackground = atlas->getSprite("background_day");
		Sprite* spriteGround = atlas->getSprite("ground");
		Sprite* spriteBird = atlas->getSprite("bird");
		spriteBird->setPivot(Sprite::k_pivotCenter);

		float spriteScale = screenWidth / spriteBackground->width();
		m_groundLevel = screenHeight - (spriteGround->height() * spriteScale);

		m_gravity = m_groundLevel;
		m_impulsVelocity = -(m_groundLevel / 2.0f);
		m_fallVelocity = -m_impulsVelocity;
		m_impulsAngle	= Math::PI / 6;
		m_fallAngle = -Math::PI / 2;
		m_impulsAngle = 0.0f;

		m_position._x = screenWidth * 0.3f;
		m_position._y = m_groundLevel * 0.6f;

		float spriteWidth = spriteBird->width() * spriteScale;
		float spriteHeight = spriteBird->height() * spriteScale;
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

		if(facingPoint >= m_groundLevel)
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
		if(facingPoint >= m_groundLevel)
		{
			nextPosition = m_groundLevel - m_radius;
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
		impuls();
	}

	void Bird::onCreate(IPlatformContext* context)
	{
		GameObject::onCreate(context);

		context->addMouseController(this);
	}

	void Bird::onDestroy(IPlatformContext* context)
	{
		context->removeMouseController(this);

		GameObject::onCreate(context);
	}



	//===============================================================================
	class GameWorld: public GameObject
	{
	public:
		static const std::string k_name;
	public:
		virtual std::string getName() { return k_name; }

		virtual void onCreate(IPlatformContext* context)
		{
			GameObject::onCreate(context);

			EventManager* eventManager = context->getEventManager();
			eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Background::k_name)));
			eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Ground::k_name)));
			eventManager->pushEventToQueye(EventPtr(new Event_Create_GameObject(Bird::k_name)));
		}
	};

	const std::string GameWorld::k_name = "game_world";

	//===============================================================================
	class FlappyBirdsFactory: public GameObjectFactory
	{
	public:
		virtual GameObject* create(const std::string& name)
		{
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

			return NULL;
		}
	};

	static FlappyBirdsFactory gs_game_object_factory;
}



