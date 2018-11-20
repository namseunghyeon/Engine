#pragma once
#include "Vector2.h"

struct  IntPoint
{
public:
	int X;
	int Y;
	int Z;

	IntPoint() : X(0), Y(0) {}
	IntPoint(int InX, int InY) : X(InX), Y(InY) {}
	IntPoint(Vector2 InVector2) : X((int)InVector2.X), Y((int)InVector2.Y) {}
	IntPoint(Vector3 InVector3) : X((int)InVector3.X), Y((int)InVector3.Y), Z(1) {}

public:
	Vector2 ToVector2() { return Vector2((float)X + 0.5f, (float)Y + 0.5f); }
	Vector3 ToVector3() { return Vector3((float)X + 0.5f, (float)Y + 0.5f, 1); }
};
