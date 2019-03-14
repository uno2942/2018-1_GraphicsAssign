#include "Draw.h"
#include<glew.h>
#include<freeglut.h>
#define PI 3.1415926535
#include <cmath>

CamMode camMode = WHOLE;
void myReshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-100, 1700, -100, 1000);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//lookAtByCamMode(camMode);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	representBox(GameManager::getInstance().screen, 255, 255, 255);
	representBox(GameManager::getInstance().enemyBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().playerBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().net, 204, 204, 204); //grey
	representCircle(GameManager::getInstance().ball);


	glutSwapBuffers();

}

void representBox(const Box& box, int colorR, int colorG, int colorB)
{
	GLfloat x = box.GetCurrentPosition().x;
	GLfloat y = box.GetCurrentPosition().y;
	GLfloat w = box.GetSize().x;
	GLfloat h = box.GetSize().y;

	
	glColor3f(colorR / 255.0, colorG / 255.0, colorB / 255.0);
	glBegin(GL_LINE_LOOP);

	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

void representCircle(const Circle& circle)
{
	int lineNum = 100; // lineNum각형 으로 근사

	GLfloat x = circle.GetCurrentPosition().x;
	GLfloat y = circle.GetCurrentPosition().y;
	GLfloat r = circle.GetSize().x / 2;

	
	glColor3f(1.0, 153.0 / 255, 153.0 / 255);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i <= lineNum; i++)
	{
		glVertex2f(x + r * sinf(i * PI * 2 / lineNum), y + r * cosf(i * PI * 2 / lineNum));
	}

	glEnd();
}

void lookAtByCamMode(CamMode _camMode)
{
	Vector2 screenSize = GameManager::getInstance().screen.GetSize();

	switch (_camMode)
	{
	case FOLLOWBALL:

		break;

	default:

		gluLookAt(screenSize.x, screenSize.y, 100.0f, 0.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f);
		break;
	}

}