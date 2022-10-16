#pragma once
#include <iostream>
#include "emitter_entity.h"
#include "ae_openGLOps.h"
#include "separator_line_entity.h"

class aParticle
{
private:
	oglPt cPos, //current location
		pPos; //previous location
	coord crd;
	double ftime;
	double amp;
public:
	aParticle(coord pos);

};
