#include "../common.h"
#include "base_physics.h"
#include "collisions.h"

namespace pegas
{
	//--------------------------------------------------------------------------------------------------------
	//	BasePhysics implementation
	//--------------------------------------------------------------------------------------------------------
	BasePhysics::BasePhysics()
	{
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typePoint] = &Intersections::isIntersectsPointPoint;
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsPointCircle;
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsPointPolygon;

		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typePoint] = &Intersections::isIntersectsCirclePoint;
		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsCircleCircle;
		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsCirclePolygon;

		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typePoint] = &Intersections::isIntersectsPolygonPoint;
		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsPolygonCircle;
		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsPolygonPolygon;
	}

	BasePhysics::~BasePhysics()
	{

	}

	void BasePhysics::create(const Rect2D& worldSize)
	{
		m_cellGrid.create(worldSize.width(), worldSize.height(), 250);
	}

	void BasePhysics::destroy()
	{
		m_cellGrid.destroy();
	}

	bool BasePhysics::isIntersects(ICollisionHull* a, ICollisionHull* b)
	{
		int32 aIndex = a->getType();
		int32 bIndex = b->getType();

		return (m_checkers[aIndex][bIndex])(a, b);
	}

	bool BasePhysics::registerPoint(int32 id, int32 group, const Vector3& position)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}

		CollisionHullPtr hull = new PointCollisionHull(id, group, position);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	bool BasePhysics::registerCircle(int32 id, int32 group, const Vector3& position, float radius)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}
		
		
		CollisionHullPtr hull = new CircleCollisionHull(id, group, position, radius);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	bool BasePhysics::registerPoligon(int32 id, int32 group, const PointList& points)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}

		
		CollisionHullPtr hull = new PoligonCollisionHull(id, group, points);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	void BasePhysics::unregisterCollisionHull(int32 id)
	{
		//assert(m_collisionHulls.count(id) > 0);
		
		if(m_collisionHulls.count(id) > 0)
		{
			CollisionHullPtr hull = m_collisionHulls[id];
			m_collisionHulls.erase(id);
			m_cellGrid.removeObject(hull.get());
		}
	}
		
	void BasePhysics::moveObject(int32 id, const Vector3& offset, bool absolute)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->moveObject(offset, absolute);
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());
	}
	
	void BasePhysics::rotateObject(int32 id, float degreesOffset, bool absolute)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->rotateObject(degreesOffset, absolute);
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());
	}

	void BasePhysics::transformObject(int32 id, const Matrix4x4& m)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->transformObject(m);
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());
	}
		
	void BasePhysics::update()
	{
		std::set<int32> closedNodes;

		m_pairs.clear();

		for(CollisionHullMap::iterator it = m_collisionHulls.begin(); it != m_collisionHulls.end(); ++it)
		{
			Vector3 position = it->second->getPosition();
			Cell<ICollisionHull*>* myCell =	m_cellGrid.getCell(position); 

			ICollisionHull* a = it->second.get();
			ICollisionHull* b = 0;

			for(int32 i = 0; i < 9; i++)
			{
				Cell<ICollisionHull*>* currentCell = myCell->getSibling(i);
				if(currentCell == 0) continue;
				
				for(Cell<ICollisionHull*>::ObjectListIt iit = currentCell->begin(); iit != currentCell->end(); ++iit)
				{
					b = *iit;

					if(a == b) continue;
					if(closedNodes.count(b->getId()) > 0) continue;

					//TODO: collision groups filter
					if(a->getCollisionGroup() == b->getCollisionGroup()) continue;

					int32 id_a = a->getId();
					int32 id_b = b->getId();
					int32 hash = std::max(id_a, id_b) << 16 | std::min(id_a, id_b);

					if(isIntersects(a, b))
					{
						if(m_previousCollisionPairs.count(hash) > 0)
						{
							continue;
						}
						
						CollisionPair pair(std::max(id_a, id_b), std::min(id_a, id_b));
						m_pairs.push_back(pair);
						m_previousCollisionPairs.insert(hash);
					}else
					{
						m_previousCollisionPairs.erase(hash);
					}
					
				}//for(Cell<ICollisionHull*>::ObjectListIt iit = currentCell->begin(); iit != currentCell->end(); ++iit)
			}//for(int32 i = 0; i < 9; i++)

			closedNodes.insert(a->getId());

		}//for(CollisionHullMap::iterator it = m_collisionHulls.begin(); it != m_collisionHulls.end(); ++it)
	}
	
	BasePhysics::CollisionPairList& BasePhysics::getCollidedPairs()
	{
		return m_pairs;
	}

	void BasePhysics::debugDraw(Gfx* gfx)
	{
		for(CollisionHullMap::iterator it = m_collisionHulls.begin(); it != m_collisionHulls.end(); ++it)
		{
			it->second->draw(gfx);
		}
	}

	//===========================================================================================================
	//	BasePhysics2 implementation
	//===========================================================================================================
	BasePhysics2::BasePhysics2()
		:m_initialized(false)
	{
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typePoint] = &Intersections::isIntersectsPointPoint;
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsPointCircle;
		m_checkers[ICollisionHull::k_typePoint][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsPointPolygon;

		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typePoint] = &Intersections::isIntersectsCirclePoint;
		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsCircleCircle;
		m_checkers[ICollisionHull::k_typeCircle][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsCirclePolygon;

		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typePoint] = &Intersections::isIntersectsPolygonPoint;
		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typeCircle] = &Intersections::isIntersectsPolygonCircle;
		m_checkers[ICollisionHull::k_typePolygon][ICollisionHull::k_typePolygon] = &Intersections::isIntersectsPolygonPolygon;
	}

	BasePhysics2::~BasePhysics2()
	{

	}

	void BasePhysics2::create(const Rect2D& worldSize)
	{
		m_quadTree.create(worldSize);

		for(int i = 0; i < k_numCollisionGroups; i++)
		{
			m_collisionGroupFlags[i] = false;

			for(int j = 0; j < k_numCollisionGroups; j++)
			{
				m_collisionGroupMatrix[i][j] = false;
			}
		}

		m_initialized = true;
	}

	void BasePhysics2::destroy()
	{
		m_quadTree.destroy();
		m_initialized = false;
	}

	void BasePhysics2::setCollisionGroupFlag(int32 group, bool checkCollisions)
	{
		assert(group < k_numCollisionGroups);

		if(group < k_numCollisionGroups)
		{
			m_collisionGroupFlags[group] = checkCollisions;
		}
	}

	void BasePhysics2::setCollisionPairGroupFlag(int32 groupA, int32 groupB, bool checkCollisions)
	{
		assert(groupA < k_numCollisionGroups);
		assert(groupB < k_numCollisionGroups);

		if(groupA < k_numCollisionGroups && groupB < k_numCollisionGroups)
		{
			m_collisionGroupMatrix[groupA][groupB] = checkCollisions;
			m_collisionGroupMatrix[groupB][groupA] = checkCollisions;
		}
	}

	bool BasePhysics2::registerPoint(int32 id, int32 group, const Vector3& position)
	{
		return false;
	}

	bool BasePhysics2::registerCircle(int32 id, int32 group, const Vector3& position, float radius)
	{
		if(!m_initialized) return false;

		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}


		CollisionHullPtr hull = new CircleCollisionHull(id, group, position, radius);
		m_collisionHulls[id] = hull;

		Rect2D aabb = hull->getAABB();
		m_quadTree.insertObject(hull, aabb);

		return true;
	}

	bool BasePhysics2::registerPoligon(int32 id, int32 group, const PointList& points)
	{
		if(!m_initialized) return false;

		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}


		CollisionHullPtr hull = new PoligonCollisionHull(id, group, points);
		m_collisionHulls[id] = hull;

		Rect2D aabb = hull->getAABB();
		m_quadTree.insertObject(hull, aabb);

		return true;
	}

	void BasePhysics2::unregisterCollisionHull(int32 id)
	{
		if(!m_initialized) return;

		if(m_collisionHulls.count(id) > 0)
		{
			CollisionHullPtr hull = m_collisionHulls[id];
			m_collisionHulls.erase(id);
			m_quadTree.removeObject(hull);
		}
	}

	void BasePhysics2::moveObject(int32 id, const Vector3& offset, bool absolute)
	{
		if(!m_initialized) return;

		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->moveObject(offset, absolute);

		m_quadTree.removeObject(hull);

		Rect2D newAabb = hull->getAABB();
		m_quadTree.insertObject(hull, newAabb);
	}

	void BasePhysics2::rotateObject(int32 id, float degreesOffset, bool absolute)
	{
		if(!m_initialized) return;

		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->rotateObject(degreesOffset, absolute);

		m_quadTree.removeObject(hull);

		Rect2D newAabb = hull->getAABB();
		m_quadTree.insertObject(hull, newAabb);
	}

	void BasePhysics2::transformObject(int32 id, const Matrix4x4& m)
	{
		if(!m_initialized) return;

		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->transformObject(m);

		m_quadTree.removeObject(hull);

		Rect2D newAabb = hull->getAABB();
		m_quadTree.insertObject(hull, newAabb);
	}

	void BasePhysics2::update()
	{
		if(!m_initialized) return;

		m_pairs.clear();

		for(CollisionHullMap::iterator it = m_collisionHulls.begin();
				it != m_collisionHulls.end(); ++it)
		{
			CollisionHullPtr hullA = it->second;
			int32 groupA = hullA->getCollisionGroup();

			//оптимизация
			//пропускаем статическую геометрию и проверяем только движущиеся объекты
			//(проверяем по коллизионной группе)
			if(!m_collisionGroupFlags[groupA])
			{
				continue;
			}

			QuadTreeNode<CollisionHullPtr>* node = m_quadTree.getNodeByObject(hullA);
			while(node != NULL)
			{
				QuadTreeNode<CollisionHullPtr>::IIterator* objectIt = node->getIterator();
				while(!objectIt->end())
				{
					CollisionHullPtr hullB = objectIt->current();

					if(hullA == hullB)
					{
						objectIt->next();
						continue;
					}

					int32 groupB = hullB->getCollisionGroup();

					//оптимизация, должна ли осущствляться проверка столкновений
					//между hullA и hullB (проверяем по коллизионной группе)
					if(!m_collisionGroupMatrix[groupA][groupB])
					{
						objectIt->next();
						continue;
					}

					int32 id_a = hullA->getId();
					int32 id_b = hullB->getId();
					int32 hash = std::max(id_a, id_b) << 16 | std::min(id_a, id_b);

					if(isIntersects(hullA.get(), hullB.get()))
					{
						if(m_previousCollisionPairs.count(hash) > 0)
						{
							objectIt->next();
							continue;
						}

						CollisionPair pair(std::max(id_a, id_b), std::min(id_a, id_b));
						m_pairs.push_back(pair);
						m_previousCollisionPairs.insert(hash);
					}else
					{
						m_previousCollisionPairs.erase(hash);
					}

					objectIt->next();
				}

				node = node->getParentNode();
			}
		}
	}

	IPhysics::CollisionPairList& BasePhysics2::getCollidedPairs()
	{
		return m_pairs;
	}

	bool BasePhysics2::isIntersects(ICollisionHull* a, ICollisionHull* b)
	{
		int32 aIndex = a->getType();
		int32 bIndex = b->getType();

		return (m_checkers[aIndex][bIndex])(a, b);
	}

	void BasePhysics2::debugDraw(Gfx* gfx)
	{
		for(CollisionHullMap::iterator it = m_collisionHulls.begin(); it != m_collisionHulls.end(); ++it)
		{
			it->second->draw(gfx);
		}
	}
}
