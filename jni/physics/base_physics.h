#ifndef PEGAS_PHUSICS_COLLISION_CHECKER_H
#define PEGAS_PHUSICS_COLLISION_CHECKER_H
#pragma once

#include "../core/includes.h"
#include "../gfx/gfx.h"

#include "physics.h"
#include "cell_grid.h"

namespace pegas
{
	class BasePhysics: public IPhysics
	{
	public:
		BasePhysics();
		virtual ~BasePhysics();

		virtual void create(const Rect2D& worldSize);
		virtual void destroy();

		virtual void setCollisionGroupFlag(int32 group, bool checkCollisions) {}
		virtual void setCollisionPairGroupFlag(int32 groupA, int32 groupB, bool checkCollisions) {}

		virtual bool registerPoint(int32 id, int32 group, const Vector3& position);
		virtual bool registerCircle(int32 id, int32 group, const Vector3& position, float radius);
		virtual bool registerPoligon(int32 id, int32 group, const PointList& points);
		virtual void unregisterCollisionHull(int32 id);
		
		virtual void moveObject(int32 id, const Vector3& offset, bool absolute = true);
		virtual void rotateObject(int32 id, float degreesOffset, bool absolute = true);
		virtual void transformObject(int32 id, const Matrix4x4& m);
		
		virtual void update();
		virtual CollisionPairList& getCollidedPairs();

		virtual bool isIntersects(ICollisionHull* a, ICollisionHull* b);

		virtual void debugDraw(Gfx* gfx);

	private:
		CellGrid<ICollisionHull*> m_cellGrid;
		CollisionHullMap m_collisionHulls;
		CollisionPairsHashes m_previousCollisionPairs;
		CollisionPairList m_pairs;

		typedef bool (*IntersectsChecker)(ICollisionHull* a, ICollisionHull* b);
		IntersectsChecker m_checkers[ICollisionHull::k_typeTotal][ICollisionHull::k_typeTotal];
	};

	class BasePhysics2: public IPhysics
	{
	public:
		enum
		{
			k_numCollisionGroups = 10
		};

	public:
		BasePhysics2();
		virtual ~BasePhysics2();

		virtual void create(const Rect2D& worldSize);
		virtual void destroy();

		virtual void setCollisionGroupFlag(int32 group, bool checkCollisions);
		virtual void setCollisionPairGroupFlag(int32 groupA, int32 groupB, bool checkCollisions);

		virtual bool registerPoint(int32 id, int32 group, const Vector3& position);
		virtual bool registerCircle(int32 id, int32 group, const Vector3& position, float radius);
		virtual bool registerPoligon(int32 id, int32 group, const PointList& points);
		virtual void unregisterCollisionHull(int32 id);

		virtual void moveObject(int32 id, const Vector3& offset, bool absolute = true);
		virtual void rotateObject(int32 id, float degreesOffset, bool absolute = true);
		virtual void transformObject(int32 id, const Matrix4x4& m);

		virtual void update();
		virtual CollisionPairList& getCollidedPairs();

		virtual bool isIntersects(ICollisionHull* a, ICollisionHull* b);
		virtual void debugDraw(Gfx* gfx);

	private:
		class KeyGenPolicy
		{
		public:
			int32 getKeyFromObject(const IPhysics::CollisionHullPtr& object)
			{
				return object->getId();
			}
		};
		typedef QuadTree<IPhysics::CollisionHullPtr, int32, KeyGenPolicy> CHQuadTree;
		typedef bool (*IntersectsChecker)(ICollisionHull* a, ICollisionHull* b);

		CHQuadTree m_quadTree;
		IntersectsChecker m_checkers[ICollisionHull::k_typeTotal][ICollisionHull::k_typeTotal];
		CollisionHullMap m_collisionHulls;
		CollisionPairsHashes m_previousCollisionPairs;
		CollisionPairList m_pairs;

		bool m_collisionGroupFlags[k_numCollisionGroups];
		bool m_collisionGroupMatrix[k_numCollisionGroups][k_numCollisionGroups];

		bool m_initialized;
	};


}

#endif
