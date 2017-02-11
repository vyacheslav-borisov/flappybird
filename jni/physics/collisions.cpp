#include "../common.h"
#include "collisions.h"

namespace pegas
{
	//------------------------------------------------------------------------------------------------
	//	PointCollisionHull class implementation
	//------------------------------------------------------------------------------------------------
	PointCollisionHull::PointCollisionHull(int32 id, int32 group, const Vector3& position)
		:ICollisionHull(id, group), m_initialPosition(position), m_currentPosition(position)
	{

	}

	void PointCollisionHull::moveObject(const Vector3& offset, bool absolute)
	{
		m_currentPosition = absolute ? (m_initialPosition + offset) : (m_currentPosition + offset);
	}

	void PointCollisionHull::rotateObject(float degreesOffset, bool absolute)
	{
		Matrix4x4 mat;

		mat.identity();
		mat.rotateZ(degreesOffset);

		m_currentPosition = absolute ? (m_initialPosition * mat) : (m_currentPosition * mat);
	}

	void PointCollisionHull::transformObject(const Matrix4x4& m)
	{
		m_currentPosition = m_initialPosition * m;
	}

	Vector3 PointCollisionHull::getPosition()
	{
		return m_currentPosition;
	}

	Rect2D PointCollisionHull::getAABB()
	{
		return Rect2D();
	}

	//------------------------------------------------------------------------------------------------
	//	CircleCollisionHull class imlementation
	//-------------------------------------------------------------------------------------------------
	CircleCollisionHull::CircleCollisionHull(int32 id, int32 group, const Vector3& position, float radius)
		:PointCollisionHull(id, group, position), m_radius(radius)
	{

	}

	Rect2D CircleCollisionHull::getAABB()
	{
		Point2D topLeft(m_currentPosition._x - m_radius, m_currentPosition._y - m_radius);
		Point2D bottomRight(m_currentPosition._x + m_radius, m_currentPosition._y + m_radius);

		return Rect2D(topLeft, bottomRight);
	}

	void CircleCollisionHull::draw(Gfx* gfx)
	{
		//TODO: CircleCollisionHull::draw
		/*
		CURCOORD left, top, width, height;
		RGBCOLOR color = 0xffff0000;

		width = height = m_radius * 2;
		left = m_currentPosition._x - m_radius;
		top = m_currentPosition._y - m_radius;

		graph.drawEllipse(left, top, width, height, color, 0x00000000);*/
	}

	//---------------------------------------------------------------------------------------------------
	//	PoligonCollisionHull class implementation
	//---------------------------------------------------------------------------------------------------
	PoligonCollisionHull::PoligonCollisionHull(int32 id, int32 group, const IPhysics::PointList& points)
		:ICollisionHull(id, group), m_initalPoints(points.begin(), points.end()), m_currentPoints(points.begin(), points.end())
	{
		for(int i = 0; i < m_currentPoints.size(); i++)
		{
			m_currentPosition = m_currentPosition + m_currentPoints[i];
		}
		m_currentPosition = m_currentPosition / m_currentPoints.size();
		m_initialPosition = m_currentPosition;
	}

	void PoligonCollisionHull::moveObject(const Vector3& offset, bool absolute)
	{
		for(int i = 0; i < m_currentPoints.size(); i++)
		{
			m_currentPoints[i] = absolute ? (m_initalPoints[i] + offset) : (m_currentPoints[i] + offset);
		}

		m_currentPosition = absolute ? (m_initialPosition + offset) : (m_currentPosition + offset);
	}

	void PoligonCollisionHull::rotateObject(float degreesOffset, bool absolute)
	{
		Matrix4x4 mat;

		mat.identity();
		mat.rotateZ(degreesOffset);

		for(int i = 0; i < m_currentPoints.size(); i++)
		{
			m_currentPoints[i] = absolute ? (m_initalPoints[i] * mat) : (m_currentPoints[i] * mat);
		}

		m_currentPosition = absolute ? (m_initialPosition * mat) : (m_currentPosition * mat);
	}

	void PoligonCollisionHull::transformObject(const Matrix4x4& m)
	{
		for(int i = 0; i < m_currentPoints.size(); i++)
		{
			m_currentPoints[i] = m_initalPoints[i] * m;
		}

		m_currentPosition = m_initialPosition * m;
	}

	Vector3 PoligonCollisionHull::getPosition()
	{
		return m_currentPosition;
	}

