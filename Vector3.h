#pragma once
#include "stdafx.h"
#include "Matrix.h"

struct Vector3
{
public:
	float X;
	float Y;
	float Z;

	Vector3() : X(0), Y(0), Z(1) {}
	Vector3(float InX, float InY) : X(InX), Y(InY), Z(1) {}
	Vector3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

	static float Dist(const Vector3 &V1, const Vector3 &V2);
	static float DistSquared(const Vector3 &V1, const Vector3 &V2);

	bool Equlas(const Vector3& V, float tolernace = KINDA_SMALL_NUMBER) const;

	Vector3 operator*(const Matrix3 & Mat)const;
};