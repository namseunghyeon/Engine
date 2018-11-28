#pragma once
#include "stdafx.h"
#include "Transform.h"
#include "Mesh.h"

struct  GameObject2D
{
public:
	GameObject2D() {};
	GameObject2D(const Mesh &InMesh) : mMesh(InMesh) {};
	GameObject2D(const Transform2D InTransform, const Mesh &InMesh) :
		Transform(InTransform), mMesh(InMesh) {}

	Mesh mMesh;
	Transform2D Transform;

	void Update()
	{
		Transform.Update();
		mMesh.Update(Transform);
	}
};