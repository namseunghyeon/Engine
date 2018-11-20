#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex(Vector3 v, ULONG c) : position(v), color(c) {}

	Vector3 position;
	ULONG color;
};