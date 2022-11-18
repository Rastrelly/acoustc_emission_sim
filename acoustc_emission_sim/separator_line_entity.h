#pragma once

#include <string> 
#include <vector> 
#include "ae_openGLOps.h"
#include "materials.h"



class sepLineEnt
{
private:
	//location of line points
	oglPt p1, p2; //line points
	glm::vec2 norm; //normal
	//mat1 is at the side BEHIND normal, 
	//mat 2 is at the side where normal is pointing
	materialprops material1, material2;
	bool is_receiver; //defines if this line registers incoming particles
	bool flip_normal;
	bool transitKills;
public:
	void setP1(oglPt p) { p1 = p; }
	void setP2(oglPt p) { p2 = p; }
	void setMaterial1(materialprops p) { material1 = p; }
	void setMaterial2(materialprops p) { material2 = p; }
	materialprops getMaterial1() { return material1; }
	materialprops getMaterial2() { return material2; }
	void setFlipNormals(bool fn) { flip_normal = fn; }
	void calcNormal();
	void drawMyself();
	bool isEmitter;

	bool getTransitKills() { return transitKills; };
	void setTransitKills(bool val) { transitKills = val; };

	oglPt getP1() { return p1; };
	oglPt getP2() { return p2; };

	glm::vec2 getNormal() { return norm; };
};

void addSeparator(std::vector<sepLineEnt> &seps, oglPt p1, oglPt p2, materialprops material1, materialprops material2, bool flipNormals, bool isEmit, bool transKill);