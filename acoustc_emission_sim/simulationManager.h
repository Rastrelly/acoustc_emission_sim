#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <vector>
#include <GL/freeglut.h>
#include "emitter_entity.h"
#include "particle_entity.h"
#include "world_timer.h"
#include "separator_line_entity.h"
#include "ae_openGLOps.h"


class simulationManager
{	
public:

	oglScreen scrArea;

	std::vector<aParticle> particles = {};
	std::vector<sepLineEnt> separators = {};

	aPartEmitter * mainEmitter;

	simulationManager(aPartEmitter * emitter) { mainEmitter = emitter; };
};

