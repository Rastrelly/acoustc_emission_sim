#include "headerCollection.h"

#define AE_WINDOW_W 800
#define AE_WINDOW_H 600

#define CIRCLE_SIDES 8
#define TT_STRAIGHT 0
#define TT_ANGLED 1

int transmitter_type = TT_ANGLED;

std::mutex lock1;

std::atomic<bool> quitstate = false; //guarantees proper
                                     //termination of renderer
std::atomic<bool> runSim = false;
std::atomic<bool> needRefresh = false;
worldTimer tmr; //worldTimer object used to track framerate
simulationManager simMgr; //contains all simulation-related data and handles communications
//between subsystems

bool drawchart=false;


std::atomic<float>winx = AE_WINDOW_W; //window size
std::atomic<float>winy = AE_WINDOW_H;

//forward declarations
void cbIdle();
void cbReshape(int nw, int nh);
void cbDisplay();
void glThread(int argc, char **argv);
bool handleChoice();
void simThread();
void makeMatDb();
void printState();

materialprops dummy_mat = { 10,10,0.1,0.1,"Dummy Material" };
std::vector<materialprops> matDb = {};

//implementation

//MAIN
int main(int argc, char **argv)
{

	makeMatDb();

	simMgr.setWorldTimerRef(&tmr);


	if (transmitter_type == TT_STRAIGHT)
	{
		//setup emitter
		simMgr.mainEmitter->setupEmitter(10000.0f,0.01f,21,50);
		simMgr.scrArea = { -10,10,-10,10 };

		//emitter-receiver
		addSeparator(simMgr.separators, { -2,4 }, { 2,4 }, matDb[1], matDb[1], true, true, false);
		
		//hull
		addSeparator(simMgr.separators, { -3,5 }, { 3,5 }, matDb[2], matDb[1], false, false, true);
		addSeparator(simMgr.separators, { 3,5 }, { 3,0.4 }, matDb[2], matDb[1], false, false, true);
		addSeparator(simMgr.separators, { 3,0.4 }, { -3,0.4 }, matDb[4], matDb[1], true, false, false);
		addSeparator(simMgr.separators, { -3,0.4 }, { -3,5 }, matDb[2], matDb[1], false, false, true);

		//sample
		addSeparator(simMgr.separators, { -12,0 }, { 12,0 }, matDb[4], matDb[3], false, false, false);
		addSeparator(simMgr.separators, { 12,0 }, { 12,-8 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { 12,-8 }, { -12,-8 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { -12,-8 }, { -12,0 }, matDb[2], matDb[3], false, false, true);

		//flaw
		addSeparator(simMgr.separators, { -1,-6 }, { 1,-6 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { 1,-6 }, { 1,-7 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { 1,-7 }, { -1,-7 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { -1,-7 }, { -1,-6 }, matDb[3], matDb[2], false, false, false);
	}


	if (transmitter_type == TT_ANGLED)
	{
		//setup emitter
		simMgr.mainEmitter->setupEmitter(10000.0f, 0.01f, 21, 50);
		simMgr.scrArea = { -10,10,-10,10 };

		//emitter-receiver
		addSeparator(simMgr.separators, { 2,4 }, { 5,1 }, matDb[1], matDb[1], true, true, false);
		
		//hull
		addSeparator(simMgr.separators, { -1,5 }, { 5.5,5 }, matDb[2], matDb[1], false, false, true);
		addSeparator(simMgr.separators, { 5.5,5 }, { 5.5,0.2 }, matDb[2], matDb[1], false, false, true);
		addSeparator(simMgr.separators, { 5.5,0.2 }, { -1,0.2 }, matDb[4], matDb[1], false, false, false);
		addSeparator(simMgr.separators, { -1,0.2 }, { -1,5 }, matDb[2], matDb[1], false, false, true);

		//sample
		addSeparator(simMgr.separators, { -5,0 }, { -1,0 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { -1,0 }, { 5.5,0 }, matDb[4], matDb[3], false, false, false);
		addSeparator(simMgr.separators, { 5.5,0 }, { 8,0 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { 8,0 }, { 8,-5 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { 8,-5 }, { -5,-5 }, matDb[2], matDb[3], false, false, true);
		addSeparator(simMgr.separators, { -5,-5 }, { -5,0 }, matDb[2], matDb[3], false, false, true);

		//flaw
		double xb=-1, yb=-1;
		addSeparator(simMgr.separators, { xb,yb }, { xb+2,yb-2 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { xb+2,yb-2 }, { xb+2,yb-3 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { xb + 2,yb - 3 }, { xb,yb - 3 }, matDb[3], matDb[2], false, false, false);
		addSeparator(simMgr.separators, { xb,yb - 3 }, { xb,yb }, matDb[3], matDb[2], false, false, false);
	}

	std::thread thrGlThread(glThread, argc, argv);
	std::thread thrSimThread(simThread);

	std::string choice = "";
	while (true)
	{
		if (handleChoice()) break;
	}

	thrGlThread.join();
	thrSimThread.join();

	return 0;
}

//do on each frame
void cbIdle()
{
	tmr.callDtRecalc();
	if (!quitstate)
	{
		cbDisplay();
	}
	else
	{
		glutLeaveMainLoop();
	}	
}

//on window size change
void cbReshape(int nw, int nh)
{
	winx = (float)nw;
	winy = (float)nh;
	glViewport(0,0,nw,nh);
}

//redisplay caller
void cbDisplay()
{
	//lock1.lock();
	if (needRefresh)
	{
		needRefresh = false;
		
		if (!drawchart)
		{
			prepOrthoArea(
				{ simMgr.scrArea.left, simMgr.scrArea.top },
				{ simMgr.scrArea.right, simMgr.scrArea.bottom },
				winx, winy);

			glClear(GL_COLOR_BUFFER_BIT);

			//render all elements here
			for (int i = 0; i < simMgr.separators.size(); i++)
			{
				simMgr.separators[i].drawMyself();
			}

			for (int i = 0; i < simMgr.particles.size(); i++)
			{
				simMgr.particles[i].drawMyself(0.1f, CIRCLE_SIDES);
			}
		}
		if (drawchart)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			std::vector<double> indata = {};
			simMgr.mainEmitter->getOldData(indata);

			if (indata.size() == 0)
			{
				simMgr.mainEmitter->getcData(indata);
			}

			indata.resize(simMgr.mainEmitter->getnTwinks());

			float l = 0;
			float r = 0;
			float b = 0;
			float t = 0;

			glClear(GL_COLOR_BUFFER_BIT);

			if (indata.size() > 0)
			{

				l = -1;
				r = indata.size();
				b = indata[0];
				t = indata[0];

				for (int i = 0; i < indata.size(); i++)
				{
					if (b > indata[i]) b = indata[i];
					if (t < indata[i]) t = indata[i];
				}

				t += 1.0f;
				b -= 1.0f;
							   
				gluOrtho2D(l, r, b, t);
				simMgr.drawCurrChart(indata);
			}
		}

		glutSwapBuffers();
		printState();
	}
	//lock1.unlock();
}

//our separate thread for rendering
void glThread(int argc, char **argv)
{
	//GLUT system init block
	glutInit(&argc, argv); //initializing GLUT

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(AE_WINDOW_W, AE_WINDOW_H);

	glutCreateWindow("GLUT WINDOW");

	//OpenGL setup block
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//GLUT launch block
	glutIdleFunc(cbIdle);
	glutReshapeFunc(cbReshape);
	glutDisplayFunc(cbDisplay);

	glutMainLoop();
}

bool handleChoice()
{
	std::string choice;
	bool ext=false;

	std::cin >> choice;

	//quit on quit call;
	if (choice == "0")
	{
		quitstate = true;
		ext = true;
	}

	//setup screen size
	if (choice == "1")
	{
		std::cout << "Input LEFT RIGHT BOTTOM TOP:\n" << std::endl;
		float l = 0, r = 0, b = 0, t = 0;
		std::cin >> l >> r >> b >> t;
		simMgr.scrArea = {l, r, t, b};
	}

	//activate/deactivate sim
	if (choice == "2")
	{
		if (!runSim)
		{
			runSim = true;
			std::cout << "+++ SIMULATION STARTED +++\n";
		}
		else
		{
			runSim = false;
			std::cout << "+++ SIMULATION STOPPED +++\n";
		}
	}

	if (choice == "3")
	{
		drawchart = !drawchart;
	}

	return ext;
}

void simThread()
{
	while(true)
	{
		tmr.callDtRecalc();
		if (runSim)
		{
			simMgr.runSimIteration();
			//simMgr.compactParticles(0.000000000001f);
			simMgr.garbageCollect();
			simMgr.mainEmitter->refreshTimeframe(simMgr.getActiveDt());
			needRefresh = true;
		}
		else
		{
			needRefresh = true;
		}
		if (quitstate) break;
	}
}

void makeMatDb()
{
	matDb.clear();
	float spd_down = 1.0f;
	float atn_down = 1.0f;
	matDb.push_back(dummy_mat); //0
	materialprops tmpmat;
	tmpmat.c_r = 2670.0f / spd_down;
	tmpmat.c_l = 2670.0f / spd_down;
	tmpmat.colour = {1.0f, 1.0f, 0.0f}; //yellow is acryl
	tmpmat.name = "Acrylic";
	tmpmat.atten_c = 3.15f / atn_down;
	matDb.push_back(tmpmat); //1
	tmpmat.c_r = 330.0f / spd_down;
	tmpmat.c_l = 330.0f / spd_down;
	tmpmat.colour = { 0.2f, 0.6f, 0.6f }; //blueish is air
	tmpmat.name = "Air";
	tmpmat.atten_c = 6.1f / atn_down;
	matDb.push_back(tmpmat); //2
	tmpmat.c_r = 5940.0f / spd_down;
	tmpmat.c_l = 3250.0f / spd_down;
	tmpmat.colour = { 0.25f, 0.25f, 0.25f }; //gray
	tmpmat.name = "Steel";
	tmpmat.atten_c = 0.09f / atn_down;
	matDb.push_back(tmpmat); //3
	tmpmat.c_r = 1480.0f / spd_down;
	tmpmat.c_l = 1480.0f / spd_down;
	tmpmat.colour = { 0.0f, 1.0f, 0.25f };
	tmpmat.name = "Water";
	tmpmat.atten_c = 0.6f / atn_down;
	matDb.push_back(tmpmat); //4
}

void printState()
{
	system("cls");
	std::cout << "Select action: \n1 - setup screen;\n2 - run/stop sim;\n3 - toggle render/chart\n0 - quit\n" << std::endl;
	std::cout << "Particles in sim: "<< simMgr.particles.size() << std::endl;
	std::cout << "t = " << simMgr.mainEmitter->getTimeVar() << "; ct = " <<
		simMgr.mainEmitter->getcTwink() << " / " << simMgr.mainEmitter->getnTwinks() << std::endl;
}