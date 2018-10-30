
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "IntPoint.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

void PutPixel(const IntPoint& inPt)
{
	PutPixel(inPt.X, inPt.Y);
}

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	//PutPixel(0, 0);

	// Draw a Fiiled Circle with Radius 100
	Vector2 center(0.0f, 0.0f);
	float radius = 100.0f;
	int nradius = (int)radius;

	Vector2 startVec(radius, 0.0f);

	static float degree = 0;
	degree += 0.1f;
	degree = fmodf(degree, 360.0f);

	Matrix2 rotMat;
	rotMat.SetRotation(degree);

	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			PutPixel(Vector2(i, j) * rotMat);
		}
	}

	// 외각선 원 찍기
	//for (int i = 0; i < 360; i++)
	//{
	//	Matrix2 _rotMat;
	//	_rotMat.SetRotation(i);
	//	IntPoint rotatedPt(startVec * _rotMat);
	//	PutPixel(rotatedPt);
	//}

	// 원 변형
	//Matrix2 scaleMat;
	//scaleMat.SetScale(2.0f, 0.5f);

	//Matrix2 rotMat;
	//rotMat.SetRotation(30);

	//Matrix2 SRMat = scaleMat * rotMat;
	//Matrix2 RSMat = rotMat * scaleMat;

	//for (int i = -nradius; i <= nradius; i++)
	//{
	//	for (int j = -nradius; j <= nradius; j++)
	//	{
	//		IntPoint pt(i, j);
	//		Vector2 ptVec = pt.ToVector2();

	//		//if (Vector2::DistSquared(center, ptVec) <= radius * radius)
	//		//{
	//		//	// Normal
	//		//	//PutPixel(ptVec);

	//		//	// Scale
	//		//	/*IntPoint scaledPt = (ptVec * scaleMat);
	//		//	PutPixel(scaledPt);*/

	//		//	// Rotate
	//		//	/*IntPoint rotatedPt(ptVec * rotMat);
	//		//	PutPixel(rotatedPt);*/

	//		//	// SR or RS
	//		//	//IntPoint SRpt(ptVec * SRMat);
	//		//	//IntPoint RSpt(ptVec * RSMat);
	//		//	//PutPixel(SRpt);
	//		//}
	//	}
	//}

	// Buffer Swap 
	BufferSwap();
}
