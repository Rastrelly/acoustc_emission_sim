#pragma once
#include <iostream>
#include <vector>
#include "ae_openGLOps.h"

struct coord { double x, y; };

class aPartEmitter
{
private:
	int part_wave; //how many particles to spawn at once
	double wave_freq; //main frequency, i.e., how often to spawn waves
	double receive_period; //how long to accum data before dumping it
	std::vector<double> old_data; //accumulated data
	std::vector<double> c_data; //current data that is being
	//gathered at current period
	double timevar; //timer var extracted from world timer
	oglPt p1, p2; //emitter extent
};