#include<iostream>
#include<glut.h>
#include<glew.h>
#include "Draw.h"

int main(int argc, char** argv) {
	Draw draw;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Pukimun Bullyball");
	glutReshapeFunc(draw.myReshape);
	glutDisplayFunc(draw.display);
	glutIdleFunc(draw.idlefunction);
	glutMainLoop();
}