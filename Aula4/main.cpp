#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

using namespace std;

GLuint vertices, verticeCount;
vector<double> vertexB;
bool vbo = true;
int slices = 50;

int timebase;
double frames;

struct Polar {
	double radius;
	double alpha;
	double beta;
};

Polar camPos = { sqrt(75), M_PI_4, M_PI_4 }; 

double polarX(Polar polar) { return polar.radius * cos(polar.beta) * sin(polar.alpha); }
double polarY(Polar polar) { return polar.radius * sin(polar.beta); }
double polarZ(Polar polar) { return polar.radius * cos(polar.beta) * cos(polar.alpha); }




void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void framerate() {
    char title[50];
    frames++;
    double time = glutGet(GLUT_ELAPSED_TIME);
    
    if (time - timebase> 1000) {
        double fps = frames * 1000.0 / (time - timebase);
        timebase = time;
        frames = 0;
        sprintf(title, "CG@DI-UM | %lf FPS", fps);
        glutSetWindowTitle(title);
    }
}

void genCylinder(double radius, double height, unsigned int slices) {
    vertexB.clear();

	double hh = height / 2;
	double sliceStep = M_PI * 2 / slices;

	Polar baseP = { hh, 0, - M_PI / 2 };
	Polar topP = { hh, 0, M_PI / 2 };

	// BASE
	glVertex3d(polarX(baseP), polarY(baseP), polarZ(baseP));
	for(unsigned int slice = slices; slice > 0; slice--) {
		Polar p = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * slice ,  - atan(hh / radius)};
        Polar pp = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * (slice + 1) ,  - atan(hh / radius)};

        vertexB.push_back(polarX(baseP));
        vertexB.push_back(polarY(baseP));
        vertexB.push_back(polarZ(baseP));

        vertexB.push_back(polarX(pp));
        vertexB.push_back(polarY(pp));
        vertexB.push_back(polarZ(pp));

        vertexB.push_back(polarX(p));
        vertexB.push_back(polarY(p));
        vertexB.push_back(polarZ(p));
	}

	// TOP
	glVertex3d(polarX(topP), polarY(topP), polarZ(topP));
	for(unsigned int slice = 0; slice < slices; slice++) {
		Polar p = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * slice ,  atan(hh / radius)};
        Polar pp = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * (slice + 1) ,  atan(hh / radius)};

        vertexB.push_back(polarX(topP));
        vertexB.push_back(polarY(topP));
        vertexB.push_back(polarZ(topP));

        vertexB.push_back(polarX(p));
        vertexB.push_back(polarY(p));
        vertexB.push_back(polarZ(p));
        
        vertexB.push_back(polarX(pp));
        vertexB.push_back(polarY(pp));
        vertexB.push_back(polarZ(pp));
	}

	// SIDE
	for(unsigned int slice = 0; slice < slices; slice++) {
		Polar pb = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * slice ,  - atan(hh / radius)};
		Polar pt = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * slice ,  atan(hh / radius)};

        Polar ppb = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * (slice + 1) ,  - atan(hh / radius)};
		Polar ppt = { sqrt(pow(hh, 2) + pow(radius,2)) , sliceStep * (slice + 1) ,  atan(hh / radius)};
        
        vertexB.push_back(polarX(pt));
        vertexB.push_back(polarY(pt));
        vertexB.push_back(polarZ(pt));

        vertexB.push_back(polarX(pb));
        vertexB.push_back(polarY(pb));
        vertexB.push_back(polarZ(pb));

        vertexB.push_back(polarX(ppb));
        vertexB.push_back(polarY(ppb));
        vertexB.push_back(polarZ(ppb));

        vertexB.push_back(polarX(ppb));
        vertexB.push_back(polarY(ppb));
        vertexB.push_back(polarZ(ppb));

        vertexB.push_back(polarX(ppt));
        vertexB.push_back(polarY(ppt));
        vertexB.push_back(polarZ(ppt));

        vertexB.push_back(polarX(pt));
        vertexB.push_back(polarY(pt));
        vertexB.push_back(polarZ(pt));
	}

    verticeCount = vertexB.size() / 3;

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(double), vertexB.data(), GL_STATIC_DRAW);
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(polarX(camPos), polarY(camPos), polarZ(camPos), 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

    // put the geometric transformations here


    // put drawing instructions here

    if(vbo) {

        glColor3ub(255,0,255);

        glBindBuffer(GL_ARRAY_BUFFER, vertices);
        glVertexPointer(3, GL_DOUBLE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, verticeCount);

    }
    else {

        glColor3ub(255, 150, 0);

        glBegin(GL_TRIANGLES);
        for(size_t i = 0; i < vertexB.size(); i += 3) {
            glVertex3d(vertexB.at(i), vertexB.at(i+1), vertexB.at(i+2));
        }
        glEnd();
    }

    framerate();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events


void keyboardFunc(unsigned char key, int x, int y) {
	switch(key) {
		case 'a':
			camPos.alpha -= M_PI / 16;
			break;
		case 'd':
			camPos.alpha += M_PI / 16;
			break;
		case 's':
			camPos.beta -= M_PI / 16;
			break;
		case 'w':
			camPos.beta += M_PI / 16;
			break;
        case 'v':
            vbo = vbo ? false : true;
            break;
        case '+':
            slices *= 2;
            genCylinder(1, 2, slices);
            printf("slices = %d\n", slices);
            break;
        case '-':
            slices /= 2;
            genCylinder(1, 2, slices);
            printf("slices = %d\n", slices);
            break;
	}
	if (camPos.alpha < 0) camPos.alpha += M_PI * 2;
	else if (camPos.alpha > M_PI * 2) camPos.alpha -= M_PI * 2;
	if (camPos.beta < - M_PI) camPos.beta += M_PI * 2;
	else if (camPos.beta > M_PI) camPos.beta -= M_PI * 2;
	glutPostRedisplay();
}

int oldx = 0, oldy = 0;

void motionFunc(int x, int y) {
    int deltax = x - oldx;
    int deltay = y - oldy;

    camPos.alpha += 2 * M_PI * deltax / 360;
    camPos.beta += 2 * M_PI * deltay / 360;

    oldx = x;
    oldy = y;

    glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

    glewInit();

    glEnableClientState(GL_VERTEX_ARRAY);

    glGenBuffers(1, &vertices);
    genCylinder(1, 2, slices);

// put here the registration of the keyboard callbacks

	glutKeyboardFunc(keyboardFunc);
    glutMotionFunc(motionFunc);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle

    timebase = glutGet(GLUT_ELAPSED_TIME); 

	glutMainLoop();
	
	return 1;
}
