#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can�t make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set camera
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f);
		
	// put drawing instructions here

	glPushMatrix();
	glRotatef(glutGet(GLUT_ELAPSED_TIME) / 20 % 360, 0.0, 1.0, 0.0);
	glutWireTeapot(0.75 + 0.25 * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0));
	glPopMatrix();

	// End of frame
	glutSwapBuffers();
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
}

void changeColor(int c) {
	switch(c) {
		case 0:
			glColor3ub(75,0,130);
			break;
		case 1:
			glColor3ub(0,0,255);
			break;
		case 2:
			glColor3ub(0,128,0);
			break;
		case 3:
			glColor3ub(255,255,0);
			break;
		case 4:
			glColor3ub(255,165,0);
			break;
		case 5:
			glColor3ub(255,0,0);
			break;
	}
	glutTimerFunc(200, changeColor, (c + 1) % 6);
}


int main(int argc, char** argv)
{
	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Buwule");

	printInfo();
	
	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	
	glutTimerFunc(0, changeColor, 0);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT�s main cycle
	glutMainLoop();
	
	return 1;
}