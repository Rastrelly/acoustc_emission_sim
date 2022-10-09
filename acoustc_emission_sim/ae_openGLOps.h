#pragma once

#include <cmath>
#include <GL/freeglut.h>

#define CLR_RED { 1.0f, 0.0f, 0.0f }
#define CLR_LIME { 0.0f, 1.0f, 0.0f }
#define CLR_GREEN { 0.0f, 0.6f, 0.0f }
#define CLR_BLUE { 0.0f, 0.0f, 1.0f }
#define CLR_WHITE { 1.0f, 1.0f, 1.0f }
#define CLR_GRAY { 0.5f, 0.5f, 0.5f }
#define CLR_BLACK { 0.0f, 0.0f, 0.0f }

//colour in 3 floats
struct oglClr
{
	float r, g, b;	
};

//point in 2 doubles
struct oglPt
{
	double x, y;
};


void prepOrthoArea(oglPt p1, oglPt p2, float wx, float wy);


//draws a line between 2 points, colour C
void drawLine(oglPt p1, oglPt p2, oglClr clr);

//draws a circle of radius R at coords C with N sides, colour C
void drawCircle(oglPt p1, double r, int nSides, oglClr clr);


