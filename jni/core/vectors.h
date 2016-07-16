#ifndef PEGAS_VECTORS_H
#define PEGAS_VECTORS_H

namespace pegas
{
	class Vector3
	{
	public:
		union
		{
			struct
			{
				float _x, _y, _z;
			};
			float _v[3];
		};

		Vector3();
		Vector3(const Vector3& src);
		Vector3(float x, float y, float z);

		float length() const;
		float dotProduct(const Vector3& other) const;
		Vector3 crossProduct(const Vector3& other) const;
		Vector3& normalize();
		
		operator float*()
		{
			return _v;
		}

		operator const float*() const
		{
			return _v;
		}

		friend Vector3 operator+(const Vector3& a, const Vector3& b);
		friend Vector3 operator-(const Vector3& a, const Vector3& b);
		friend Vector3 operator*(float a, const Vector3& b);
		friend Vector3 operator*(const Vector3& a, float b);
		friend Vector3 operator/(const Vector3& a, float b);
	};

	inline Vector3::Vector3(): _x(0), _y(0), _z(0)
	{

	}

	inline Vector3::Vector3(const Vector3& src): _x(src._x), _y(src._y), _z(src._z)
	{

	}

	inline Vector3::Vector3(float x, float y, float z)
		: _x(x), _y(y), _z(z)
	{

	}

	inline Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3((a._x + b._x), (a._y + b._y), (a._z + b._z));
	}

	inline Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3((a._x - b._x), (a._y - b._y), (a._z - b._z));
	}

	inline Vector3 operator*(float a, const Vector3& b)
	{
		return Vector3((a * b._x), (a * b._y), (a * b._z));
	}

	inline Vector3 operator*(const Vector3& a, float b)
	{
		return Vector3((a._x * b), (a._y * b), (a._z * b));
	}

	inline Vector3 operator/(const Vector3& a, float b)
	{
		return Vector3((a._x / b), (a._y / b), (a._z / b));
	}

	inline float Vector3::length() const
	{
		return sqrt((_x * _x) + (_y * _y) + (_z * _z));
	}

	inline float Vector3::dotProduct(const Vector3& other) const
	{
		return ((_x * other._x) + (_y * other._y) + (_z * other._z));
	}

	inline Vector3 Vector3::crossProduct(const Vector3& other) const
	{
		const Vector3& v1 = *this;
		const Vector3& v2 = other;

		float i = (v1._y * v2._z) - (v2._y * v1._z);
		float j = (v1._x * v2._z) - (v2._x * v1._z);
		float k = (v1._x * v2._y) - (v2._x * v1._y);

		return Vector3(i, -j, k);
	}

	inline Vector3& Vector3::normalize()
	{
		float l = length();
		if(l != 0)
		{
			_x /= l;
			_y /= l;
			_z /= l;
		}

		return *this;
	}
}

#endif//PEGAS_VECTORS_H
