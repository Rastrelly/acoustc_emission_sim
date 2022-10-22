#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <vector>
#include <cmath>
#include <GL/freeglut.h>
#include "ae_openGLOps.h"

class aParticle
{
private:
	oglPt cPos, //current location
		pPos; //previous location
	oglPt crd;

	glm::vec2 forw;

	double ftime;
	double amp;
	float spd;

	double dt;
public:
	aParticle(oglPt pos, glm::vec2 dir);
	void move();
	void die() { dead = true; };
	void setSpeed(float val) { spd = val; }
	void setDt(double val) { dt = val; }

	void drawMyself(float r, int sides);

	oglPt getCPos() { return cPos; };
	oglPt getPPos() { return pPos; };

	bool dead;
};
