/******************************************************************************
Class:Vector2
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Simple 2-component vector class, with associated math overloads

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>

class Vector2	{
public:
	Vector2(void) {
		ToZero();
	}

	Vector2(const float x, const float y) {
		this->x = x;
		this->y = y;
	}

	~Vector2(void){}

	float x;
	float y;

	void ToZero(){
		x = 0.0f;
		y = 0.0f;
	}

	static float	Dot(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.x)+(a.y*b.y);
	}

	static float	Cross(const Vector2 &a, const Vector2 &b) {
		return (a.x*b.y) - (a.y*b.x);
	}

	float			Length() const {
		return sqrt((x*x)+(y*y));	
	}

	float			LengthSquared() const {
		return (x*x)+(y*y);	
	}

	void			Normalise() {
		float length = Length();

		if(length != 0.0f)	{
			length = 1.0f / length;
			x = x * length;
			y = y * length;
		}
	}

	static Vector2 Lerp(const Vector2 &a, const Vector2 &b, float by) {
		Vector2 p;

		p.x = ( (b.x * by) + (a.x * (1.0f - by)) );
		p.y = ( (b.y * by) + (a.y * (1.0f - by)) );

		return p;
	}


	inline friend std::ostream& operator<<(std::ostream& o, const Vector2& v){
		o << "Vector2(" << v.x << "," << v.y << ")" << std::endl;
		return o;
	}

	inline Vector2  operator-(const Vector2  &a) const{
		return Vector2(x - a.x,y - a.y);
	}

	inline Vector2  operator+(const Vector2  &a) const{
		return Vector2(x + a.x,y + a.y);
	}

	inline Vector2  operator*(const Vector2  &a) const{
		return Vector2(x * a.x,y * a.y);
	}

	inline Vector2  operator*(const float  &a) const{
		return Vector2(x * a,y * a);
	}

	inline Vector2  operator/(const Vector2  &a) const{
		return Vector2(x / a.x,y / a.y);
	}

	inline Vector2  operator/(const float  &a) const{
		return Vector2(x / a,y / a);
	}

	inline void  operator/=(const float  &a) {
		x /= a;
		y /= a;
	}

	inline void  operator*=(const float  &a) {
		x *= a;
		y *= a;
	}

	inline void operator+=(const Vector2  &a){
		x += a.x;
		y += a.y;
	}

	inline void operator-=(const Vector2  &a){
		x -= a.x;
		y -= a.y;
	}

	inline void operator*=(const Vector2  &a){
		x *= a.x;
		y *= a.y;
	}

	inline void operator/=(const Vector2  &a){
		x /= a.x;
		y /= a.y;
	}
};

