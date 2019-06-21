#include "Vector4.h"
#include "Vector3.h"
#include "Common.h"

Vector2  Vector4::ToVector2() {
	return Vector2(x,y);
}

Vector3 Vector4::ToVector3() {
	return Vector3(x,y,z);
}

Vector3 Vector4::DivisionByW() {
	return Vector3(x / w, y / w, z / w);
}

void Vector4::SelfDivisionByW() {
	float recip = 1.0f / w;

	x *= recip;
	y *= recip;
	z *= recip;
	w = 1.0f;
}

float Vector4::GetMaxElement() {
	return max(x,max(max(y, z),w));	
}

float Vector4::GetMinElement() {
	return min(x,min(min(y, z),w));	
}