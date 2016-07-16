#ifndef CORE_QUAD_TREE_H_
#define CORE_QUAD_TREE_H_

#include "../core/geometry.h"
#include "../system./log.h"

namespace pegas
{
	/*template<typename T, typename K>
	class DefaultObjectKeyGenPolicy
	{
	public:
		K getKeyFromObject(const T& object)
		{
			return const_cast<K>(object);
		}
	};*/

	template<typename T>
	class QuadTreeNode
	{
	public:
		QuadTreeNode();
		virtual ~QuadTreeNode();

		QuadTreeNode<T>* insertObject(const T& object, const Rect2D& objectAABB);
		bool removeObject(const T& object);
		bool removeAllObjects();
		void query(const Rect2D& objectAABB, std::list<T>& result);
		void query(const Point2D& queryPoint, std::list<T>& result);
		void query(std::list<T>& result);
		void setAABB(const Rect2D& AABB);

	private:
		template <typename E>
		struct TQuadTreeItem
		{
			TQuadTreeItem(const E& object, const Rect2D& objectAABB)
				:_object(object), _objectAABB(objectAABB)
			{

			}

			bool operator==(const TQuadTreeItem<E>& other) const
			{
				return _object == other._object;
			}

			E _object;
			Rect2D _objectAABB;
		};

		typedef TQuadTreeItem<T> QuadTreeItem;
		typedef std::list<QuadTreeItem> ObjectList;
		typedef typename ObjectList::iterator ObjectListIt;

		enum
		{
			k_childNorthWest,
			k_childNorthEast,
			k_childSouthEast,
			k_childSouthWest,
			k_childTotal
		};

		QuadTreeNode* m_childs[k_childTotal];
		Rect2D m_AABB;
		ObjectList m_objects;

	private:
		QuadTreeNode(const QuadTreeNode& other);
		QuadTreeNode& operator=(const QuadTreeNode& other);
	};

	template<typename T, typename  K, typename KeyGenPolicy>
	class QuadTree: public KeyGenPolicy
	{
		using KeyGenPolicy::getKeyFromObject;

	public:
		QuadTree();
		virtual ~QuadTree();

		void create(const Rect2D& worldArea);
		void destroy();

		bool insertObject(const T& object, const Rect2D& objectAABB);
		bool removeObject(const T& object);
		bool removeAllObjects();
		void query(const Rect2D& objectAABB, std::list<T>& result);
		void query(const Point2D& queryPoint, std::list<T>& result);

	private:
		typedef std::map<K, QuadTreeNode<T>*> ObjectNodeLookupTable;
		typedef typename ObjectNodeLookupTable::iterator ObjectNodeLookupTableIt;

		QuadTreeNode<T>* 		m_rootNode;
		ObjectNodeLookupTable   m_lookupTable;
	private:
		QuadTree(const QuadTree<T, K, KeyGenPolicy>& other);
		QuadTree& operator=(const QuadTree<T, K, KeyGenPolicy>& other);
	};

