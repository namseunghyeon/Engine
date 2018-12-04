#pragma once

#include "stdafx.h"
#include "Triangle.h"

struct Mesh
{
private:
	int _length = 0;
	void(*_draw)(const Triangle &tris, const Matrix3 &matrix3);
	Vector2 _minPos, _maxPos;
	float invDenom;

public:
	Mesh() : Triangles(NULL), _length(0) {};
	Mesh(Triangle Intriangles[], int InLength, void(*draw)(const Triangle &tris, const Matrix3 &matrix3)) :
		Triangles(Intriangles), _length(InLength), _draw(draw) {}
	~Mesh()
	{
		_draw = NULL;
		_draw = nullptr;
	}

public:
	Triangle *Triangles;

	const int GetLength() { return _length; }

	const void Update(Transform2D &transform) {

		Matrix3 matrix = transform.GetMatrix();
		for (int i = 0; i < _length; i++)
			for (int tris = 0; tris <= 2; tris++) {
				// DRAW
				_draw(Triangles[i], matrix);
			}
	}
};