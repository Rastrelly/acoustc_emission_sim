#pragma once

#include <string> 
#include <vector> 
#include <glm/glm.hpp>  //using GLM for vectors
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/matrix.hpp>
#include "ae_openGLOps.h"

struct materialprops
{
	double c_r, //speed of sound for shift wave
		c_l, //speed of sound for straight wave
		atten_c, // coefficient of energy loss
		refr_c; //coeff for angle of refraction
	std::string name;
};

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
public:
	void setP1(oglPt p) { p1 = p; }
	void setP2(oglPt p) { p2 = p; }
	void setMaterial1(materialprops p) { material1 = p; }
	void setMaterial2(materialprops p) { material2 = p; }
	void setFlipNormals(bool fn) { flip_normal = fn; }
	void calcNormal();
	void drawMyself();
};

void addSeparator(std::vector<sepLineEnt> &seps, oglPt p1, oglPt p2, materialprops material1, materialprops material2, bool flipNormals);