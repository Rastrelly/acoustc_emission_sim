#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <vector>
#include <cmath>
#include <GL/freeglut.h>
#include "emitter_entity.h"
#include "particle_entity.h"
#include "world_timer.h"
#include "separator_line_entity.h"
#include "ae_openGLOps.h"


class simulationManager
{	
private:
	double timeSack;
public:

	oglScreen scrArea;

	std::vector<aParticle> particles = {};
	std::vector<sepLineEnt> separators = {};

	aPartEmitter * mainEmitter;
	worldTimer * wtRef;

	simulationManager();
	void setWorldTimerRef(worldTimer * ref);

	void spawnParticle(oglPt point, glm::vec2 dir);
	void spawnParticleWave();

	void processParticles();
	void checkIntersection(oglPt p1_1, oglPt p1_2, oglPt p2_1, oglPt p2_2, bool &isIntersected, oglPt &ip);

	void runSimIteration();
	int emitterId();
};

