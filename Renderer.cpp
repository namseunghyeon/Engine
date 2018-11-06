
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Figure.h"


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

	// COLOR
	static Vector3 circleColor;
	circleColor.X += 0.1f;
	circleColor.Y += 0.2f;
	circleColor.Z += 0.3f;

	circleColor.X = fmodf(circleColor.X, 255.0f);
	circleColor.Y = fmodf(circleColor.Y, 255.0f);
	circleColor.Z = fmodf(circleColor.Z, 255.0f);

	// Draw
	SetColor(circleColor.X, circleColor.Y, circleColor.Z);

	//¿ø
	static float radius = 80.0f;
	static int nradius = (int)radius;
	static float circleWay = 30.0f;

	static float rotateCircleDegree = 0;
	rotateCircleDegree += 0.5f;
	rotateCircleDegree = fmodf(rotateCircleDegree, 360.0f);

	static float MaxRadius = 2;

	static Vector3 center(0, 0);
	float circleX = (cosf(Deg2Rad(rotateCircleDegree)) * circleWay) + center.X;
	float circleY = (sinf(Deg2Rad(rotateCircleDegree)) * circleWay) + center.Y;

	// CHILD
	static float rotateCircleChildDegree = 0;
	rotateCircleChildDegree -= 0.5f;
	rotateCircleChildDegree = fmodf(rotateCircleDegree, -360.0f);

	Matrix3 transMat;
	transMat.SetTransLation(circleX, circleY);

	Matrix3 rotMat;
	rotMat.SetRotation(rotateCircleDegree);
	Matrix3 TrsMat = transMat * rotMat;

	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			IntPoint pt(i, j);
			Vector3 ptVec = pt.ToVector3();

			if (Vector3::DistSquared(center, ptVec) <= radius * radius)
			{
				PutPixel(ptVec * TrsMat);
			}
		}
	}

	// WAY
	static float circleChild_Way = radius + 70.0f;
	float circleChildX_Right = (cosf(Deg2Rad(rotateCircleChildDegree)) * circleChild_Way) + circleX;
	float circleChildY_Right = (sinf(Deg2Rad(rotateCircleChildDegree)) * circleChild_Way) + circleY;

	// COLOR
	static Vector3 color_instance_Right;
	color_instance_Right.X += 0.2f;
	color_instance_Right.Y += 0.4f;
	color_instance_Right.Z += 0.3f;

	color_instance_Right.X = fmodf(color_instance_Right.X, 255.0f);
	color_instance_Right.Y = fmodf(color_instance_Right.Y, 255.0f);
	color_instance_Right.Z = fmodf(color_instance_Right.Z, 255.0f);

	SetColor(color_instance_Right.X, color_instance_Right.Y, color_instance_Right.Z);

	// INSTANCE RIGHT
	// ROTATE
	static float rotateDegree_circleChild = 0;
	rotateDegree_circleChild++;
	rotateDegree_circleChild = fmodf(rotateDegree_circleChild, 360.0f);

	// MOVE
	//static float MaxMove1 = 101;
	//static float move1 = 0;
	//move1 = sinf(Deg2Rad(sinDegree)) * MaxMove1;

	// SCALE
	/*static float scale1 = 1;
	scale1 += 0.01f;
	scale1 = fmodf(scale1, 1.0f);*/

	static Figure instance_Right(20.0f, PutPixel);
	instance_Right.SetPosition(circleChildX_Right, circleChildY_Right);
	//instance1.SetScale(scale1, scale1, scale1);
	instance_Right.SetRotation(rotateDegree_circleChild);
	instance_Right.Update();

	// COLOR
	static Vector3 color_Right_Child;
	color_Right_Child.X += 0.4f;
	color_Right_Child.Y += 0.2f;
	color_Right_Child.Z += 0.7f;

	color_Right_Child.X = fmodf(color_Right_Child.X, 255.0f);
	color_Right_Child.Y = fmodf(color_Right_Child.Y, 255.0f);
	color_Right_Child.Z = fmodf(color_Right_Child.Z, 255.0f);

	SetColor(color_Right_Child.X, color_Right_Child.Y, color_Right_Child.Z);

	// INSTANCE RIGHT_CHILD
	static float rotateWay_Right_Child = 0;
	rotateWay_Right_Child += 3.0f;
	rotateWay_Right_Child = fmodf(rotateWay_Right_Child, 360.0f);

	// ROTATE
	static float rotateDegree_Right_Child = 0;
	rotateDegree_Right_Child+= 7.0f;
	rotateDegree_Right_Child = fmodf(rotateDegree_Right_Child, 360.0f);

	// WAY
	static float circleChild_Way_Right_Child = 50.0f;
	float circleChildX_Right_Child = (cosf(Deg2Rad(rotateWay_Right_Child)) * circleChild_Way_Right_Child) + circleChildX_Right;
	float circleChildY_Right_Child = (sinf(Deg2Rad(rotateWay_Right_Child)) * circleChild_Way_Right_Child) + circleChildY_Right;

	static Figure instance_Right_Child(10.0f, PutPixel);
	instance_Right_Child.SetPosition(circleChildX_Right_Child, circleChildY_Right_Child);
	instance_Right_Child.SetRotation(rotateDegree_Right_Child);
	instance_Right_Child.Update();

	// COLOR
	static Vector3 color_Left;
	color_Left.X += 0.7f;
	color_Left.Y += 0.2f;
	color_Left.Z += 0.4f;

	color_Left.X = fmodf(color_Left.X, 255.0f);
	color_Left.Y = fmodf(color_Left.Y, 255.0f);
	color_Left.Z = fmodf(color_Left.Z, 255.0f);

	SetColor(color_Left.X, color_Left.Y, color_Left.Z);

	// INSTANCE LEFT
	// ROTATE
	// WAY
	float circleChildX_Left = (cosf(Deg2Rad(rotateCircleChildDegree + 180.0f)) * circleChild_Way) + circleX;
	float circleChildY_Left = (sinf(Deg2Rad(rotateCircleChildDegree + 180.0f)) * circleChild_Way) + circleY;

	static Figure instance_Left(20.0f, PutPixel);
	instance_Left.SetPosition(circleChildX_Left, circleChildY_Left);
	instance_Left.SetRotation(rotateDegree_circleChild);
	instance_Left.Update();

	// COLOR
	static Vector3 color_Left_Child;
	color_Left_Child.X += 0.6f;
	color_Left_Child.Y += 0.2f;
	color_Left_Child.Z += 0.3f;

	color_Left_Child.X = fmodf(color_Left_Child.X, 255.0f);
	color_Left_Child.Y = fmodf(color_Left_Child.Y, 255.0f);
	color_Left_Child.Z = fmodf(color_Left_Child.Z, 255.0f);

	SetColor(color_Right_Child.X, color_Right_Child.Y, color_Right_Child.Z);

	// INSTANCE LEFT_CHILD
	static float rotateWay_Left_Child = 0;
	rotateWay_Left_Child += 3.0f;
	rotateWay_Left_Child = fmodf(rotateWay_Left_Child, 360.0f);

	// ROTATE
	static float rotateDegree_Left_Child = 0;
	rotateDegree_Left_Child += 7.0f;
	rotateDegree_Left_Child = fmodf(rotateDegree_Left_Child, 360.0f);

	// WAY
	static float circleChild_Way_Left_Child = 50.0f;
	float circleChildX_Left_Child = (cosf(Deg2Rad(rotateWay_Left_Child)) * circleChild_Way_Left_Child) + circleChildX_Left;
	float circleChildY_Left_Child = (sinf(Deg2Rad(rotateWay_Left_Child)) * circleChild_Way_Left_Child) + circleChildY_Left;

	static Figure instance_Left_Child(10.0f, PutPixel);
	instance_Right_Child.SetPosition(circleChildX_Left_Child, circleChildY_Left_Child);
	instance_Right_Child.SetRotation(rotateDegree_Left_Child);
	instance_Right_Child.Update();

	// COLOR
	static Vector3 color_Top;
	color_Top.X += 0.2f;
	color_Top.Y += 0.2f;
	color_Top.Z += 0.6f;

	color_Top.X = fmodf(color_Top.X, 255.0f);
	color_Top.Y = fmodf(color_Top.Y, 255.0f);
	color_Top.Z = fmodf(color_Top.Z, 255.0f);

	SetColor(color_Top.X, color_Top.Y, color_Top.Z);

	// INSTANCE TOP
	// ROTATE
	// WAY
	float circleChildX_Top = (cosf(Deg2Rad(rotateCircleChildDegree + 90.0f)) * circleChild_Way) + circleX;
	float circleChildY_Top = (sinf(Deg2Rad(rotateCircleChildDegree + 90.0f)) * circleChild_Way) + circleY;

	static Figure instance_Top(20.0f, PutPixel);
	instance_Left.SetPosition(circleChildX_Top, circleChildY_Top);
	instance_Left.SetRotation(rotateDegree_circleChild);
	instance_Left.Update();

	// COLOR
	static Vector3 color_Top_Child;
	color_Top_Child.X += 0.6f;
	color_Top_Child.Y += 0.6f;
	color_Top_Child.Z += 0.3f;

	color_Top_Child.X = fmodf(color_Top_Child.X, 255.0f);
	color_Top_Child.Y = fmodf(color_Top_Child.Y, 255.0f);
	color_Top_Child.Z = fmodf(color_Top_Child.Z, 255.0f);

	SetColor(color_Top_Child.X, color_Top_Child.Y, color_Top_Child.Z);

	// INSTANCE TOP_CHILD
	static float rotateWay_Top_Child = 0;
	rotateWay_Top_Child += 3.0f;
	rotateWay_Top_Child = fmodf(rotateWay_Top_Child, 360.0f);

	// ROTATE
	static float rotateDegree_Top_Child = 0;
	rotateDegree_Top_Child += 7.0f;
	rotateDegree_Top_Child = fmodf(rotateDegree_Top_Child, 360.0f);

	// WAY
	static float circleChild_Way_Top_Child = 50.0f;
	float circleChildX_Top_Child = (cosf(Deg2Rad(rotateWay_Top_Child)) * circleChild_Way_Top_Child) + circleChildX_Top;
	float circleChildY_Top_Child = (sinf(Deg2Rad(rotateWay_Top_Child)) * circleChild_Way_Top_Child) + circleChildY_Top;

	static Figure instance_Top_Child(10.0f, PutPixel);
	instance_Top_Child.SetPosition(circleChildX_Top_Child, circleChildY_Top_Child);
	instance_Top_Child.SetRotation(rotateDegree_Top_Child);
	instance_Top_Child.Update();

	////

	// COLOR
	static Vector3 color_Bottom;
	color_Bottom.X += 0.2f;
	color_Bottom.Y += 0.2f;
	color_Top.Z += 0.6f;

	color_Bottom.X = fmodf(color_Bottom.X, 255.0f);
	color_Bottom.Y = fmodf(color_Bottom.Y, 255.0f);
	color_Bottom.Z = fmodf(color_Bottom.Z, 255.0f);

	SetColor(color_Bottom.X, color_Bottom.Y, color_Bottom.Z);

	// INSTANCE BOTTOM
	// ROTATE
	// WAY
	float circleChildX_Bottom = (cosf(Deg2Rad(rotateCircleChildDegree + 270.0f)) * circleChild_Way) + circleX;
	float circleChildY_Bottom = (sinf(Deg2Rad(rotateCircleChildDegree + 270.0f)) * circleChild_Way) + circleY;

	static Figure instance_Bottom(20.0f, PutPixel);
	instance_Bottom.SetPosition(circleChildX_Bottom, circleChildY_Bottom);
	instance_Bottom.SetRotation(rotateDegree_circleChild);
	instance_Bottom.Update();

	// COLOR
	static Vector3 color_Bottom_Child;
	color_Bottom_Child.X += 0.6f;
	color_Bottom_Child.Y += 0.6f;
	color_Bottom_Child.Z += 0.3f;

	color_Bottom_Child.X = fmodf(color_Bottom_Child.X, 255.0f);
	color_Bottom_Child.Y = fmodf(color_Bottom_Child.Y, 255.0f);
	color_Bottom_Child.Z = fmodf(color_Bottom_Child.Z, 255.0f);

	SetColor(color_Bottom_Child.X, color_Bottom_Child.Y, color_Bottom_Child.Z);

	// INSTANCE TOP_CHILD
	static float rotateWay_Bottom_Child = 0;
	rotateWay_Bottom_Child += 3.0f;
	rotateWay_Bottom_Child = fmodf(rotateWay_Bottom_Child, 360.0f);

	// ROTATE
	static float rotateDegree_Bottom_Child = 0;
	rotateDegree_Bottom_Child += 7.0f;
	rotateDegree_Bottom_Child = fmodf(rotateDegree_Bottom_Child, 360.0f);

	// WAY
	static float circleChild_Way_Bttom_Child = 50.0f;
	float circleChildX_Bottom_Child = (cosf(Deg2Rad(rotateWay_Bottom_Child)) * circleChild_Way_Top_Child) + circleChildX_Bottom;
	float circleChildY_Bottom_Child = (sinf(Deg2Rad(rotateWay_Bottom_Child)) * circleChild_Way_Top_Child) + circleChildY_Bottom;

	static Figure instance_Bttom_Child(10.0f, PutPixel);
	instance_Bttom_Child.SetPosition(circleChildX_Bottom_Child, circleChildY_Bottom_Child);
	instance_Bttom_Child.SetRotation(rotateDegree_Bottom_Child);
	instance_Bttom_Child.Update();

	// Buffer Swap 
	BufferSwap();
}
