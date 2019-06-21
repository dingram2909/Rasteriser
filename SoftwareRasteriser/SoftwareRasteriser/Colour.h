/******************************************************************************
Class:Colour
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Simple class to hold colours made up of 4 bytes.

You might be wondering why in the class, the bytes aren't arranged in rgb order

This is due to how Win32 internally stores its bitmaps - in bgra. In order to 
speed up mem copies to the window buffer, we keep our colours in the same order

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once

struct Colour {
	union {
		struct {
			unsigned char b;
			unsigned char g;
			unsigned char r;
			unsigned char a;
		};
		unsigned int c;
	};


	Colour() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}

	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	inline void Reset() {
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}

	inline void Set(const unsigned char &red, const unsigned char &green,const unsigned char &blue,const unsigned char &alpha ) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	static Colour Lerp(const Colour &a, const Colour &b, float by) {
		Colour p;

		float minusBy = 1.0f - by;

		p.r = unsigned char( (b.r * by) + (a.r * minusBy) );
		p.g = unsigned char( (b.g * by) + (a.g * minusBy) );
		p.b = unsigned char( (b.b * by) + (a.b * minusBy) );
		p.a = unsigned char( (b.a * by) + (a.a * minusBy) );

		return p;
	}

	inline Colour  operator*(const float factor) const{
		return Colour(	(unsigned char)(float)(r * factor),
						(unsigned char)(float)(g * factor), 
						(unsigned char)(float)(b * factor), 
						(unsigned char)(float)(a * factor));
	}



	inline Colour  operator+(const Colour  &add) const{
		return Colour(r + add.r,g + add.g, b + add.b, a + add.a);
	}

	inline void  operator+=(const Colour  &add) {
		r += add.r;
		g += add.g;
		b += add.b;
		a += add.a;
	}


	inline Colour  operator-(const Colour  &sub) const{
		return Colour(r - sub.r,g - sub.g, b - sub.b, a - sub.a);
	}

	static Colour White;
};

