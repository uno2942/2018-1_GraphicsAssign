#include "Draw.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#define PI 3.1415926535
#include <cmath>
void myReshape(int width, int height, CamMode camMode)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lookAtByCamMode(camMode);

}

void display()
{
	representBox(GameManager::getInstance().screen, 255, 255, 255);
	representBox(GameManager::getInstance().enemyBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().playerBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().net, 204, 204, 204); //grey
	representCircle(GameManager::getInstance().ball);


	glutSwapBuffers();

}

void representBox(Box box, int colorR, int colorG, int colorB)
{
	float x = box.GetCurrentPosition().x;
	float y = box.GetCurrentPosition().y;
	float w = box.GetSize().x;
	float h = box.GetSize().y;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(colorR / 255.0, colorG / 255.0, colorB / 255.0);
	glBegin(GL_LINE_LOOP);

	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

void representCircle(Circle circle)
{
	int lineNum = 100; // lineNum각형 으로 근사

	float x = circle.GetCurrentPosition().x;
	float y = circle.GetCurrentPosition().y;
	float r = circle.GetSize().x / 2;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 153.0 / 255, 153.0 / 255);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i <= lineNum; i++)
	{
		glVertex2f(x + r * sinf(i * PI * 2 / lineNum), y + r * cosf(i * PI * 2 / lineNum));
	}

	glEnd();
}

void lookAtByCamMode(CamMode camMode)
{
	Vector2 screenSize = GameManager::getInstance().screen.GetSize();

	switch (camMode)
	{
	case FOLLOWBALL:

		break;

	default:

		gluLookAt(screenSize.x, screenSize.y, 100.0f, 0.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f);
		break;
	}

}
