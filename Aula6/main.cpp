

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int t;
int tw, th;
unsigned char * imageData;

GLuint* vertices;
double vertexCount;



void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTerrain() {

	for(int strip = 0; strip < th-1; strip++) {
		glBindBuffer(GL_ARRAY_BUFFER, vertices[strip]);
		glVertexPointer(3, GL_DOUBLE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount);
	}

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawTerrain();

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

double h(int i, int j) {
	double x = imageData[i * tw + j];
	return x / 255 * 30;
}

void init() {

// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);

	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	imageData = ilGetData();

// 	Build the vertex arrays
	vertices = (GLuint *)calloc(th-1, sizeof(GLuint));
	glGenBuffers(th-1, vertices);

	int halfW = tw / 2;
	int halfH = th / 2;

	vertexCount = 2 * tw;

	for(int n_strip = 0; n_strip < th - 1; n_strip++) {
		std::vector<double> strip;
		for(int j = 0; j < tw; j++) {
			strip.push_back(n_strip - halfH);
			strip.push_back(h(n_strip,j));
			strip.push_back(j - halfW);

			strip.push_back(n_strip + 1 - halfH);
			strip.push_back(h(n_strip + 1,j));
			strip.push_back(j - halfW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertices[n_strip]);
    	glBufferData(GL_ARRAY_BUFFER, strip.size() * sizeof(double), strip.data(), GL_STATIC_DRAW);
	}


// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);

	ilInit();

	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

