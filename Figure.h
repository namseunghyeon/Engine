#pragma once

#include "Vector.h"
#include "IntPoint.h"

class Figure
{
private:

	Matrix3 _position;
	Matrix3 _rotate;
	Matrix3 _scale;

	float _radius;
	int _nradius;

	void(*_putPixel)(const IntPoint&);

	Figure();
public:
	Figure(float radius, void(*method)(const IntPoint&)) : _radius(radius), _nradius((int)radius), _putPixel(method) {}
	~Figure() { _putPixel = nullptr; }

	float GetRadius() const { return _radius; }
	int GetNRadius() const { return _nradius; }

	Matrix3 GetPosition() { return _position; }
	Matrix3 GetRotate() { return _rotate; }
	Matrix3 GetScale() { return _scale; }

	void SetPosition(float InX, float InY)
	{
		_position.SetIdentity();
		_position._13 = InX;
		_position._23 = InY;
	}

	void SetRotation(float degree)
	{
		_rotate.SetIdentity();
		float radian = Deg2Rad(degree);

		_rotate._11 = cosf(radian);
		_rotate._12 = -sinf(radian);
		_rotate._21 = -_rotate._12;
		_rotate._22 = _rotate._11;
	}

	void SetScale(float x, float y, float z)
	{
		_scale.SetIdentity();
		_scale._11 = x;
		_scale._22 = y;
		_scale._33 = z;
	}

	Matrix3 GetTrs()const { return _position * _rotate * _scale; }

	void Update()
	{
		if (_putPixel != nullptr)
			for (int i = -_nradius; i <= _nradius; i++)
			{
				for (int j = -_nradius; j <= _nradius; j++)
				{
					_putPixel(Vector3(i, j) * GetTrs());
				}
			}
	}
};


//enum FIGURE_TYPE
//{
//	_RECT = 0x00,
//	_CIRCLE = 0x01
//};
//
//class FigureInstance
//{
//private:
//
//	Vector3 **_pixelPos;
//
//	void Rect()
//	{
//		for (int i = 0; i <= _nradius * 2; i++)
//		{
//			for (int j = 0; j <= _nradius * 2; j++)
//			{
//				_pixelPos[i][j] * GetTrs();
//			}
//		}
//	}
//
//	void Circle()
//	{
//	}
//
//	void(FigureInstance::*_update)();
//
//public:
//	FigureInstance() : _radius(1.0f), _nradius(1)
//	{
//		_update = &FigureInstance::Rect;
//	}
//
//	FigureInstance(float radius) : _radius(radius), _nradius((int)radius)
//	{
//		_update = &FigureInstance::Rect;
//	}
//
//	FigureInstance(float radius, FIGURE_TYPE type) :_radius(radius), _nradius((int)radius)
//	{
//		if (type == FIGURE_TYPE::_RECT)
//			_update = &FigureInstance::Rect;
//
//		else if (type == FIGURE_TYPE::_CIRCLE)
//			_update = &FigureInstance::Circle;
//	}
//
//	~FigureInstance()
//	{
//		_update = nullptr;
//	}
//
//
//	Vector3 GetPixelPos() const
//	{
//		return _pixelPos;
//	}
//
//	void Update()
//	{
//		(this->*_update)();
//	}
//};