	//-----------------------------------------------------------------------------
	//	QuadTree class implementation
	//-----------------------------------------------------------------------------
	template<typename T, typename  K, typename KeyGenPolicy>
	inline QuadTree<T, K, KeyGenPolicy>::QuadTree()
		:m_rootNode(NULL)
	{
		LOGI("QuadTree constructor");
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline QuadTree<T, K, KeyGenPolicy>::~QuadTree()
	{
		LOGI("QuadTree destructor");

		destroy();
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline void QuadTree<T, K, KeyGenPolicy>::create(const Rect2D& worldArea)
	{
		LOGI("QuadTree::create [worldArea: x1  = %.2f, y1  = %.2f, x2  = %.2f, y2  = %.2f]",
				worldArea._topLeft._x, worldArea._topLeft._y,
				worldArea._bottomRight._x, worldArea._bottomRight._y);

		destroy();

		m_rootNode = new QuadTreeNode<T>();
		m_rootNode->setAABB(worldArea);
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline void QuadTree<T, K, KeyGenPolicy>::destroy()
	{
		LOGI("QuadTree::destroy");

		if(m_rootNode)
		{
			delete m_rootNode;
			m_rootNode = NULL;
		}
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline bool QuadTree<T, K, KeyGenPolicy>::insertObject(const T& object, const Rect2D& objectAABB)
	{
		LOGI("QuadTree::insertObject");
		LOGI("[objectAABB: x1 = %0.2f, y1 = %0.2f, x2 = %0.2f, y2 = %0.2f]",
				objectAABB._topLeft._x, objectAABB._topLeft._y,
				objectAABB._bottomRight._x, objectAABB._bottomRight._y);

		if(objectAABB.width() == 0.0f || objectAABB.height() == 0.0f)
		{
			LOGE("could not insert null sized object");
			return false;
		}

		if(m_rootNode)
		{
			QuadTreeNode<T>* node = m_rootNode->insertObject(object, objectAABB);
			if(node)
			{
				K key = getKeyFromObject(object);
				if(m_lookupTable.count(key) == 0)
				{
					m_lookupTable[key] = node;
					return true;
				}
			}
		}

		return false;
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline bool QuadTree<T, K, KeyGenPolicy>::removeObject(const T& object)
	{
		K key = getKeyFromObject(object);
		if(m_lookupTable.count(key) > 0)
		{
			QuadTreeNode<T>* node = m_lookupTable[key];
			bool result = node->removeObject(object);
			m_lookupTable.erase(key);

			return result;
		}

		return false;
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline bool QuadTree<T, K, KeyGenPolicy>::removeAllObjects()
	{
		if(m_rootNode)
		{
			return m_rootNode->removeAllObjects();
		}

		return false;
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline void QuadTree<T, K, KeyGenPolicy>::query(const Rect2D& objectAABB, std::list<T>& result)
	{
		if(m_rootNode)
		{
			m_rootNode->query(objectAABB, result);
		}
	}

	template<typename T, typename  K, typename KeyGenPolicy>
	inline void QuadTree<T, K, KeyGenPolicy>::query(const Point2D& queryPoint, std::list<T>& result)
	{
		if(m_rootNode)
		{
			m_rootNode->query(queryPoint, result);
		}
	}


	//----------------------------------------------------------------------------
	//  QuadTreeNode class implementation
	//----------------------------------------------------------------------------

	template<typename T>
	inline QuadTreeNode<T>::QuadTreeNode()
	{
		LOGI("QuadTreeNode constructor [this: 0x%X]", this);

		for(int i = 0; i < k_childTotal; i++)
		{
			m_childs[i] = NULL;
		}
	}

	template<typename T>
	inline QuadTreeNode<T>::~QuadTreeNode()
	{
		LOGI("QuadTreeNode destructor [this: 0x%X]", this);

		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i]) delete m_childs[i];
		}
	}

	template<typename T>
	void QuadTreeNode<T>::setAABB(const Rect2D& AABB)
	{
		LOGI("QuadTreeNode<T>::setAABB [this: 0x%X]", this);
		LOGI("[AABB: x1 = %0.2f, y1 = %0.2f, x2 = %0.2f, y2 = %0.2f]",
				AABB._topLeft._x, AABB._topLeft._y,
				AABB._bottomRight._x, AABB._bottomRight._y);

		m_AABB = AABB;

		if(m_childs[0] != NULL)
		{
			Point2D half = (AABB._bottomRight - AABB._topLeft) / 2;
			Point2D center = AABB._topLeft + half;
			Point2D centerLeftEdge(AABB._topLeft._x, center._y);
			Point2D centerRightEdge(AABB._bottomRight._x, center._y);
			Point2D centerUpEdge(center._x, AABB._topLeft._y);
			Point2D centerBottomEdge(center._x, AABB._bottomRight._y);

			Rect2D childAABB;

			LOGI("setup AABB for NorthWest child node");
			childAABB._topLeft = AABB._topLeft;
			childAABB._bottomRight = center;
			m_childs[k_childNorthWest]->setAABB(childAABB);

			LOGI("setup AABB for NorthEast child node");
			childAABB._topLeft = centerUpEdge;
			childAABB._bottomRight = centerRightEdge;
			m_childs[k_childNorthEast]->setAABB(childAABB);

			LOGI("setup AABB for SouthEast child node");
			childAABB._topLeft = center;
			childAABB._bottomRight = AABB._bottomRight;
			m_childs[k_childSouthEast]->setAABB(childAABB);

			LOGI("setup AABB for SouthWest child node");
			childAABB._topLeft = centerLeftEdge;
			childAABB._bottomRight = centerBottomEdge;
			m_childs[k_childSouthWest]->setAABB(childAABB);
		}
	}

	template<typename T>
	inline QuadTreeNode<T>* QuadTreeNode<T>::insertObject(const T& object, const Rect2D& objectAABB)
	{
		LOGI("QuadTreeNode<T>::insertObject [this: 0x%X]", this);
		LOGI("[objectAABB: x1 = %0.2f, y1 = %0.2f, x2 = %0.2f, y2 = %0.2f]",
				objectAABB._topLeft._x, objectAABB._topLeft._y,
				objectAABB._bottomRight._x, objectAABB._bottomRight._y);

		if(!m_AABB.contains(objectAABB))
		{
			LOGW("!m_AABB.contains(objectAABB)");

			return NULL;
		}

		if(m_childs[0] == NULL)
		{
			LOGI("creating child nodes");

			for(int i = 0; i < k_childTotal; i++)
			{
				m_childs[i] = new QuadTreeNode<T>();
			}
			setAABB(m_AABB);
		}

		LOGI("try insert object into child nodes");
		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i]->insertObject(object, objectAABB))
			{
				return m_childs[i];
			}
		}

		QuadTreeItem item(object, objectAABB);
		ObjectListIt foundIt = std::find(m_objects.begin(), m_objects.end(), item);
		if(foundIt == m_objects.end())
		{
			LOGI("inserting object to node 0x%X", this);

			m_objects.push_back(item);
			return this;
		}

		return NULL;
	}

	template<typename T>
	inline bool QuadTreeNode<T>::removeObject(const T& object)
	{
		LOGI("QuadTreeNode::removeObject [this: 0x%X]", this);

		QuadTreeItem item(object, Rect2D());
		ObjectListIt foundIt = std::find(m_objects.begin(), m_objects.end(), item);
		if(foundIt != m_objects.end())
		{
			m_objects.erase(foundIt);
			return true;
		}

		return false;
	}

	template<typename T>
	inline bool QuadTreeNode<T>::removeAllObjects()
	{
		LOGI("QuadTreeNode::removeAllObjects [this: 0x%X]", this);

		m_objects.clear();

		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i])
			{
				m_childs[i]->removeAllObjects();
			}
		}//for(int i = 0; i < k_childTotal; i++)
	}

	template<typename T>
	inline void QuadTreeNode<T>::query(const Rect2D& queryAABB, std::list<T>& result)
	{
		//LOGI("QuadTreeNode::query [this: 0x%X]", this);

		if(queryAABB.contains(m_AABB))
		{
			//область запроса целиком содержит данный узел
			//-выбираем все объекты этого и дочерних узлов без проверок
			//LOGW("queryAABB.contains(m_AABB)");

			query(result);
			return;
		}

		if(!m_AABB.intersectsWith(queryAABB) && !m_AABB.contains(queryAABB))
		{
			//область запроса не перескаеться с данным узлом
			//и не входит в данный узел - значит отсюда выбирать нечего, выходим
			//LOGW("!m_AABB.intersectsWith(queryAABB) && !m_AABB.contains(queryAABB)");

			return;
		}

		for(ObjectListIt it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			Rect2D rect = (*it)._objectAABB;
			if(queryAABB.contains(rect) //область запроса содержит данный объект - выбираем
				|| rect.contains(queryAABB) //объект такой большой, что перекрывает область запроса - выбираем
				|| queryAABB.intersectsWith(rect)) //объект частично попадает в область запроса - выбираем
			{
				result.push_back((*it)._object);
			}
		}

		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i])
			{
				m_childs[i]->query(queryAABB, result);
			}
		}//for(int i = 0; i < k_childTotal; i++)
	}

	template<typename T>
	inline void QuadTreeNode<T>::query(const Point2D& queryPoint, std::list<T>& result)
	{
		if(!m_AABB.contains(queryPoint))
		{
			return;
		}

		for(ObjectListIt it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			Rect2D rect = (*it)._objectAABB;
			if(rect.contains(queryPoint))
			{
				result.push_back((*it)._object);
			}
		}

		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i])
			{
				m_childs[i]->query(queryPoint, result);
			}
		}//for(int i = 0; i < k_childTotal; i++)
	}

	template<typename T>
	inline void QuadTreeNode<T>::query(std::list<T>& result)
	{
		for(ObjectListIt it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			result.push_back((*it)._object);
		}

		for(int i = 0; i < k_childTotal; i++)
		{
			if(m_childs[i])
			{
				m_childs[i]->query(result);
			}
		}//for(int i = 0; i < k_childTotal; i++)
	}
}



#endif /* CORE_QUAD_TREE_H_ */
