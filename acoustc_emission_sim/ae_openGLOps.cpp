#include "ae_openGLOps.h"

void prepOrthoArea(oglPt p1, oglPt p2, float wx, float wy)
{
	float rp1, rp2;
	float xc, yc;
	if (wx > wy)
	{
		xc = wx / wy; yc = 1;
	}
	else
	{
		xc = 1; yc = wy / wx;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(p1.x*xc, p2.x*xc, p1.y*yc, p2.y*yc);
	glMatrixMode(GL_MODELVIEW);
}

void drawLine(oglPt p1, oglPt p2, oglClr clr)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	
	glColor3f(clr.r,clr.g,clr.b);

	
	glBegin(GL_LINES);
	{

		glVertex2d(p1.x, p1.y);
		glVertex2d(p2.x, p2.y);

	}
	glEnd();
	

	glPopMatrix();
}

void drawCircle(oglPt p1, double r, int nSides, oglClr clr)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	glColor3f(clr.r, clr.g, clr.b);

	glBegin(GL_LINE_LOOP);
	{
		for (int i = 0; i < nSides; i++)
		{
			oglPt cirPt = { 0,0 };
			double ang = ((((double)360 / (double)nSides)*(double)i)*3.14) / 180.0;
			cirPt.x = r * std::cos(ang) + p1.x;
			cirPt.y = r * std::sin(ang) + p1.y;
			glVertex2d(cirPt.x, cirPt.y);
		}
	}
	glEnd();

	glPopMatrix();
}

void drawChart(std::vector<double> &indata)
{
	if (indata.size() > 0)
	{

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f,0.0f,0.0f);
		for (int i = 0; i < indata.size(); i++)
		{
			glVertex2d((double)i, indata[i]);
		}
		glEnd();
	}
}

double getDist(oglPt p1, oglPt p2)
{
	return pow(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2), 0.5f);
}

double snellius(double c1, double c2, double cang)
{
	const double pi = 3.1415926f;
	return ((c2*sin(cang * pi / 180.0f))/c1) * (180.0f / pi);
}