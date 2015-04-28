#include <ctime>
#include <cstdlib>
#include <glut.h>
#include <Windows.h>
#include <vector>
#include "TerrainGen.h"
#include "Bot.h"

#define MAX_BOTS 50

using namespace std;

const int w = 1500, h = 700;
vector<Bot> bots;

void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// draw
	render();
	/*for(Bot &bot: bots){
		bot.update();
		bot.render();*/
	for(int i = 0; i < MAX_BOTS; i++){
		//bots[i].update();
		bots[i].render();
		printf("Debug: %d\t", i);
	}
	//
	glFlush();
	glutSwapBuffers(); // for smoother animation
	//slow it down
	Sleep(250);
}

//idle function
void cb_idle() {
	glutPostRedisplay();
}

//reshape window function
void cb_reshape(int w, int h) {
	int aspect = w/h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, aspect, 0.0005, 1000); // necessary to preserve aspect ratio
	glMatrixMode(GL_MODELVIEW);
}

//spin the screen based on mouse clicks
void cb_mouseclick(int button, int state, int x, int y) {
	return;	
}

//don't need this one
void cb_mousemove(int x, int y) {
	return;
}

//call the right function and do the right calculations for all of the defined key functions
void cb_keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'b':
	case 'B':
		toggleBlocky();
		break;
	case 'c':
	case 'C':
		exit(0);
		break;
	}
	return;
}

//monster setup function.  open GLUT, create the engine, set rendering preferences, create lights, 
//send funcions to glut, and print the menu
void setupGlut(int& argc, char* argv[]){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutCreateWindow("Evolution");
	glutIgnoreKeyRepeat(false);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glCullFace(GL_FRONT_AND_BACK);
	//lighting
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	float rgb = 0.75f;
	glClearColor(rgb, rgb, rgb, 0);
	glutMotionFunc(cb_mousemove);
	glutMouseFunc(cb_mouseclick);
	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);

	srand(time(NULL));
	createWorld();
	for(int i = 0; i < MAX_BOTS; i++){
		Bot* b = new Bot();
		bots.push_back( *b );
	}
	printf("Debug: %d\n", bots.size());
}