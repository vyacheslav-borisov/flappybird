#ifndef PEGAS_MATH_GEOMETRY_H_
#define PEGAS_MATH_GEOMETRY_H_

namespace pegas
{
	struct Point2D
	{
		Point2D() :_x(0.0f), _y(0.0f) {}
		Point2D(float x, float y): _x(x), _y(y) {}

		Point2D& operator*(float mult)
		{
			_x *= mult;
			_y *= mult;

			return (*this);
		}

		Point2D& operator/(float div)
		{
			if(div != 0.0f)
			{
				_x /= div;
				_y /= div;
			}

			return (*this);
		}

		float _x;
		float _y;
	};

	inline Point2D operator-(const Point2D& a, const Point2D& b)
	{
		return Point2D((a._x - b._x), (a._y - b._y));
	}

	inline Point2D operator+(const Point2D& a, const Point2D& b)
	{
		return Point2D((a._x + b._x), (a._y + b._y));
	}

	inline bool operator==(const Point2D& a, const Point2D& b)
	{
		return (a._x == b._x) && (a._y == b._y);
	}

	struct Rect2D
	{
		Rect2D() {}
		Rect2D(const Point2D& topLeft,
				const Point2D& bottomRight)
			:_topLeft(topLeft), _bottomRight(bottomRight)
		{

		}

		Rect2D(float x, float y, float width, float height)
			:_topLeft(x, y), _bottomRight(width, height)
		{
#ifdef PEGAS_USE_SCREEN_COORDS
			_bottomRight = _topLeft + _bottomRight;
#else
			_bottomRight._x = _topLeft._x + _bottomRight._x;
			_bottomRight._y = _topLeft._y - _bottomRight._y;
#endif
		}


		bool contains(const Point2D& point) const
		{
			bool inX = (point._x >= _topLeft._x) && (point._x <= _bottomRight._x);
#ifdef PEGAS_USE_SCREEN_COORDS
			bool inY = (point._y >= _topLeft._y) && (point._y <= _bottomRight._y);
#else
			bool inY = (point._y <= _topLeft._y) && (point._y >= _bottomRight._y);
#endif
			return (inX && inY);
		}

		bool contains(const Rect2D& rect) const
		{
			return contains(rect._topLeft) && contains(rect._bottomRight);
		}

		bool intersectsWith(const Rect2D& rect) const
		{
			const Point2D& a1 = _topLeft;
			const Point2D& a2 = _bottomRight;
			const Point2D& b1 = rect._topLeft;
			const Point2D& b2 = rect._bottomRight;

			//проверяем если левая грань первого прямоугольника находиться правее правой грании второго
			//или правая грань первого прямоугольника находиться левее левой грани первого
			//- значит прямоугольники не пересекаються по горизонтали
			bool notIntersectsHorizontal = (a1._x > b2._x) || (a2._x < b1._x);
			//Проверяем если верхняя грань первого прямоугольника находится ниже второго,
			//или нижняя грань верхнего прямоугольника выше верхней  грани первого.
			//- значит не пересекаются по вертикали
#ifdef PEGAS_USE_SCREEN_COORDS
			bool notIntersectsVertical = (a1._y > b2._y) || (a2._y < b1._y);
#else
			bool notIntersectsVertical = (a1._y < b2._y) || (a2._y > b1._y);
#endif
			bool intersects = !(notIntersectsHorizontal || notIntersectsVertical);

			return intersects;
		}

		float width() const
		{
			Point2D ptSize = _bottomRight - _topLeft;

			return std::abs(ptSize._x);
		}

		float height() const
		{
			Point2D ptSize = _bottomRight - _topLeft;

			return std::abs(ptSize._y);
		}

		Point2D _topLeft;
		Point2D _bottomRight;
	};
}



#endif /* MATH_GEOMETRY_H_ */
