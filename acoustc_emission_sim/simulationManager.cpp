#include "simulationManager.h"

simulationManager::simulationManager()
{
	mainEmitter = new aPartEmitter();
	timeSack = 0;
	activeDt = 0.00001;
	maxParticles = 200000;
}

void simulationManager::setWorldTimerRef(worldTimer * ref)
{ 
	wtRef = ref;
	mainEmitter->setWorldTimerRef(ref);
};

void simulationManager::spawnParticle(oglPt point, glm::vec2 dir, float spd)
{
	particles.push_back(aParticle(point,dir));
	int lp = particles.size() - 1;
	particles[lp].setSpeed(spd);//set this up properly later
	//std::cout << "Particle " << lp << " spawned...\n";
}

//void reflect(glm::vec2 &out, const glm::vec2 &incidentVec, const glm::vec2 &normal)
//{
//	out = incidentVec - 2.f * glm::dot(incidentVec, normal) * normal;
//}

void simulationManager::processParticles()
{
	int l = particles.size();
	
	int sepInt=-1;
	
	if (l == 0) return;
	
	//go through all the particles and see
	//if there is any separator they are intersecting
	//if they do, kill the particle and spawn 2 new ones -
	//one that went through and one that got reflected
	//modify amplitude data accordingly

	for (int i = 0; i < l; i++)
	{
		if (i<particles.size())
		if (!particles[i].dead) //obv. ignore dead particles
		{

			particles[i].setDt(activeDt);

			particles[i].move();

			bool isInt = false;
			oglPt intPt = { 0,0 };

			int sepl = separators.size();

			if (sepl > 0)
			{
				if (!particles[i].getSkipIntCheck())
				{
					if (!((particles[i].getCPos().x == particles[i].getPPos().x)
						&& (particles[i].getCPos().y == particles[i].getPPos().y)))
					{
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
							if (isInt)
							{
								sepInt = j;

								glm::vec2 cbase = {0.0f, 1.0f};
								glm::vec2 cnorm = separators[sepInt].getNormal();
								glm::vec2 cforw = particles[i].getForw();

								//intersection angle
								double az1 = acos(glm::dot(cbase, cnorm))*180.0f / boost::math::constants::pi<double>();
								double az2 = acos(glm::dot(cbase, cforw))*180.0f / boost::math::constants::pi<double>();
								double cang = az1 - az2;

								bool intomat2 = false;
																
								if (abs(cang) > 90) 
								{
									//invert received normal if it is opposite
									cnorm = cnorm * (-1.0f);
									intomat2 = true;
								}

								if ((abs(cang) < 26) && (abs(cang) > 0)) particles[i].die();

								float cc = particles[i].getSpd();

								float tc = 100;
								double att = 0;

								materialprops mat1;
								materialprops mat2;

								if (intomat2)
								{
									att = separators[j].getMaterial2().atten_c;
									mat2 = separators[j].getMaterial2();
									mat1 = separators[j].getMaterial1();
									if ((cang == 90) || (cang == 0))
									{
										tc = separators[j].getMaterial2().c_l;
									}
									else
									{
										tc = separators[j].getMaterial2().c_r;
									}
								}
								else
								{
									att = separators[j].getMaterial1().atten_c;
									mat2 = separators[j].getMaterial1();
									mat1 = separators[j].getMaterial2();
									if ((cang == 90) || (cang == 0))
									{
										tc = separators[j].getMaterial1().c_l;
									}
									else
									{
										tc = separators[j].getMaterial1().c_r;
									}
								}

								float refc = 1; //rewrite for Snellius

								if (particles.size() <= maxParticles)
								{
									glm::vec2 nforw = glm::reflect(particles[i].getForw(), cnorm);
									spawnParticle({ intPt.x + nforw.x*0.1, intPt.y + nforw.y*0.1 }, nforw, cc);
									particles[particles.size() - 1].setBAmp(particles[i].getBAmp());
									particles[particles.size() - 1].setECoeff(particles[i].getECoeff()/2);
									particles[particles.size() - 1].setPath(particles[i].getPath());
									particles[particles.size() - 1].setMaterial(mat1);
									//particles[particles.size() - 1].setLifetime(0);
								}

								//glm::vec2 refforw = glm::refract(particles[i].getForw(), cnorm, refc);
								particles[i].setECoeff(particles[i].getECoeff()/2);
								particles[i].setAtten(att);
								//particles[i].setLifetime(0);
								particles[i].setSpd(tc);
								//particles[i].setForw(refforw);
								particles[i].setMaterial(mat2);

								if (separators[j].getTransitKills()) particles[i].die();

								if (separators[j].isEmitter)
								{
									recordData(particles[i].getRAmp());
								}

								particles[i].setSkipIntCheck(true);
								particles[particles.size() - 1].setSkipIntCheck(true);
								//std::cout << "Intersection event! " << sepInt << " collided particle " << i << std::endl;
							}
						}
					}
					else
					{
						particles[i].die();
					}
				}
				else
				{
					particles[i].setSkipIntCheck(false);
				}
			}
		}
	}
}

