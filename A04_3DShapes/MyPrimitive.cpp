#include <cmath>
#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
    float angle = 360.0f / a_nSubdivisions / 180 * PI;
    float y = -a_fHeight / 2;
    vector3 tip(0, a_fHeight / 2, 0);
    vector3 center(0, y, 0);

    for (int i = 0; i < a_nSubdivisions; i++)
    {
        vector3 a(cos(i * angle) * a_fRadius, y, sin(i * angle) * a_fRadius);
        vector3 b(cos((i + 1) * angle) * a_fRadius, y, sin((i + 1) * angle) * a_fRadius);

        AddVertexPosition(b);
        AddVertexPosition(a);
        AddVertexPosition(tip);

        AddVertexPosition(a);
        AddVertexPosition(b);
        AddVertexPosition(center);
    }

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
    float angle = 360.0f / a_nSubdivisions / 180 * PI;
    float topY = a_fHeight / 2;
    float botY = -a_fHeight / 2;
    vector3 topCenter(0, topY, 0);
    vector3 botCenter(0, botY, 0);

    for (int i = 0; i < a_nSubdivisions; i++)
    {
        vector3 a(cos(i * angle) * a_fRadius, botY, sin(i * angle) * a_fRadius);
        vector3 b(cos((i + 1) * angle) * a_fRadius, botY, sin((i + 1) * angle) * a_fRadius);
        vector3 c(cos(i * angle) * a_fRadius, topY, sin(i * angle) * a_fRadius);
        vector3 d(cos((i + 1) * angle) * a_fRadius, topY, sin((i + 1) * angle) * a_fRadius);

        AddQuad(b, a, d, c);

        AddVertexPosition(d);
        AddVertexPosition(c);
        AddVertexPosition(topCenter);

        AddVertexPosition(a);
        AddVertexPosition(b);
        AddVertexPosition(botCenter);
    }

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
    float angle = 360.0f / a_nSubdivisions / 180 * PI;
    float topY = a_fHeight / 2;
    float botY = -a_fHeight / 2;

    for (int i = 0; i < a_nSubdivisions; i++)
    {
        vector3 a(cos(i * angle) * a_fOuterRadius, botY, sin(i * angle) * a_fOuterRadius);
        vector3 b(cos((i + 1) * angle) * a_fOuterRadius, botY, sin((i + 1) * angle) * a_fOuterRadius);
        vector3 c(cos(i * angle) * a_fOuterRadius, topY, sin(i * angle) * a_fOuterRadius);
        vector3 d(cos((i + 1) * angle) * a_fOuterRadius, topY, sin((i + 1) * angle) * a_fOuterRadius);

        vector3 e(cos(i * angle) * a_fInnerRadius, botY, sin(i * angle) * a_fInnerRadius);
        vector3 f(cos((i + 1) * angle) * a_fInnerRadius, botY, sin((i + 1) * angle) * a_fInnerRadius);
        vector3 g(cos(i * angle) * a_fInnerRadius, topY, sin(i * angle) * a_fInnerRadius);
        vector3 h(cos((i + 1) * angle) * a_fInnerRadius, topY, sin((i + 1) * angle) * a_fInnerRadius);

        AddQuad(a, c, b, d);
        AddQuad(e, f, g, h);
        AddQuad(a, b, e, f);
        AddQuad(d, c, h, g);
    }

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
    float angleA = 360.0f / a_nSubdivisionsA / 180 * PI;
    float angleB = 360.0f / a_nSubdivisionsB / 180 * PI;
    float miniRadius = (a_fOuterRadius - a_fInnerRadius) / 2;
    float radiusMid = miniRadius + a_fInnerRadius;

    vector3* points = new vector3[a_nSubdivisionsB];
    vector3* nextPoints = new vector3[a_nSubdivisionsB];

    for (int i = 0; i < a_nSubdivisionsA; i++)
    {
        for (int k = 0; k < a_nSubdivisionsB; k++)
        {
            nextPoints[k] = vector3(cos(i * angleA) * miniRadius, sin(k * angleB) * miniRadius, sin(i * angleA) * miniRadius);
        }

        //add quads for current section if not first time
        if (i != 0)
        {
            for (int k = 0; k < a_nSubdivisionsB; k++)
                AddQuad(nextPoints[(k + 1) % a_nSubdivisionsB], nextPoints[k], points[(k + 1) % a_nSubdivisionsB], points[k]);
        }

        for (int k = 0; k < a_nSubdivisionsB; k++)
            points[k] = nextPoints[k];
    }

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//Your code starts here
    int hSubdivisions = a_nSubdivisions + 2;

    //max y value
    float y = a_fRadius;

    //difference in height per slice
    float yDif = a_fRadius * 2 / (hSubdivisions + 1);

    //amount of rotation per subdivision
    float angle = 360.0f / a_nSubdivisions / 180 * PI;

    //create array to hold current points and next points
    vector3* points = new vector3[a_nSubdivisions];
    vector3* nextPoints = new vector3[a_nSubdivisions];

    //create array to hold radii
    float* radii = new float[hSubdivisions];

    //calculate radius at each subdivisions
    for (int i = 0; i < hSubdivisions / 2; i++)
    {
        radii[i] = radii[hSubdivisions - i - 1] =
            sqrt(pow(a_fRadius, 2) - pow(a_fRadius - (i * yDif), 2));
    }

    //if the number of subdivisions is odd,
    //set the middle radius to the full radius
    if (hSubdivisions % 2 == 1)
        radii[hSubdivisions / 2] = a_fRadius;

    //sides
    for (int i = 0; i < hSubdivisions; i++)
    {
        //calculate the next points in the slices
        for (int k = 0; k < a_nSubdivisions; k++)
            nextPoints[k] = vector3(cos(k * angle) * radii[i], y - (i + 1) * yDif, sin(k * angle) * radii[i]);

        //add the quad for this slice
        if (i != 0)
        {
            for (int k = 0; k < a_nSubdivisions; k++)
                AddQuad(nextPoints[(k + 1) % a_nSubdivisions], nextPoints[k], points[(k + 1) % a_nSubdivisions], points[k]);
        }

        //store nextPoints into points
        for (int k = 0; k < a_nSubdivisions; k++)
            points[k] = nextPoints[k];
    }

    delete[] radii;
    delete[] points;
    delete[] nextPoints;

	//Your code ends here
	CompileObject(a_v3Color);
}