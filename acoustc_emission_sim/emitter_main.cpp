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

#define AE_WINDOW_W 800
#define AE_WINDOW_H 600

#define CIRCLE_SIDES 16

std::atomic<bool> quitstate = false; //guarantees proper
                                     //termination of renderer
worldTimer tmr; //worldTimer object used to track framerate
std::vector<aParticle> particles = {};
std::vector<sepLineEnt> separators = {};
aPartEmitter emitter_main;

std::atomic<float>winx = AE_WINDOW_W; //window size
std::atomic<float>winy = AE_WINDOW_H;

materialprops dummy_mat = { 10,10,0.1,0.1,"Dummy Material" };

//forward declarations
void cbIdle();
void cbReshape(int nw, int nh);
void cbDisplay();
void glThread(int argc, char **argv);


//implementation

//MAIN
int main(int argc, char **argv)
{
	std::thread thrGlThread(glThread, argc, argv);

	addSeparator(separators, { 5,5 }, { 2,-2 }, dummy_mat, dummy_mat, false);
	addSeparator(separators, { 7,7 }, { 4,-8 }, dummy_mat, dummy_mat, true);

	std::string choice = "";
	while (true)
	{

		std::cout << "Select action: 0 - quit\n" << std::endl;
		std::cin >> choice;

		//quit on quit call;
		if (choice == "0")
		{
			quitstate = true;
			break;
		}
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

	prepOrthoArea({ -10,-10 }, {10,10}, winx, winy);

	glClear(GL_COLOR_BUFFER_BIT);

	//render all elements here
	for (int i = 0; i < separators.size(); i++)
	{
		separators[i].drawMyself();
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


