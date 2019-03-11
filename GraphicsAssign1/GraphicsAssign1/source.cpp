#include <glut.h> /* this includes the others */

void myReshape(int w, int h) {
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);
	glRectf(10.0, 10.0, 90.0, 90.0);
	glutSwapBuffers();
}
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow((const char*)"simple");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
