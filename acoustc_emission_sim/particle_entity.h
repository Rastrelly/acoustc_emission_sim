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
#include "materials.h"

class aParticle
{
private:
	oglPt cPos, //current location
		pPos; //previous location
	oglPt crd;

	glm::vec2 forw;

	double ftime;
	double b_amp, r_amp, d_amp;
	float spd;

	float minDelta;

	double myDt;
	double lifetime;
	double atten;

	double path_l;

	double ecoeff;

	materialprops myMat;

	bool skipIntCheck;
public:
	aParticle(oglPt pos, glm::vec2 dir);
	void move();
	void die() { dead = true; };
	void setSpeed(float val) { spd = val; }
	void setSpd(float val) { spd = val; }
	void setDt(double val) { myDt = val; }
	void resetDt() { myDt = 0; };
	void setForw(glm::vec2 val) { forw = val; }
	void setCPos(oglPt val) { cPos = val; }
	void setPPos(oglPt val) { pPos = val; }
	void setLifetime(double val) { lifetime = val; }
	void setAtten(double val) { atten = val; }

	void setBAmp(double val) { b_amp = val; };
	void setRAmp(double val) { r_amp = val; };
	double getBAmp() { return b_amp; };
	double getRAmp() { return r_amp; };

	void setPath(double val) { path_l = val; };
	double getPath() { return path_l; };

	void setECoeff(double val) { ecoeff = val; };
	double getECoeff() { return ecoeff; };

	float getSpd() { return spd; };
	
	void drawMyself(float r, int sides);

	oglPt getCPos() { return cPos; };
	oglPt getPPos() { return pPos; };
	glm::vec2 getForw() { return forw; }

	void setMaterial(materialprops val) { myMat = val; }
	materialprops getMaterial() { return myMat; }

	bool dead;

	bool getSkipIntCheck() { return skipIntCheck; };
	void setSkipIntCheck(bool val) { skipIntCheck = val; };
};