	Rect2D PoligonCollisionHull::getAABB()
	{
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();

		for(IPhysics::PointList::iterator it = m_currentPoints.begin();
			it != m_currentPoints.end(); ++it)
		{
			float x = (*it)._x;
			float y = (*it)._y;

			minX = (minX > x) ? x : minX;
			minY = (minY > y) ? y : minY;
			maxX = (maxX < x) ? x : maxX;
			maxY = (maxY < y) ? y : maxY;
		}

		return Rect2D(Point2D(minX, minY), Point2D(maxX, maxY));
	}

	void PoligonCollisionHull::draw(Gfx* gfx)
	{
		//TODO: PoligonCollisionHull::draw
		/*
		CURCOORD fromX, fromY, toX, toY;
		RGBCOLOR color = 0xffff0000;

		for(int32 i = 1; i < m_currentPoints.size(); i++)
		{
			fromX = m_currentPoints[i - 1]._x;
			fromY = m_currentPoints[i - 1]._y;
			toX = m_currentPoints[i]._x;
			toY = m_currentPoints[i]._y;

			graph.drawLine(fromX, fromY, toX, toY, color);
		}

		int32 iLast = m_currentPoints.size() - 1;

		fromX = m_currentPoints[0]._x;
		fromY = m_currentPoints[0]._y;
		toX = m_currentPoints[iLast]._x;
		toY = m_currentPoints[iLast]._y;

		graph.drawLine(fromX, fromY, toX, toY, color);*/
	}

	//-----------------------------------------------------------------------------------------------
	//	Collision checkers
	//-----------------------------------------------------------------------------------------------
	bool Intersections::isIntersectsPointCircle(ICollisionHull* point, ICollisionHull* circle)
	{
		PointCollisionHull* pointCH = dynamic_cast<PointCollisionHull*>(point);
		CircleCollisionHull* circleCH = dynamic_cast<CircleCollisionHull*>(circle);

		Vector3 dv = pointCH->getPosition() - circleCH->getPosition();
		float distance = dv.length();

		return (distance <= circleCH->getRadius());
	}

	bool Intersections::isIntersectsCirclePoint(ICollisionHull* circle, ICollisionHull* point)
	{
		return isIntersectsPointCircle(point, circle);
	}

	bool Intersections::isIntersectsPointPolygon(ICollisionHull* point, ICollisionHull* polygon)
	{
		PointCollisionHull* pointCH = dynamic_cast<PointCollisionHull*>(point);
		PoligonCollisionHull* poligonCH = dynamic_cast<PoligonCollisionHull*>(polygon);

		Vector3 position = pointCH->getPosition();
		IPhysics::PointList points = poligonCH->getPoints();

		int i0, i1;
		float A, B, C, D;
		Vector3 P0, P1;
		for(int i = 0; i < points.size(); i++)
		{
			i0 = i;
			i1 = (i == (points.size() - 1)) ? 0 : i + 1;

			P0 = points[i0];
			P1 = points[i1];

			A = P0._y - P1._y;
			B = P1._x - P0._x;
			C = (P0._x * P1._y) - (P1._x * P0._y);
			D =  (A * position._x) + (B * position._y) + C;

			if(D > 0)
			{
				return false;
			}
		}

		return true;
	}

	bool Intersections::isIntersectsPolygonPoint(ICollisionHull* polygon, ICollisionHull* point)
	{
		return isIntersectsPointPolygon(point, polygon);
	}

