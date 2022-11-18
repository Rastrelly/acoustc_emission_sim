#include "particle_entity.h"


aParticle::aParticle(oglPt pos, glm::vec2 dir)
{
	lifetime = 0;
	crd = pos;
	setPPos(pos);
	setCPos(pos);
	setForw(dir);
	resetDt();
	dead = false;
	minDelta = 0.001f;
	b_amp = 10000.0f;
	r_amp = 10000.0f;
	d_amp = 0.1f;
	atten = 1;
	path_l = 0;
	ecoeff = 1.0f;
	skipIntCheck = true;
}

void aParticle::move()
{	

	lifetime += myDt;
	
	glm::vec2 disp = forw * (float)(spd * myDt);

	path_l += pow(pow(disp.x,2)+ pow(disp.y, 2),0.5);

	r_amp = b_amp * exp(-1.0f * atten * (path_l/50.0f)) * ecoeff;
	if (r_amp > b_amp) r_amp = b_amp;
    
	setPPos(getCPos());

	setCPos({ getCPos().x + disp.x, getCPos().y + disp.y });	

	if (r_amp <= d_amp) die();

	//if (!dead) std::cout << "t = "<< lifetime <<"; A = " << r_amp << "; exp res = " << pow(2.71828, -0.000016f * lifetime)  << std::endl;

	//std::cout << "Moved to: " << getCPos().x << "; " << getCPos().y << " at dt = " << myDt << std::endl;
}

void aParticle::drawMyself(float r,int sides)
{
	drawCircle(getCPos(), r, sides, myMat.colour);
	//drawCircle(getCPos(), r*(r_amp/b_amp), sides, CLR_BLUE);
	//drawCircle(getCPos(), b_amp / 6000.0f, sides, CLR_RED);
	//drawCircle(getCPos(), r*(r_amp / b_amp),sides,CLR_BLUE);
	//drawLine(cPos,pPos,CLR_LIME);
	//drawLine({cPos.x-r,cPos.y-r}, { cPos.x + r,cPos.y + r }, CLR_BLUE);
	//std::cout << "Drawing particle\n";
}