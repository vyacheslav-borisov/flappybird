/*
 * scene_2D.h
 *
 *  Created on: 20 дек. 2015 г.
 *      Author: Borisov
 */

#ifndef PEGAS_SCENE_2D_H_
#define PEGAS_SCENE_2D_H_

#include "../core/includes.h"

namespace pegas
{
	class Gfx;

	class SceneNode;
	class SceneNodeEventListener
	{
	public:
		virtual ~SceneNodeEventListener() {}

		virtual void onTransfromChanged(SceneNode* sender) = 0;
		virtual void onNodeRemoved(SceneNode* sender) = 0;
	};

	template<typename T, typename K>
	class SceneNodeKeyGenPolicy
	{
	public:
		K getKeyFromObject(const T& object)
		{
			return const_cast<K>(object);
		}
	};

	class SceneNode
	{
	public:
		SceneNode(SceneNode* parentNode = NULL);
		virtual ~SceneNode();

		SceneNode* getParentNode();
		void attachChild(SceneNode* childNode);
		void removeChild(SceneNode* childNode, bool deleteChild = false);
		void removeAllChilds(bool deleteChild = false);

		void setListener(SceneNodeEventListener* listener);
		SceneNodeEventListener* getListener();

		virtual void setTransfrom(const Matrix4x4& transform);
		virtual Matrix4x4  getLocalTransform();
		virtual Matrix4x4  getWorldTransfrom();

		void setZIndex(float zIndex) { m_zIndex = zIndex; }
		float getZIndex() const { return m_zIndex; }

		virtual void render(Gfx* gfx);
		virtual Rect2D getBoundBox();
	private:
		typedef std::list<SceneNode*> ChildNodeList;
		typedef ChildNodeList::iterator ChildNodeListIt;

		SceneNode* m_parentNode;
		SceneNodeEventListener* m_listener;
		ChildNodeList m_childsNodes;
		Matrix4x4 m_transform;
		float	  m_zIndex;

	private:
		SceneNode(const SceneNode& other);
		SceneNode& operator=(const SceneNode& other);
	};

	class SceneManager: public SceneNodeEventListener
	{
	public:
		SceneManager();
		virtual ~SceneManager();

		void create(const Rect2D& worldArea);
		void destroy();

		SceneNode* getRootNode();
		void render(Gfx* gfx, const Rect2D& rect);
		void query(const Rect2D& rect, std::list<SceneNode*>& result);
		void query(const Point2D& point, std::list<SceneNode*>& result);

		virtual void onTransfromChanged(SceneNode* sender);
		virtual void onNodeRemoved(SceneNode* sender);
	private:
		typedef QuadTree<SceneNode*, SceneNode*,
				SceneNodeKeyGenPolicy<SceneNode*, SceneNode*> > SMQuadTree;

		SMQuadTree m_quadTree;
		SceneNode  m_rootNode;

	private:
		SceneManager(const SceneManager& other);
		SceneManager& operator=(const SceneManager& other);
	};
}

#endif /* SCENE_2D_H_ */
