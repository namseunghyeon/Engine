
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Vertex.h"
#include "IntPoint.h"
#include "GameObject.h"

#pragma region UV_POINT
// BODY
// Vector2(0.3125f, 0.5f));
// Vector2(0.3125f, 0.3125f));
// Vector2(0.4375f, 0.3125f));
// Vector2(0.4375f, 0.5f));

// L ARAM
// Vector2(0.5625f, 1.0f));
// Vector2(0.5625f, 0.8125f));
// Vector2(0.625f, 0.8125f));
// Vector2(0.625f, 1.0f));

// R ARAM
// Vector2(0.6875f, 0.5f));
// Vector2(0.6875f, 0.3125f));
// Vector2(0.75f, 0.3125f));
// Vector2(0.75f, 0.5f));

// L LEG
// Vector2(0.3125f, 1.0f));
// Vector2(0.3125f, 0.8125f));
// Vector2(0.375f, 0.8125f));
// Vector2(0.375f, 1.0f));

// R LEG
// Vector2(0.0625f, 0.5f));
// Vector2(0.0625f, 0.3125f));
// Vector2(0.125f, 0.3125f));
// Vector2(0.125f, 0.5f));
#pragma endregion

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

	for (int x = minPt.X; x < maxPt.X; x++) {
		for (int y = minPt.Y; y < maxPt.Y; y++) {
			IntPoint pt(x, y);
			Vector3 w = pt.ToVector3() - p1;
			float dotUW = Vector3::Dot(u, w);
			float dotVW = Vector3::Dot(v, w);
			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1)) {
				ULONG fColor = RGB32(255, 0, 0);
				if (g_Texture->IsLoaded())
				{
					Vector2 uvValue = tris.vt[0].Uv * (1 - s - t)
						+ tris.vt[1].Uv * s + tris.vt[2].Uv * t;

					fColor = g_Texture->GetTexturePixel(uvValue);
				}

				else {
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

void UpdateFrame(void) {
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	static float xPos = 0;
	static float yPos = 0;

	static float scale = 1;

	static float degree = 0;

	// Input
	if (GetAsyncKeyState(VK_LEFT)) xPos--;
	else if (GetAsyncKeyState(VK_RIGHT)) xPos++;

	if (GetAsyncKeyState(VK_DOWN)) yPos--;
	else if (GetAsyncKeyState(VK_UP)) yPos++;

	if (GetAsyncKeyState(VK_HOME)) degree--;
	else if (GetAsyncKeyState(VK_END))degree++;

	// VK_PRIOR, VK_NEXT
	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.01f;

	static GameObject2D Cameara(Transform2D(Vector2(0, 0), 0));
	Cameara.Transform.SetPosition(xPos, yPos);
	Cameara.Transform.SetAngle(degree);
	Cameara.Transform.UpdateRTMatrix();
		
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

	static Triangle tris[]{
		Triangle(v1,v2,v3), Triangle(v1,v3,v4),
	};

	static GameObject2D object1(Mesh(tris, _countof(tris), DrawTriangle));
	
	// OBJECT 2
	static Vertex object2V1(Vector3::Make2DPoint(-350, 0), RGB32(255, 255, 255), Vector2(0.125f, 0.25f));
	static Vertex object2V2(Vector3::Make2DPoint(-350, -50), RGB32(255, 0, 0), Vector2(0.125f, 0.125f));
	static Vertex object2V3(Vector3::Make2DPoint(-300, -50), RGB32(0, 255, 0), Vector2(0.25f, 0.125f));
	static Vertex object2V4(Vector3::Make2DPoint(-300, 0), RGB32(0, 0, 255), Vector2(0.25f, 0.25f));

	static Triangle object2Tris[]{
		Triangle(object2V1,object2V2,object2V3),
		Triangle(object2V1,object2V3,object2V4),
	};

	static GameObject2D object2(Mesh(object2Tris, _countof(object2Tris), DrawTriangle));

	// OBJECT 3
	static Vertex object3V1(Vector3::Make2DPoint(-250, 0), RGB32(255, 255, 255), Vector2(0.3125f, 0.5f));
	static Vertex object3V2(Vector3::Make2DPoint(-250, -50), RGB32(255, 0, 0), Vector2(0.3125f, 0.3125f));
	static Vertex object3V3(Vector3::Make2DPoint(-200, -50), RGB32(0, 255, 0), Vector2(0.4375f, 0.3125f));
	static Vertex object3V4(Vector3::Make2DPoint(-200, 0), RGB32(0, 0, 255), Vector2(0.4375f, 0.5f));

	static Triangle object3Tris[]{
		Triangle(object3V1,object3V2,object3V3),
		Triangle(object3V1,object3V3,object3V4),
	};

	static GameObject2D object3(Mesh(object3Tris, _countof(object3Tris), DrawTriangle));

	object1.Update(Cameara.Transform);
	object2.Update(Cameara.Transform);
	object3.Update(Cameara.Transform);

	// Buffer Swap 
	BufferSwap();
}