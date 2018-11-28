#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() : Color(RGB32(0,0,0)){}
	Vertex(const Vector3 InPosition, ULONG InColor) : Position(InPosition), Color(InColor) {}
	Vertex(const Vector3 InPosition, ULONG InColor,Vector2& InUv) 
		: Position(InPosition), Color(InColor), Uv(InUv) {}

	Vector3 Position;
	ULONG Color;
	Vector2 Uv;
};