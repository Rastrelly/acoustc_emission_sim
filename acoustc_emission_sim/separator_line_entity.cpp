#include "separator_line_entity.h"

void sepLineEnt::calcNormal()
{
	glm::vec2 delta(p2.x - p1.x, p2.y - p1.y);
	glm::vec2 p_norm(-1*delta.y,delta.x);
	glm::vec2 p_norm_n=glm::normalize(p_norm);
	if (flip_normal)
	{
		p_norm_n.x = p_norm_n.x * -1;
		p_norm_n.y = p_norm_n.y * -1;
	}
	norm = p_norm_n;
}

void sepLineEnt::drawMyself()
{
	glm::vec2 delta(p2.x - p1.x, p2.y - p1.y);
	oglPt mid = {p1.x + delta.x/2, p1.y + delta.y/2};
	drawLine(p1,p2,CLR_BLUE);
	drawLine(mid, {mid.x+norm.x,mid.y + norm.y }, CLR_GREEN);
}

void addSeparator(std::vector<sepLineEnt>&seps, oglPt p1, oglPt p2, materialprops material1, materialprops material2, bool flipNormals)
{
	seps.push_back(sepLineEnt()); //add an object to work on
	seps[seps.size() - 1].setP1(p1);
	seps[seps.size() - 1].setP2(p2);
	seps[seps.size() - 1].setMaterial1(material1);
	seps[seps.size() - 1].setMaterial2(material2);
	seps[seps.size() - 1].setFlipNormals(flipNormals);
}