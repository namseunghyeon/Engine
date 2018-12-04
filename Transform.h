#pragma once
#include "Vector.h"
#include "stdafx.h"

struct Transform2D
{
private:
	Matrix3 _matrix;

	Vector2 _position;
	float _angle;
	Vector3 _scale;

public:
	Transform2D() : _position(0, 0), _angle(0), _scale(Vector3(1, 1)) {}
	Transform2D(Vector2 InPositin) : _position(InPositin) {}
	Transform2D(Vector2 InPositin, float Inangle) : _position(InPositin), _angle(Inangle) {}
	Transform2D(Vector2 InPositin, float Inangle, Vector3 InScale)
		: _position(InPositin), _angle(Inangle), _scale(InScale) {}

	// GET
	const Matrix3 GetMatrix() { return _matrix; }
	const Vector2 GetPosition() { return _position; }

	void SetPosition(const Vector2 &InPosition) { _position = InPosition; }
	void SetPosition(const float InX, float InY) { _position.X = InX; _position.Y = InY; }
	void SetAngle(const float &InAngle) { _angle = InAngle; }
	void SetScale(const Vector3 &InScale) { _scale = InScale; }
	void SetScale(const float InScaleX, const float InScaleY, const float InScaleZ)
	{
		_scale.X = InScaleX;
		_scale.Y = InScaleY;
		_scale.Z = InScaleZ;
	}

	void UpdateRTMatrix() {
		Matrix3 tMat, rMat;
		tMat.SetTransLation(_position.X, _position.Y);
		rMat.SetRotation(_angle);

		Matrix3 result = rMat * tMat;
		result._13 *= -1;
		result._23 *= -1;

		_matrix = result;
	}

	void Update()
	{
		Matrix3 tMat, rMat, sMat;
		tMat.SetTransLation(_position.X, _position.Y);
		rMat.SetRotation(_angle);
		sMat.SetScale(_scale.X, _scale.Y, _scale.Z);

		_matrix = tMat * rMat * sMat;
	}

	void Update(const Matrix3 &camMatrix)
	{
		Update();

		_matrix = _matrix * camMatrix;
	}
};