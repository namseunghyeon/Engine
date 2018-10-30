#pragma once
#include "Vector2.h"

struct  IntPoint
{
public:
	int X;
	int Y;

	IntPoint() : X(0), Y(0) {}
	IntPoint(int InX, int InY) : X(InX), Y(InY) {}
	IntPoint(Vector2 InVector2) : X((int)InVector2.X), Y((int)InVector2.Y) {}

public:
	Vector2 ToVector2() { return Vector2((float)X, (float)Y); }
};
