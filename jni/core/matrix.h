#pragma once

#include "types.h"
#include "vectors.h"


namespace pegas
{
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const Matrix4x4& src);
		Matrix4x4(float _11, float _12, float _13, float _14,
				  float _21, float _22, float _23, float _24,
				  float _31, float _32, float _33, float _34,
				  float _41, float _42, float _43, float _44);

		Matrix4x4& identity();
		Matrix4x4& scale(float sx, float sy, float sz);
		Matrix4x4& translate(float tx, float ty, float tz);
		Matrix4x4& rotateX(float angle);
		Matrix4x4& rotateY(float angle);
		Matrix4x4& rotateZ(float angle);
		Matrix4x4& lookAt(const Vector3& eye, const Vector3& at, const Vector3& up);
		Matrix4x4& perspectiveLH(float width, float height, float zNear, float zFar);
		Matrix4x4& perspectiveRH(float width, float height, float zNear, float zFar);
		Matrix4x4& perspectiveFovLH(float FOV, float aspect, float zNear, float zFar);
		Matrix4x4& perspectiveFovRH(float FOV, float aspect, float zNear, float zFar);
		Matrix4x4& orthoLH(float width, float height, float zNear, float zFar);
		Matrix4x4& orthoRH(float width, float height, float zNear, float zFar);

		friend Matrix4x4 operator+(const Matrix4x4& a, const Matrix4x4& b);
		friend Matrix4x4 operator-(const Matrix4x4& a, const Matrix4x4& b);
		friend Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
		friend Vector3 operator*(const Vector3& a, const Matrix4x4& b);
		friend Matrix4x4 operator*(float a, const Matrix4x4& b);
		friend Matrix4x4 operator*(const Matrix4x4& a, float b);

		operator float*()
		{
			return _v;
		}

		operator const float*() const
		{
			return _v;
		}

	public:
		union
		{
			struct {
				float _11, _12, _13, _14,
					  _21, _22, _23, _24,
					  _31, _32, _33, _34,
					  _41, _42, _43, _44;
			};
			float _m[4][4];
			float _v[16];
		};
	};

	/************************************************************************************************
		class operations
	*************************************************************************************************/
	inline Matrix4x4::Matrix4x4()
	{
		memset(_m, 0, sizeof(float) * 16);
	}
	
	inline Matrix4x4::Matrix4x4(const Matrix4x4& src)
	{
		memcpy(_m, src._m, sizeof(float) * 16);
	}
		
	inline Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
								float _21, float _22, float _23, float _24,
								float _31, float _32, float _33, float _34,
								float _41, float _42, float _43, float _44)
	{
		_m[0][0] = _11;
		_m[0][1] = _12;
		_m[0][2] = _13;
		_m[0][3] = _14;

		_m[1][0] = _21;
		_m[1][1] = _22;
		_m[1][2] = _23;
		_m[1][3] = _24;

		_m[2][0] = _31;
		_m[2][1] = _32;
		_m[2][2] = _33;
		_m[2][3] = _34;

		_m[3][0] = _41;
		_m[3][1] = _42;
		_m[3][2] = _43;
		_m[3][3] = _44;
	}

		
	inline Matrix4x4& Matrix4x4::identity()
	{
		memset(_m, 0, sizeof(float) * 16);

		_m[0][0] = 1.0f;
		_m[1][1] = 1.0f;
		_m[2][2] = 1.0f;
		_m[3][3] = 1.0f;

		return (*this);
	}
		
	inline Matrix4x4& Matrix4x4::scale(float sx, float sy, float sz)
	{
		_m[0][0] = sx;
		_m[1][1] = sy;
		_m[2][2] = sz;
		
		return (*this);
	}
	
	inline Matrix4x4& Matrix4x4::translate(float tx, float ty, float tz)
	{
		_m[3][0] = tx;
		_m[3][1] = ty;
		_m[3][2] = tz;
		
		return (*this);
	}

	inline Matrix4x4& Matrix4x4::rotateX(float angle)
	{
		_m[1][1] = cos(angle);
		_m[1][2] = -sin(angle);
		_m[2][1] = sin(angle);
		_m[2][2] = cos(angle);

		return (*this);
	}
		
	inline Matrix4x4& Matrix4x4::rotateY(float angle)
	{
		_m[0][0] = cos(angle);
		_m[0][2] = sin(angle);
		_m[2][0] = -sin(angle);
		_m[2][2] = cos(angle);

		return (*this);
	}

	inline	Matrix4x4& Matrix4x4::rotateZ(float angle)
	{
		_m[0][0] = cos(angle);
		_m[0][1] = -sin(angle);
		_m[1][0] = sin(angle);
		_m[1][1] = cos(angle);

		return (*this);
	}

	inline	Matrix4x4& Matrix4x4::lookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
	{
		Vector3 cameraUp = up;
		cameraUp.normalize();

		Vector3 zAxis = at - eye;
		zAxis.normalize();

		Vector3 xAxis = cameraUp.crossProduct(zAxis);
		xAxis.normalize();

		Vector3 yAxis = zAxis.crossProduct(xAxis);

		_m[0][0] = xAxis._x;
		_m[1][0] = xAxis._y;
		_m[2][0] = xAxis._z;
		_m[3][0] = -xAxis.dotProduct(eye);

		_m[0][1] = yAxis._x;
		_m[1][1] = yAxis._y;
		_m[2][1] = yAxis._z;
		_m[3][1] = -yAxis.dotProduct(eye);

		_m[0][2] = zAxis._x;
		_m[1][2] = zAxis._y;
		_m[2][2] = zAxis._z;
		_m[3][2] = -zAxis.dotProduct(eye);

		_m[0][3] = 0.0f;
		_m[1][3] = 0.0f;
		_m[2][3] = 0.0f;
		_m[3][3] = 1.0f;

		return (*this);
	}

	inline	Matrix4x4& Matrix4x4::perspectiveLH(float width, float height, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		_m[0][0] = 2.0 * zNear / width;
		_m[1][1] = 2.0 * zNear / height;
		_m[2][2] = zFar / (zFar - zNear);
		_m[2][3] = 1.0f;
		_m[3][2] = (zNear * zFar) / (zNear - zFar);

		return (*this);
	}

	inline	Matrix4x4& Matrix4x4::perspectiveRH(float width, float height, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		_m[0][0] = 2.0 * zNear / width;
		_m[1][1] = 2.0 * zNear / height;
		_m[2][2] = zFar / (zNear - zFar);
		_m[2][3] = -1.0f;
		_m[3][2] = (zNear * zFar) / (zNear - zFar);

		return (*this);
	}

	inline Matrix4x4& Matrix4x4::perspectiveFovLH(float FOV, float aspect, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		float w = cos(FOV / 2.0f);
		float h = w / aspect;

		_m[0][0] = w;
		_m[1][1] = h;
		_m[2][2] = zFar / (zFar - zNear);
		_m[2][3] = 1.0f;
		_m[3][2] = (-zNear * zFar) / (zFar - zNear);

		return (*this);
	}

	inline Matrix4x4& Matrix4x4::perspectiveFovRH(float FOV, float aspect, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		float w = cos(FOV / 2.0f);
		float h = w / aspect;

		_m[0][0] = w;
		_m[1][1] = h;
		_m[2][2] = zFar / (zNear - zFar);
		_m[2][3] = -1.0f;
		_m[3][2] = (zNear * zFar) / (zNear - zFar);

		return (*this);
	}

	inline Matrix4x4& Matrix4x4::orthoLH(float width, float height, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		_m[0][0] = 2.0  / width;
		_m[1][1] = 2.0  / height;
		_m[2][2] = 1.0f / (zFar - zNear);
		_m[3][2] = (zNear) / (zNear - zFar);
		_m[3][3] = 1.0f;

		return (*this);
	}

	inline Matrix4x4& Matrix4x4::orthoRH(float width, float height, float zNear, float zFar)
	{
		memset(_v, 0, sizeof(float) * 16);

		_m[0][0] = 2.0  / width;
		_m[1][1] = 2.0  / height;
		_m[2][2] = 1.0f / (zNear - zFar);
		_m[3][2] = (zNear) / (zNear - zFar);
		_m[3][3] = 1.0f;

		return (*this);
	}


	/**********************************************************************************************
		global operations
	***********************************************************************************************/
	inline Matrix4x4 operator+(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 result;

		result._m[0][0] = a._m[0][0] + b._m[0][0];
		result._m[0][1] = a._m[0][1] + b._m[0][1];
		result._m[0][2] = a._m[0][2] + b._m[0][2];
		result._m[0][3] = a._m[0][3] + b._m[0][3];

		result._m[1][0] = a._m[1][0] + b._m[1][0];
		result._m[1][1] = a._m[1][1] + b._m[1][1];
		result._m[1][2] = a._m[1][2] + b._m[1][2];
		result._m[1][3] = a._m[1][3] + b._m[1][3];

		result._m[2][0] = a._m[2][0] + b._m[2][0];
		result._m[2][1] = a._m[2][1] + b._m[2][1];
		result._m[2][2] = a._m[2][2] + b._m[2][2];
		result._m[2][3] = a._m[2][3] + b._m[2][3];

		result._m[3][0] = a._m[3][0] + b._m[3][0];
		result._m[3][1] = a._m[3][1] + b._m[3][1];
		result._m[3][2] = a._m[3][2] + b._m[3][2];
		result._m[3][3] = a._m[3][3] + b._m[3][3];

		return result;
	}

	inline Matrix4x4 operator-(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 result;

		result._m[0][0] = a._m[0][0] - b._m[0][0];
		result._m[0][1] = a._m[0][1] - b._m[0][1];
		result._m[0][2] = a._m[0][2] - b._m[0][2];
		result._m[0][3] = a._m[0][3] - b._m[0][3];

		result._m[1][0] = a._m[1][0] - b._m[1][0];
		result._m[1][1] = a._m[1][1] - b._m[1][1];
		result._m[1][2] = a._m[1][2] - b._m[1][2];
		result._m[1][3] = a._m[1][3] - b._m[1][3];

		result._m[2][0] = a._m[2][0] - b._m[2][0];
		result._m[2][1] = a._m[2][1] - b._m[2][1];
		result._m[2][2] = a._m[2][2] - b._m[2][2];
		result._m[2][3] = a._m[2][3] - b._m[2][3];

		result._m[3][0] = a._m[3][0] - b._m[3][0];
		result._m[3][1] = a._m[3][1] - b._m[3][1];
		result._m[3][2] = a._m[3][2] - b._m[3][2];
		result._m[3][3] = a._m[3][3] - b._m[3][3];

		return result;
	}

	inline Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 result;

		result._m[0][0] = (a._m[0][0] * b._m[0][0]) + (a._m[0][1] * b._m[1][0]) + (a._m[0][2] * b._m[2][0]) + (a._m[0][3] * b._m[3][0]);
		result._m[0][1] = (a._m[0][0] * b._m[0][1]) + (a._m[0][1] * b._m[1][1]) + (a._m[0][2] * b._m[2][1]) + (a._m[0][3] * b._m[3][1]);
		result._m[0][2] = (a._m[0][0] * b._m[0][2]) + (a._m[0][1] * b._m[1][2]) + (a._m[0][2] * b._m[2][2]) + (a._m[0][3] * b._m[3][2]);
		result._m[0][3] = (a._m[0][0] * b._m[0][3]) + (a._m[0][1] * b._m[1][3]) + (a._m[0][2] * b._m[2][3]) + (a._m[0][3] * b._m[3][3]);

		result._m[1][0] = (a._m[1][0] * b._m[0][0]) + (a._m[1][1] * b._m[1][0]) + (a._m[1][2] * b._m[2][0]) + (a._m[1][3] * b._m[3][0]);
		result._m[1][1] = (a._m[1][0] * b._m[0][1]) + (a._m[1][1] * b._m[1][1]) + (a._m[1][2] * b._m[2][1]) + (a._m[1][3] * b._m[3][1]);
		result._m[1][2] = (a._m[1][0] * b._m[0][2]) + (a._m[1][1] * b._m[1][2]) + (a._m[1][2] * b._m[2][2]) + (a._m[1][3] * b._m[3][2]);
		result._m[1][3] = (a._m[1][0] * b._m[0][3]) + (a._m[1][1] * b._m[1][3]) + (a._m[1][2] * b._m[2][3]) + (a._m[1][3] * b._m[3][3]);

		result._m[2][0] = (a._m[2][0] * b._m[0][0]) + (a._m[2][1] * b._m[1][0]) + (a._m[2][2] * b._m[2][0]) + (a._m[2][3] * b._m[3][0]);
		result._m[2][1] = (a._m[2][0] * b._m[0][1]) + (a._m[2][1] * b._m[1][1]) + (a._m[2][2] * b._m[2][1]) + (a._m[2][3] * b._m[3][1]);
		result._m[2][2] = (a._m[2][0] * b._m[0][2]) + (a._m[2][1] * b._m[1][2]) + (a._m[2][2] * b._m[2][2]) + (a._m[2][3] * b._m[3][2]);
		result._m[2][3] = (a._m[2][0] * b._m[0][3]) + (a._m[2][1] * b._m[1][3]) + (a._m[2][2] * b._m[2][3]) + (a._m[2][3] * b._m[3][3]);

		result._m[3][0] = (a._m[3][0] * b._m[0][0]) + (a._m[3][1] * b._m[1][0]) + (a._m[3][2] * b._m[2][0]) + (a._m[3][3] * b._m[3][0]);
		result._m[3][1] = (a._m[3][0] * b._m[0][1]) + (a._m[3][1] * b._m[1][1]) + (a._m[3][2] * b._m[2][1]) + (a._m[3][3] * b._m[3][1]);
		result._m[3][2] = (a._m[3][0] * b._m[0][2]) + (a._m[3][1] * b._m[1][2]) + (a._m[3][2] * b._m[2][2]) + (a._m[3][3] * b._m[3][2]);
		result._m[3][3] = (a._m[3][0] * b._m[0][3]) + (a._m[3][1] * b._m[1][3]) + (a._m[3][2] * b._m[2][3]) + (a._m[3][3] * b._m[3][3]);

		return result;
	}

	inline Vector3 operator*(const Vector3& a, const Matrix4x4& b)
	{
		Vector3 result;

		result._x = (a._x * b._m[0][0]) + (a._y * b._m[1][0]) + (a._z * b._m[2][0]) + (1.0f * b._m[3][0]);
		result._y = (a._x * b._m[0][1]) + (a._y * b._m[1][1]) + (a._z * b._m[2][1]) + (1.0f * b._m[3][1]);
		result._z = (a._x * b._m[0][2]) + (a._y * b._m[1][2]) + (a._z * b._m[2][2]) + (1.0f * b._m[3][2]);

		return result;
	}

	inline Matrix4x4 operator*(float a, const Matrix4x4& b)
	{
		Matrix4x4 result;

		for(int32 i = 0; i < 4; i++)
		{
			for(int32 j = 0; j < 4; j++)
			{
				result._m[i][j] = b._m[i][j] * a;
			}
		}

		return result;
	}

	inline Matrix4x4 operator*(const Matrix4x4& a, float b)
	{
		Matrix4x4 result;

		for(int32 i = 0; i < 4; i++)
		{
			for(int32 j = 0; j < 4; j++)
			{
				result._m[i][j] = a._m[i][j] * b;
			}
		}

		return result;
	}
}
