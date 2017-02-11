/*
 * collisions.h
 *
 *  Created on: 8 февр. 2017 г.
 *      Author: merelalkar
 */

#ifndef PHYSICS_COLLISIONS_H_
#define PHYSICS_COLLISIONS_H_
#pragma once

#include "physics.h"

namespace pegas
{
	class Intersections
	{
	public:
		static bool isIntersectsPointCircle(ICollisionHull* point, ICollisionHull* circle);
		static bool isIntersectsCirclePoint(ICollisionHull* circle, ICollisionHull* point);

		static bool isIntersectsPointPolygon(ICollisionHull* point, ICollisionHull* polygon);
		static bool isIntersectsPolygonPoint(ICollisionHull* polygon, ICollisionHull* point);

		static bool isIntersectsCirclePolygon(ICollisionHull* circle, ICollisionHull* polygon);
		static bool isIntersectsPolygonCircle(ICollisionHull* polygon, ICollisionHull* circle);

		static bool isIntersectsPointPoint(ICollisionHull* point1, ICollisionHull* point2);
		static bool isIntersectsCircleCircle(ICollisionHull* circle1, ICollisionHull* circle2);
		static bool isIntersectsPolygonPolygon(ICollisionHull* polygon1, ICollisionHull* polygon2);
	};

	class PointCollisionHull: public ICollisionHull
	{
	public:
		PointCollisionHull(int32 id, int32 group, const Vector3& position);

		virtual int32 getType() { return k_typePoint; }

		virtual void moveObject(const Vector3& offset, bool absolute);
		virtual void rotateObject(float degreesOffset, bool absolute);
		virtual void transformObject(const Matrix4x4& m);
		virtual Vector3 getPosition();
		virtual Rect2D getAABB();
		virtual void draw(Gfx* gfx) { }

	protected:
		Vector3 m_initialPosition;
		Vector3 m_currentPosition;
	};

	class CircleCollisionHull: public PointCollisionHull
	{
	public:
		CircleCollisionHull(int32 id, int32 group, const Vector3& position, float radius);

		virtual int32 getType() { return k_typeCircle; }
		float getRadius() const { return m_radius; }
		virtual Rect2D getAABB();
		virtual void draw(Gfx* gfx);

	protected:
		float m_radius;
	};

	class PoligonCollisionHull: public ICollisionHull
	{
	public:
		PoligonCollisionHull(int32 id, int32 group, const IPhysics::PointList& points);

		virtual int32 getType() { return k_typePolygon; }

		virtual void moveObject(const Vector3& offset, bool absolute);
		virtual void rotateObject(float degreesOffset, bool absolute);
		virtual void transformObject(const Matrix4x4& m);

		virtual Vector3 getPosition();
		virtual Rect2D getAABB();
		IPhysics::PointList getPoints() const { return m_currentPoints; }

		virtual void draw(Gfx* gfx);

	protected:
		IPhysics::PointList m_initalPoints;
		IPhysics::PointList m_currentPoints;
		Vector3 m_initialPosition;
		Vector3 m_currentPosition;
	};
}

#endif /* PHYSICS_COLLISIONS_H_ */
