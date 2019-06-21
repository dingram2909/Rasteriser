#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"
#include "Common.h"

Vector4 Vector3::ToVector4(float w) {
	return Vector4(x,y,z,w);
}

float Vector3::GetMaxElement() {
	return max(x,max(y,z));	
}

float Vector3::GetMinElement() {
	return min(x,min(y,z));	
}

Vector2 Vector3::ToVector2() {
	return Vector2(x,y);
}