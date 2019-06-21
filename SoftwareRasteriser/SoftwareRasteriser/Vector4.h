/******************************************************************************
Class:Vector4
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Simple 4-component vector class, with associated math overloads

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector2.h"
#include "Vector3.h"
class Vector3;
class Vector2;

class Vector4	{
public:
	Vector4(void) {
		x = y = z = w = 1.0f;
	}
	Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector2 ToVector2();
	Vector3 ToVector3();

	Vector3 DivisionByW();
	void	SelfDivisionByW();

	~Vector4(void){}

	inline friend std::ostream& operator<<(std::ostream& o, const Vector4& v){
		o << "Vector4(" << v.x << "," << v.y << "," << v.z <<  "," << v.w << ")" << std::endl;
		return o;
	}

	inline Vector4  operator+(const Vector4  &a) const{
		return Vector4(x + a.x,y + a.y, z + a.z, w + a.w);
	}

	inline Vector4  operator-(const Vector4  &a) const{
		return Vector4(x - a.x,y - a.y, z - a.z, w - a.w);
	}

	inline Vector4  operator-() const{
		return Vector4(-x,-y,-z, -w);
	}

	float			Length() const {
		return sqrt((x*x)+(y*y)+(z*z)+(w*w));	
	}

	float			LengthSquared() const {
		return (x*x)+(y*y)+(z*z)+(w*w);	
	}


	inline void operator+=(const Vector4  &a){
		x += a.x;
		y += a.y;
		z += a.z;
		w += a.w;
	}

	inline void operator-=(const Vector4  &a){
		x -= a.x;
		y -= a.y;
		z -= a.z;
		w -= a.w;
	}

	inline Vector4  operator*(const float a) const{
		return Vector4(x * a,y * a, z * a, w * a);
	}

	inline Vector4  operator*(const Vector4  &a) const{
		return Vector4(x * a.x,y * a.y, z * a.z, w * a.w);
	}

	inline Vector4  operator/(const Vector4  &a) const{
		return Vector4(x / a.x,y / a.y, z / a.z, w / a.w);
	};

	inline Vector4  operator/(const float v) const{
		return Vector4(x / v,y / v, z / v, w / v);
	};

	float GetMaxElement();

	float GetMinElement();

	static inline Vector4 Lerp(const Vector4 &a, const Vector4 &b, float by) {
		return (a*(1.0f-by)) + (b * by);
	}

	inline bool	operator==(const Vector4 &A)const {return (A.x == x && A.y == y && A.z == z && A.w == w) ? true : false;};
	inline bool	operator!=(const Vector4 &A)const {return (A.x == x && A.y == y && A.z == z && A.w == w) ? false : true;};

	inline float operator[](const int i) const {
		return array[i];
	}

	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float array[4];
	};

};

