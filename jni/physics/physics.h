/*
 * physics.h
 *
 *  Created on: 8 февр. 2017 г.
 *      Author: merelalkar
 */

#ifndef PHYSICS_PHYSICS_H_
#define PHYSICS_PHYSICS_H_
#pragma once

#include "../core/includes.h"
#include "../gfx/gfx.h"

namespace pegas
{
	class ICollisionHull
	{
	public:
		enum
		{
			k_typePoint = 0,
			k_typeCircle,
			k_typePolygon,
			k_typeTotal
		};
	public:
		ICollisionHull(int32 id, int32 collisionGroup):
		  m_id(id), m_collisionGroup(collisionGroup) {}
		virtual ~ICollisionHull() {}

		int32 getId() const { return m_id; }
		int32 getCollisionGroup() const { return m_collisionGroup; }
		virtual int32 getType() = 0;

		virtual void moveObject(const Vector3& offset, bool absolute) = 0;
		virtual void rotateObject(float degreesOffset, bool absolute) = 0;
		virtual void transformObject(const Matrix4x4& m) = 0;

		virtual Vector3 getPosition() = 0;
		virtual Rect2D getAABB()= 0;
		virtual void draw(Gfx* gfx) = 0;

	protected:
		int32 m_id;
		int32 m_collisionGroup;
	};

	class IPhysics
	{
	public:
		typedef std::vector<Vector3> PointList;
		typedef std::pair<int32, int32> CollisionPair;
		typedef std::list<CollisionPair> CollisionPairList;
		typedef CollisionPairList::iterator CollisionPairListIt;

		typedef ptr<ICollisionHull> CollisionHullPtr;
		typedef std::map<int32, CollisionHullPtr> CollisionHullMap;
		typedef std::set<int32> CollisionPairsHashes;

	public:
		virtual ~IPhysics() { }

		//установка флага для коллизинной группы
		//если флаг установлен то все объекты из этой группы будут проверяться на столкновения с соседями
		//в целях оптимизации рекомендуеться устанавливать флаг только для движущихся динамических объектов
		//(персонажей, снарядов и т.д.) и сбрасывать для статической геометрии вроде стен
		virtual void setCollisionGroupFlag(int32 group, bool checkCollisions) = 0;
		//установка флага для сочетания коллизионных групп
		//если установлен то объекты из данного сочетания групп коллизий будут проверяться на столкновения
		virtual void setCollisionPairGroupFlag(int32 groupA, int32 groupB, bool checkCollisions) = 0;

		virtual void create(const Rect2D& worldSize) = 0;
		virtual void destroy() = 0;
		virtual bool registerPoint(int32 id, int32 group, const Vector3& position) = 0;
		virtual bool registerCircle(int32 id, int32 group, const Vector3& position, float radius) = 0;
		virtual bool registerPoligon(int32 id, int32 group, const PointList& points) = 0;
		virtual void unregisterCollisionHull(int32 id) = 0;

		virtual void moveObject(int32 id, const Vector3& offset, bool absolute = true) = 0;
		virtual void rotateObject(int32 id, float degreesOffset, bool absolute = true) = 0;
		virtual void transformObject(int32 id, const Matrix4x4& m) = 0;

		virtual void update() = 0;
		virtual CollisionPairList& getCollidedPairs() = 0;
		virtual bool isIntersects(ICollisionHull* a, ICollisionHull* b) = 0;
		virtual void debugDraw(Gfx* gfx) = 0;
	};
}


#endif /* PHYSICS_PHYSICS_H_ */
