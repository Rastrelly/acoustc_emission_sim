#include "particle_entity.h"

aParticle::aParticle(oglPt pos, glm::vec2 dir)
{
	crd = pos;
	pPos = pos;
	cPos = pos;
	forw = dir;
	dead = false;
}

void aParticle::move()
{
	pPos = cPos;
	glm::vec2 disp = forw * (float)(spd * dt);
	cPos.x += disp.x;
	cPos.y += disp.y;
	//std::cout << "Moved to: " << cPos.x << "; " << cPos.y << std::endl;
}

void aParticle::drawMyself(float r,int sides)
{
	drawCircle(cPos,r,sides,CLR_BLUE);
	//drawLine({cPos.x-r,cPos.y-r}, { cPos.x + r,cPos.y + r }, CLR_BLUE);
	//std::cout << "Drawing particle\n";
}