	bool Intersections::isIntersectsCirclePolygon(ICollisionHull* circle, ICollisionHull* polygon)
	{
		CircleCollisionHull* circleCH = dynamic_cast<CircleCollisionHull*>(circle);
		PoligonCollisionHull* poligonCH = dynamic_cast<PoligonCollisionHull*>(polygon);
		Vector3 position = circleCH->getPosition();
		Vector3 dotPosition = position;
		dotPosition._z = 1.0f;

		float radius = circleCH->getRadius();
		IPhysics::PointList points = poligonCH->getPoints();

		for(int i = 0; i < points.size(); i++)
		{
			Vector3 vDistance = position - points[i];
			float distance = vDistance.length();
			if(distance < radius)
			{
				return true;
			}

			Vector3 p1 = points[i];
			Vector3 p2 = (i == points.size() - 1) ? points[0] : points[i + 1];

			Vector3 line;
			line._x = p1._y - p2._y; //A
			line._y = p2._x - p1._x; //B
			line._z = p1._x * (p2._y - p1._y) - p1._y * (p2._x - p1._x); //C

			Vector3 line2(line._x, line._y, 0.0f);
			float denominator = line2.length();
			if(denominator == 0.0f) denominator = 1.0f;

			float deviation = line.dotProduct(dotPosition);
			deviation = std::abs(deviation) / denominator;
			if(deviation < radius)
			{
				Vector3 v1 = position - p1;
				Vector3 v2 = position - p2;
				Vector3 l0 = p1 - p2;

				float dot1 = l0.dotProduct(v1);
				float dot2 = l0.dotProduct(v2);
				if((dot1 * dot2) <= 0.0f)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool Intersections::isIntersectsPolygonCircle(ICollisionHull* polygon, ICollisionHull* circle)
	{
		return isIntersectsCirclePolygon(circle, polygon);
	}

	bool Intersections::isIntersectsPointPoint(ICollisionHull* point1, ICollisionHull* point2)
	{
		PointCollisionHull* pointCH1 = dynamic_cast<PointCollisionHull*>(point1);
		PointCollisionHull* pointCH2 = dynamic_cast<PointCollisionHull*>(point2);

		Vector3 p1 = pointCH1->getPosition();
		Vector3 p2 = pointCH2->getPosition();

		const float epsilon = 0.001;
		bool b1 = abs(p1._x - p2._x) < epsilon;
		bool b2 = abs(p1._y - p2._y) < epsilon;

		return (b1 && b2);
	}

	bool Intersections::isIntersectsCircleCircle(ICollisionHull* circle1, ICollisionHull* circle2)
	{
		CircleCollisionHull* circleCH1 = dynamic_cast<CircleCollisionHull*>(circle1);
		CircleCollisionHull* circleCH2 = dynamic_cast<CircleCollisionHull*>(circle2);

		Vector3 p1 = circleCH1->getPosition();
		Vector3 p2 = circleCH2->getPosition();
		float r1 = circleCH1->getRadius();
		float r2 = circleCH2->getRadius();

		Vector3 dv = p1 - p2;
		float distance = dv.length();

		return (distance < (r1 + r2));
	}

	bool Intersections::isIntersectsPolygonPolygon(ICollisionHull* polygon1, ICollisionHull* polygon2)
	{
		PoligonCollisionHull* poligonCH1 = dynamic_cast<PoligonCollisionHull*>(polygon1);
		PoligonCollisionHull* poligonCH2 = dynamic_cast<PoligonCollisionHull*>(polygon2);

		IPhysics::PointList points1 = poligonCH1->getPoints();
		IPhysics::PointList points2 = poligonCH2->getPoints();

		std::vector<float> A1(points1.size());
		std::vector<float> B1(points1.size());
		std::vector<float> C1(points1.size());

		std::vector<float> A2(points2.size());
		std::vector<float> B2(points2.size());
		std::vector<float> C2(points2.size());

		int i0, i1;
		float D;
		Vector3 P0, P1;

		for(int i = 0; i < points1.size(); i++)
		{
			i0 = i;
			i1 = (i == (points1.size() - 1)) ? 0 : i + 1;

			P0 = points1[i0];
			P1 = points1[i1];

			A1[i] = P0._y - P1._y;
			B1[i] = P1._x - P0._x;
			C1[i] = (P0._x * P1._y) - (P1._x * P0._y);
		}

		for(int i = 0; i < points2.size(); i++)
		{
			i0 = i;
			i1 = (i == (points2.size() - 1)) ? 0 : i + 1;

			P0 = points2[i0];
			P1 = points2[i1];

			A2[i] = P0._y - P1._y;
			B2[i] = P1._x - P0._x;
			C2[i] = (P0._x * P1._y) - (P1._x * P0._y);
		}

		//cheking 1 against 2
		for(int i = 0; i < points1.size(); i++)
		{
			for(int j = 0; j < points2.size(); j++)
			{
				P0 = points1[i];
				D = (P0._x * A2[j]) + (P0._y * B2[j]) + C2[j];
				if(D > 0)
				{
					return true;
				}
			}
		}//cheking 1 against 2

		//cheking 2 against 1
		for(int i = 0; i < points2.size(); i++)
		{
			for(int j = 0; j < points1.size(); j++)
			{
				P0 = points2[i];
				D = (P0._x * A1[j]) + (P0._y * B1[j]) + C1[j];
				if(D > 0)
				{
					return true;
				}
			}
		}//cheking 2 against 1

		return false;
	}
}

