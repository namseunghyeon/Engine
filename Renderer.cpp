
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Vertex.h"
#include "IntPoint.h"
#include "GameObject.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);
void DrawLine(const Vector3 &start, const Vector3 &end);

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

void DrawLine(const Vector3 &start, const Vector3 &end)
{
	// L = aP + bQ, a+b=1
	float length = Vector3::Dist(end, start);
	float inc = 1.0f / length;
	int nLength = (int)(length + 1);

	for (int i = 0; i < nLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 pt = start * (1.0f - t) + end * t;
		PutPixel(IntPoint(pt));
	}
}

void DrawTriangle(const Triangle &tris, const Matrix3 &matrix)
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	Vector3 p1 = tris.vt[0].Position * matrix;
	Vector3 p2 = tris.vt[1].Position * matrix;
	Vector3 p3 = tris.vt[2].Position * matrix;

	if (p1.X < minPos.X) minPos.X = p1.X;
	if (p1.Y < minPos.Y) minPos.Y = p1.Y;
	if (p1.X > maxPos.X) maxPos.X = p1.X;
	if (p1.Y > maxPos.Y) maxPos.Y = p1.Y;

	if (p2.X < minPos.X) minPos.X = p2.X;
	if (p2.Y < minPos.Y) minPos.Y = p2.Y;
	if (p2.X > maxPos.X) maxPos.X = p2.X;
	if (p2.Y > maxPos.Y) maxPos.Y = p2.Y;

	if (p3.X < minPos.X) minPos.X = p3.X;
	if (p3.Y < minPos.Y) minPos.Y = p3.Y;
	if (p3.X > maxPos.X) maxPos.X = p3.X;
	if (p3.Y > maxPos.Y) maxPos.Y = p3.Y;

	Vector3 u = p2 - p1;
	Vector3 v = p3 - p1;
	float dotUU = Vector3::Dot(u, u);
	float dotUV = Vector3::Dot(u, v);
	float dotVV = Vector3::Dot(v, v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	IntPoint minPt(minPos);
	IntPoint maxPt(maxPos);

	for (int x = minPt.X; x < maxPt.X; x++)
	{
		for (int y = minPt.Y; y < maxPt.Y; y++)
		{
			IntPoint pt(x, y);
			Vector3 w = pt.ToVector3() - p1;
			float dotUW = Vector3::Dot(u, w);
			float dotVW = Vector3::Dot(v, w);
			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1))
			{
				ULONG fColor = RGB32(255, 0, 0);
				if (g_Texture->IsLoaded())
				{
					Vector2 uvValue = tris.vt[0].Uv * (1 - s - t)
						+ tris.vt[1].Uv * s + tris.vt[2].Uv * t;

					fColor = g_Texture->GetTexturePixel(uvValue);
				}

				else
				{
					BYTE RV1 = GetRValue(tris.vt[0].Color);
					BYTE RV2 = GetRValue(tris.vt[1].Color);
					BYTE RV3 = GetRValue(tris.vt[2].Color);

					BYTE GV1 = GetGValue(tris.vt[0].Color);
					BYTE GV2 = GetGValue(tris.vt[1].Color);
					BYTE GV3 = GetGValue(tris.vt[2].Color);


					BYTE BV1 = GetBValue(tris.vt[0].Color);
					BYTE BV2 = GetBValue(tris.vt[1].Color);
					BYTE BV3 = GetBValue(tris.vt[2].Color);

					BYTE FinalR = (BYTE)(RV1 * (1 - s - t) + RV2 * s + RV3 * t);
					BYTE FinalG = (BYTE)(GV1 * (1 - s - t) + GV2 * s + GV3 * t);
					BYTE FinalB = (BYTE)(BV1 * (1 - s - t) + BV2 * s + BV3 * t);
					fColor = RGB32(FinalR, FinalG, FinalB);
				}

				SetColor(fColor);
				PutPixel(pt);
			}
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	static float xPos = 0;
	static float yPos = 0;

	static float scale = 1;

	static float degree = 0;

	// Input
	if (GetAsyncKeyState(VK_LEFT)) degree += 1;
	if (GetAsyncKeyState(VK_RIGHT)) degree -= 1;

	if (GetAsyncKeyState(VK_DOWN))
	{
		xPos -= cosf(Deg2Rad(degree));
		yPos -= sinf(Deg2Rad(degree));
	}
	if (GetAsyncKeyState(VK_UP))
	{
		xPos += cosf(Deg2Rad(degree));
		yPos += sinf(Deg2Rad(degree));
	}

	// VK_PRIOR, VK_NEXT
	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.01f;

	SetColor(255, 0, 0);

	// HEAD
	static Vector3 p1 = Vector3::Make2DPoint(-80, -80);
	static Vector3 p2 = Vector3::Make2DPoint(-80, 80);
	static Vector3 p3 = Vector3::Make2DPoint(80, 80);
	static Vector3 p4 = Vector3::Make2DPoint(80, -80);

	static Vertex v1(p1, RGB32(255, 0, 0), Vector2(0.125f, 0.25f));
	static Vertex v2(p2, RGB32(0, 255, 0), Vector2(0.125f, 0.125f));
	static Vertex v3(p3, RGB32(0, 0, 255), Vector2(0.25f, 0.125f));
	static Vertex v4(p4, RGB32(255, 255, 255), Vector2(0.25f, 0.25f));

	// BODY
	static Vector3 p5 = Vector3::Make2DPoint(-80, -300);
	static Vector3 p6 = Vector3::Make2DPoint(-80, -80);
	static Vector3 p7 = Vector3::Make2DPoint(80, -80);
	static Vector3 p8 = Vector3::Make2DPoint(80, -300);

	static Vertex v5(p5, RGB32(255, 0, 0), Vector2(0.3125f, 0.5f));
	static Vertex v6(p6, RGB32(0, 255, 0), Vector2(0.3125f, 0.3125f));
	static Vertex v7(p7, RGB32(0, 0, 255), Vector2(0.4375f, 0.3125f));
	static Vertex v8(p8, RGB32(255, 255, 255), Vector2(0.4375f, 0.5f));

	// L ARAM
	static Vector3 p9 = Vector3::Make2DPoint(80, -300);
	static Vector3 p10 = Vector3::Make2DPoint(80, -80);
	static Vector3 p11 = Vector3::Make2DPoint(140, -80);
	static Vector3 p12 = Vector3::Make2DPoint(140, -300);

	static Vertex v9(p9, RGB32(255, 0, 0), Vector2(0.5625f, 1.0f));
	static Vertex v10(p10, RGB32(0, 255, 0), Vector2(0.5625f, 0.8125f));
	static Vertex v11(p11, RGB32(0, 0, 255), Vector2(0.625f, 0.8125f));
	static Vertex v12(p12, RGB32(255, 255, 255), Vector2(0.625f, 1.0f));

	// R ARAM
	static Vector3 p13 = Vector3::Make2DPoint(-140, -300);
	static Vector3 p14 = Vector3::Make2DPoint(-140, -80);
	static Vector3 p15 = Vector3::Make2DPoint(-80, -80);
	static Vector3 p16 = Vector3::Make2DPoint(-80, -300);

	static Vertex v13(p13, RGB32(255, 0, 0), Vector2(0.6875f, 0.5f));
	static Vertex v14(p14, RGB32(0, 255, 0), Vector2(0.6875f, 0.3125f));
	static Vertex v15(p15, RGB32(0, 0, 255), Vector2(0.75f, 0.3125f));
	static Vertex v16(p16, RGB32(255, 255, 255), Vector2(0.75f, 0.5f));

	// L LEG
	static Vector3 p17 = Vector3::Make2DPoint(0, -500);
	static Vector3 p18 = Vector3::Make2DPoint(0, -300);
	static Vector3 p19 = Vector3::Make2DPoint(80, -300);
	static Vector3 p20 = Vector3::Make2DPoint(80, -500);

	static Vertex v17(p17, RGB32(255, 0, 0), Vector2(0.3125f, 1.0f));
	static Vertex v18(p18, RGB32(0, 255, 0), Vector2(0.3125f, 0.8125f));
	static Vertex v19(p19, RGB32(0, 0, 255), Vector2(0.375f, 0.8125f));
	static Vertex v20(p20, RGB32(255, 255, 255), Vector2(0.375f, 1.0f));

	// R LEG
	static Vector3 p21 = Vector3::Make2DPoint(-80, -500);
	static Vector3 p22 = Vector3::Make2DPoint(-80, -300);
	static Vector3 p23 = Vector3::Make2DPoint(0, -300);
	static Vector3 p24 = Vector3::Make2DPoint(0, -500);

	static Vertex v21(p21, RGB32(255, 0, 0), Vector2(0.0625f, 0.5f));
	static Vertex v22(p22, RGB32(0, 255, 0), Vector2(0.0625f, 0.3125f));
	static Vertex v23(p23, RGB32(0, 0, 255), Vector2(0.125f, 0.3125f));
	static Vertex v24(p24, RGB32(255, 255, 255), Vector2(0.125f, 0.5f));

	static Triangle tris[]
	{
		// HEAD
		Triangle(v1,v2,v3), Triangle(v1,v3,v4),

		// BODY
		Triangle(v5,v6,v7), Triangle(v5,v7,v8),

		// L ARAM
		Triangle(v9,v10,v11), Triangle(v9,v11,v12),

		// R ARAM
		Triangle(v13,v14,v15), Triangle(v13,v15,v16),

		// L LEG
		Triangle(v17,v18,v19), Triangle(v17,v19,v20),

		// R LEG
		Triangle(v21,v22,v23), Triangle(v21,v23,v24),
	};

	static GameObject2D object1(Mesh(tris, _countof(tris), DrawTriangle));

	object1.Transform.SetPosition(xPos, yPos);
	object1.Transform.SetAngle(degree);
	object1.Transform.SetScale(scale, scale, scale);
	object1.Update();

	// Buffer Swap 
	BufferSwap();
}