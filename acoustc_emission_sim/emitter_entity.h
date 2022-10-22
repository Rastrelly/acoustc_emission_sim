#pragma once

#include <vector>
#include "ae_openGLOps.h"
#include "particle_entity.h"
#include "separator_line_entity.h"
#include "world_timer.h"

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

	int nTwinks; //how many blocks to use to accumulate data

	worldTimer * wtRef;
	
public:
	aPartEmitter();
	void setWorldTimerRef(worldTimer * ref) { wtRef = ref; };
	bool enabled;
	double getFreq() { return wave_freq; }
	int getPartWave() { return part_wave; };
};