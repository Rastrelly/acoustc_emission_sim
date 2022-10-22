#pragma once

#include <chrono>

class worldTimer
{
private:
	std::chrono::steady_clock::time_point lastUpdate;
	double deltaTime;
	double accumTime;
	void calcDeltaTime()
	{
		auto now = std::chrono::steady_clock::now();
		double dt = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
		if (!(dt <= 0.000000001)) //prevent updating on the same frame
		{
			lastUpdate = now;
			deltaTime = dt;
			accumTime += dt;
		}
	}
public:
	worldTimer()
	{
		lastUpdate = std::chrono::steady_clock::now(); //init our timestamp
		deltaTime = 0;
	}
	//public call to recalc deltaTime
	void callDtRecalc()  
	{
		calcDeltaTime();
	}

	//returns the VALUE of deltatime
	double getDeltaTime() 
	{
		return deltaTime;
	}

	//returms the VALUE of accumTime
	double getAccumTime()
	{
		return accumTime;
	}

	//resets accumTime back to 0
	void resetAccumTime()
	{
		accumTime = 0;
	}
};