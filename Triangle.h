#pragma once
#include "Vertex.h"
#include "stdafx.h"

struct  Triangle
{

public:
	Vertex vt[3];
	Triangle() {}
	Triangle(Vertex v1, Vertex v2, Vertex v3)
	{
		vt[0] = v1;
		vt[1] = v2;
		vt[2] = v3;
	}

	ULONG GetColor(const Vector2 & colorValues) const
	{
		BYTE RV1 = GetRValue(vt[0].Color);
		BYTE RV2 = GetRValue(vt[1].Color);
		BYTE RV3 = GetRValue(vt[2].Color);

		BYTE GV1 = GetGValue(vt[0].Color);
		BYTE GV2 = GetGValue(vt[1].Color);
		BYTE GV3 = GetGValue(vt[2].Color);

		BYTE BV1 = GetBValue(vt[0].Color);
		BYTE BV2 = GetBValue(vt[1].Color);
		BYTE BV3 = GetBValue(vt[2].Color);

		BYTE FinalR = (BYTE)(RV1 * (1 - colorValues.X - colorValues.Y) + RV2 * colorValues.X + RV3 * colorValues.Y);
		BYTE FinalG = (BYTE)(GV1 * (1 - colorValues.X - colorValues.Y) + GV2 * colorValues.X + GV3 * colorValues.Y);
		BYTE FinalB = (BYTE)(BV1 * (1 - colorValues.X - colorValues.Y) + BV2 * colorValues.X + BV3 * colorValues.Y);

		return RGB32(FinalR, FinalG, FinalB);
	}

};