#include "../common.h"
#include "../system/includes.h"

#include "scene.h"
#include "../core/includes.h"

namespace pegas
{
	//-----------------------------------------------------------------------------
	//	SceneManager class implementation
	//-----------------------------------------------------------------------------
	SceneManager::SceneManager()
		:m_quadTree()
	{
		LOGI("SceneManager constructor");
	}

	SceneManager::~SceneManager()
	{
		LOGI("SceneManager destructor");

		destroy();
	}

	void SceneManager::create(const Rect2D& worldArea)
	{
		LOGI("SceneManager::create [worldArea: x1 = %0.2f, y1 = %0.2f, x2 = %0.2f, y2 = %0.2f]",
				worldArea._topLeft._x, worldArea._topLeft._y,
				worldArea._bottomRight._x, worldArea._bottomRight._y);

		LOGI("setup listener");
		m_rootNode.setListener(this);

		LOGI("setup quad tree");
		m_quadTree.create(worldArea);
	}

	void SceneManager::destroy()
	{
		LOGI("SceneManager::destroy");

		m_quadTree.destroy();
		m_rootNode.removeAllChilds(true);
	}

	SceneNode* SceneManager::getRootNode()
	{
		return &m_rootNode;
	}

	void SceneManager::render(Gfx* gfx, const Rect2D& rect)
	{
		std::list<SceneNode*> nodesToRender;
		m_quadTree.query(rect, nodesToRender);

		LOGI_LOOP("nodes to render = %d", nodesToRender.size());

		for(std::list<SceneNode*>::iterator it = nodesToRender.begin();
				it != nodesToRender.end(); ++it)
		{
			(*it)->render(gfx);
		}
	}

	void SceneManager::query(const Rect2D& rect, std::list<SceneNode*>& result)
	{
		m_quadTree.query(rect, result);
	}

	void SceneManager::query(const Point2D& point, std::list<SceneNode*>& result)
	{
		m_quadTree.query(point, result);
	}

	void SceneManager::onTransfromChanged(SceneNode* sender)
	{
		LOGI("SceneManager::onTransfromChanged [sender = 0x%X]");

		Rect2D newAABB = sender->getBoundBox();
		LOGI("AABB: x1: %.2f, y1: %.2f, x2: %.2f, y2: %.2f", newAABB._topLeft._x,
				newAABB._topLeft._y, newAABB._bottomRight._x, newAABB._bottomRight._y);

		bool r1 = m_quadTree.removeObject(sender);
		if(r1)
		{
			LOGI("previous QuadTreeNode removed");
		}else
		{
			LOGI("previous QuadTreeNode not removed");
		}

		bool r2 = m_quadTree.insertObject(sender, newAABB);
		if(r2)
		{
			LOGI("new QuadTreeNode inserted");
		}else
		{
			LOGI("new QuadTreeNode not inserted");
		}
	}

	void SceneManager::onNodeRemoved(SceneNode* sender)
	{
		LOGI("SceneManager::onNodeRemoved [sender = 0x%X]", sender);

		bool r = m_quadTree.removeObject(sender);
		if(r)
		{
			LOGI("previous QuadTreeNode removed");
		}else
		{
			LOGI("previous QuadTreeNode not removed");
		}
	}

	//-----------------------------------------------------------------------------
	//	SceneNode class implementation
	//-----------------------------------------------------------------------------
	SceneNode::SceneNode(SceneNode* parentNode)
		:m_parentNode(parentNode), m_listener(NULL), m_zIndex(1.0f)
	{
		LOGI("SceneNode constructor [this: 0x%X]", this);

		m_transform.identity();
	}

	SceneNode::~SceneNode()
	{
		LOGI("SceneNode destructor [this: 0x%X]", this);

		removeAllChilds(true);
	}

	SceneNode* SceneNode::getParentNode()
	{
		return m_parentNode;
	}

	void SceneNode::attachChild(SceneNode* childNode)
	{
		LOGI("SceneNode::attachChild [this: 0x%X, child = 0x%X]", this, childNode);

		SceneNode* prevParent = childNode->m_parentNode;
		if(prevParent)
		{
			prevParent->removeChild(childNode);
		}

		ChildNodeListIt it = std::find(m_childsNodes.begin(),
				m_childsNodes.end(), childNode);

		if(it == m_childsNodes.end())
		{
			childNode->m_listener = m_listener;
			childNode->m_parentNode = this;
			m_childsNodes.push_back(childNode);
			m_listener->onTransfromChanged(childNode);
		}
	}

	void SceneNode::removeChild(SceneNode* childNode, bool deleteChild)
	{
		LOGI("SceneNode::removeChild [this: 0x%X, child = 0x%X, delete: %d]", this, childNode, deleteChild);

		ChildNodeListIt it = std::find(m_childsNodes.begin(),
						m_childsNodes.end(), childNode);

		if(it != m_childsNodes.end())
		{
			if(m_listener)
			{
				m_listener->onNodeRemoved(*it);
			}

			if(deleteChild)
			{
				delete (*it);
			}
			m_childsNodes.erase(it);
		}
	}

	void SceneNode::removeAllChilds(bool deleteChild)
	{
		LOGI("SceneNode::removeAllChilds [this: 0x%X, delete: %d]", this, deleteChild);

		for(ChildNodeListIt it = m_childsNodes.begin();
						it != m_childsNodes.end(); ++it)
		{
			if(m_listener)
			{
				m_listener->onNodeRemoved(*it);
			}

			if(deleteChild)
			{
				delete (*it);
			}
		}
		m_childsNodes.clear();
	}

	void SceneNode::setTransfrom(const Matrix4x4& transform)
	{
		m_transform = transform;

		if(m_listener)
		{
			m_listener->onTransfromChanged(this);
		}
	}

	Matrix4x4  SceneNode::getLocalTransform()
	{
		return m_transform;
	}

	Matrix4x4  SceneNode::getWorldTransfrom()
	{
		Matrix4x4 local = getLocalTransform();
		Matrix4x4 world = local;

		if(m_parentNode)
		{
			world = local * m_parentNode->getWorldTransfrom();
		}

		return world;
	}

	Rect2D SceneNode::getBoundBox()
	{
		return Rect2D();
	}

	void SceneNode::render(Gfx* gfx)
	{
		for(ChildNodeListIt it = m_childsNodes.begin();
						it != m_childsNodes.end(); ++it)
		{
			(*it)->render(gfx);
		}
	}

	void SceneNode::setListener(SceneNodeEventListener* listener)
	{
		LOGI("SceneNode::setListener [this: 0x%X, listener: 0x%X]", this, listener);

		m_listener = listener;
	}

	SceneNodeEventListener* SceneNode::getListener()
	{
		return m_listener;
	}
}




