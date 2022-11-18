#include "emitter_entity.h"

aPartEmitter::aPartEmitter()
{
	setupEmitter(1000.0f, 0.4f, 10, 50);
}

void aPartEmitter::setupEmitter(float wf, float recperiod, int partwav, int nTw)
{
	enabled = false;
	wave_freq = wf;
	receive_period = recperiod;
	part_wave = partwav;
	nTwinks = nTw;
	cTwink = 0;
	dTwink = (float)receive_period / (float)nTwinks;
}

void aPartEmitter::recData(double c_amt)
{
	if(cTwink<c_data.size())
	c_data[cTwink] = c_amt;
	else
	{
		c_data.resize(cTwink+1);
		c_data[cTwink] = c_amt;
	}
}

void aPartEmitter::dumpData()
{
	timevar = 0.0f;
	cTwink = 0;
	old_data.resize(0);
	if (c_data.size() > 0)
	{
		for (double c : c_data)
		{
			old_data.push_back(c);
		}
	}
	c_data.clear();
	c_data.resize(nTwinks);
	std::cout << "nt=" << old_data.size() << std::endl;
}

void aPartEmitter::refreshTimeframe(double dt)
{
	timevar += dt;
	if (timevar > (double)cTwink*(double)dTwink)
	{
		cTwink++;
	}
	if (cTwink > nTwinks - 1) dumpData();
}