void simulationManager::garbageCollect()
{
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].dead)
		{
			particles.erase(particles.begin() + i);
		}
	}
}

void simulationManager::checkIntersection(oglPt p1_1, oglPt p1_2, oglPt p2_1, oglPt p2_2, bool &isIntersected, oglPt &ip)
{
	bstPt pt1_1(p1_1.x, p1_1.y);
	bstPt pt1_2(p1_2.x, p1_2.y);
	bstPt pt2_1(p2_1.x, p2_1.y);
	bstPt pt2_2(p2_2.x, p2_2.y);

	//src: https://stackoverflow.com/questions/19867265/using-boost-geometry-to-check-if-two-lines-have-an-intersection
	Segment AB(pt1_1, pt1_2);
	Segment CD(pt2_1, pt2_2);

	std::vector<bstPt> output;
	bool result = boost::geometry::intersection(AB, CD, output);
	
	if ((result) && (output.size()>0))
	{
		float fx = output[0].get<0>();
		float fy = output[0].get<1>();
		ip = { fx, fy };
	}
	else result = false;

	isIntersected = result;
}

void simulationManager::runSimIteration()
{
	//spawn new particles
	timeSack += activeDt;
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
		if (particles.size() <= maxParticles)
		{
			oglPt cSpawnPt = { ept1.x + fdx*(float)i, ept1.y + fdy * (float)i };
			glm::vec2 cSpawnDir = separators[eid].getNormal();
			spawnParticle(
				{ cSpawnPt.x + cSpawnDir.x*0.1,
				cSpawnPt.y + cSpawnDir.y*0.1 },
				cSpawnDir, separators[eid].getMaterial1().c_l
						);
			particles[particles.size() - 1].setAtten(separators[eid].getMaterial1().atten_c);
			particles[particles.size() - 1].setMaterial(separators[eid].getMaterial1());
			particles[particles.size() - 1].setSpd(particles[particles.size() - 1].getMaterial().c_l);
		}
	}
}

void simulationManager::recordData(double c_amt)
{
	mainEmitter->recData(c_amt);
}

void simulationManager::drawCurrChart(std::vector<double> &dvec)
{
	drawChart(dvec);
}

void simulationManager::compactParticles(double r)
{
	int l = particles.size();
	if (l > 1)
	{
		std::vector<int>deathlist = {};
		for (int i = 0; i < l; i++)
		{
			deathlist.clear();
			if (!particles[i].dead)
			{
				oglPt p1 = particles[i].getCPos();
				for (int j = 0; j < l; j++)
				{
					if ((!particles[j].dead) && (j != i))
					{
						oglPt p2 = particles[j].getCPos();
						double d = getDist(p1,p2);
						if (d <= r) deathlist.push_back(j);
					}
					particles[i].setBAmp(particles[i].getBAmp()*deathlist.size());
					for (int k = 0; k < deathlist.size(); k++)
					{
						particles[deathlist[k]].die();
					}
				}
			}
		}
	}
}