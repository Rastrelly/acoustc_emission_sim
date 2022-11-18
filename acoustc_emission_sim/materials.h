#pragma once

#include <string>

struct materialprops
{
	double c_r, //speed of sound for shift wave
		c_l, //speed of sound for straight wave
		atten_c, // coefficient of energy loss
		refr_c; //coeff for angle of refraction
	std::string name;
	oglClr colour;
};