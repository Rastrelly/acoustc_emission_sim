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
#include "simulationManager.h"

#define AE_WINDOW_W 800
#define AE_WINDOW_H 600

#define CIRCLE_SIDES 16

std::atomic<bool> quitstate = false; //guarantees proper
                                     //termination of renderer
worldTimer tmr; //worldTimer object used to track framerate
aPartEmitter emitter_main; //emitter entity
simulationManager simMgr(&emitter_main); //contains all simulation-related data and handles communications
//between subsystems


std::atomic<float>winx = AE_WINDOW_W; //window size
std::atomic<float>winy = AE_WINDOW_H;

materialprops dummy_mat = { 10,10,0.1,0.1,"Dummy Material" };

//forward declarations
void cbIdle();
void cbReshape(int nw, int nh);
void cbDisplay();
void glThread(int argc, char **argv);
bool handleChoice();


//implementation

//MAIN
int main(int argc, char **argv)
{

	simMgr.scrArea = { -10,10,-10,10 };

	std::thread thrGlThread(glThread, argc, argv);

	addSeparator(simMgr.separators, { 5,5 }, { 2,-2 }, dummy_mat, dummy_mat, false);
	addSeparator(simMgr.separators, { 7,7 }, { 4,-8 }, dummy_mat, dummy_mat, true);

	std::string choice = "";
	while (true)
	{
		if (handleChoice()) break;
	}

	thrGlThread.join();

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

	prepOrthoArea(
		{ simMgr.scrArea.left, simMgr.scrArea.bottom },
		{ simMgr.scrArea.right, simMgr.scrArea.top },
		winx, winy);

	glClear(GL_COLOR_BUFFER_BIT);

	//render all elements here
	for (int i = 0; i < simMgr.separators.size(); i++)
	{
		simMgr.separators[i].drawMyself();
	}

	glutSwapBuffers();
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

	std::cout << "Select action: \n1 - setup screen;\n0 - quit\n" << std::endl;
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
	return ext;
}
