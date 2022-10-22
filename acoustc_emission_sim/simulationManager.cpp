#include "simulationManager.h"

simulationManager::simulationManager()
{
	mainEmitter = new aPartEmitter();
	timeSack = 0;
}

void simulationManager::setWorldTimerRef(worldTimer * ref)
{ 
	wtRef = ref;
	mainEmitter->setWorldTimerRef(ref);
};

void simulationManager::spawnParticle(oglPt point, glm::vec2 dir)
{
	particles.push_back(aParticle(point,dir));
	int lp = particles.size() - 1;
	particles[lp].setSpeed(10);//set this up properly later
	//std::cout << "Particle " << lp << "spawned...\n";
}

void simulationManager::processParticles()
{
	int l = particles.size();
	if (l == 0) return;
	
	//go through all the particles and see
	//if there is any separator they are intersecting
	//if they doo, kill the particle and spawn 2 new ones -
	//one that went through and one that got reflected
	//modify amplitude data accordingly
	for (int i = 0; i < l; i++)
	{
		if (!particles[i].dead) //obv. ignore dead particles
		{
			particles[i].setDt(wtRef->getDeltaTime());
			particles[i].move();
			bool isInt = false;
			oglPt intPt;
			int sepl = separators.size();
			if (sepl > 0)
				for (int j = 0; j < sepl; j++)
				{
					checkIntersection(
						particles[i].getPPos(),
						particles[i].getCPos(),
						separators[j].getP1(),
						separators[j].getP2(),
						isInt,
						intPt
					);
				}

			if (isInt)
			{
				//write here what to do if intersecting
			}
		}
	}
}

void simulationManager::checkIntersection(oglPt p1_1, oglPt p1_2, oglPt p2_1, oglPt p2_2, bool &isIntersected, oglPt &ip)
{

}

void simulationManager::runSimIteration()
{
	//spawn new particles
	timeSack += wtRef->getDeltaTime();
	double wavePer = 1 / mainEmitter->getFreq();
	if (timeSack >= wavePer)
	{
		timeSack = 0;
		spawnParticleWave();
	}

	//do the moving and stuff
	processParticles();
}

int simulationManager::emitterId()
{
	for (int i = 0; i < separators.size(); i++)
	{
		if (separators[i].isEmitter) return i;
	}
	return -1;
}

void simulationManager::spawnParticleWave()
{
	int eid = emitterId(); //get emitter id
	int np = mainEmitter->getPartWave();

	oglPt ept1 = separators[eid].getP1();
	oglPt ept2 = separators[eid].getP2();

	float fdx = (ept2.x - ept1.x) / (float)np;
	float fdy = (ept2.y - ept1.y) / (float)np;

	for (int i = 0; i <= np; i++)
	{		
		oglPt cSpawnPt = { ept1.x + fdx*(float)i, ept1.y + fdy * (float)i };
		glm::vec2 cSpawnDir = separators[eid].getNormal();
		spawnParticle(cSpawnPt, cSpawnDir);
	}

	
}