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
#include <boost/geometry.hpp> 
#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/math/constants/constants.hpp>

typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian>bstPt;
typedef boost::geometry::model::segment<bstPt> Segment;

class simulationManager
{	
private:
	double timeSack;
	double activeDt;
	int maxParticles;
public:

	oglScreen scrArea;

	std::vector<aParticle> particles = {};
	std::vector<sepLineEnt> separators = {};

	aPartEmitter * mainEmitter;
	worldTimer * wtRef;

	simulationManager();
	void setWorldTimerRef(worldTimer * ref);

	void spawnParticle(oglPt point, glm::vec2 dir, float spd);
	void spawnParticleWave();

	void processParticles();

	void garbageCollect();

	void checkIntersection(oglPt p1_1, oglPt p1_2, oglPt p2_1, oglPt p2_2, bool &isIntersected, oglPt &ip);

	void runSimIteration();

	void recordData(double c_amt);

	int emitterId();

	double getActiveDt() { return activeDt; };

	void drawCurrChart(std::vector<double> &dvec);

	void compactParticles(double r);
};

