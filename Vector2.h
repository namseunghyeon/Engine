#pragma once
#include "stdafx.h"
#include "Matrix.h"

struct Vector2
{
public:
	float X;
	float Y;

	Vector2() : X(0), Y(0) {}
	Vector2(float InX, float InY) : X(InX), Y(InY) {}

public:
	static float Dist(const Vector2 &V1, const Vector2 &V2);
	static float DistSquared(const Vector2 &V1, const Vector2 &V2);

	Vector2 operator*(const Matrix2 & Mat)const;

	bool Equlas(const Vector2& V, float tolernace = KINDA_SMALL_NUMBER) const;
};