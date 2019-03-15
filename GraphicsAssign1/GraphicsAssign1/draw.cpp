#define BVIEW_HALF_W 400
#define BVIEW_HALF_H 225

#include "Draw.h"
#include<glew.h>
#include<freeglut.h>
#define PI 3.1415926535
#include <cmath>

CamMode camMode = WHOLE;
bool ReshapeFlag = false;
void myReshape(int width, int height)
{	
	GameManager::getInstance().SavePlayerPositionBeforeReshape();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
	//나머지 부분을 아래쪽으로: 공을 추적하기 위해 공이 움직일 때마다 카메라 움직여야 함
	
	//lookAtByCamMode(camMode);
}

void display()
{
	if (ReshapeFlag) {
		GameManager::getInstance().LoadPlayerPositionBeforeReshape();
		ReshapeFlag = !ReshapeFlag;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//
	
	if(camMode == WHOLE)
		gluOrtho2D(-100, 1700, -100, 1000);
	else
		gluOrtho2D(-BVIEW_HALF_W - 100, BVIEW_HALF_W + 100, -BVIEW_HALF_H - 100, BVIEW_HALF_H + 100);
		

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camMode == WHOLE)
		gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f);
	else
		lookAtBall(GameManager::getInstance().ball);



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
	glBegin(GL_QUADS);

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
	glBegin(GL_POLYGON);

	for (int i = 0; i <= lineNum; i++)
	{
		glVertex2f(x + r + r * sinf(i * PI * 2 / lineNum), y + r + r * cosf(i * PI * 2 / lineNum));
	}

	glEnd();
}


void lookAtBall(const Circle& circle) // to be modified
{

	GLfloat lookAtX = circle.GetCurrentPosition().x + circle.GetSize().x / 2;
	GLfloat lookAtY = circle.GetCurrentPosition().y + circle.GetSize().y / 2;
	GLfloat screenX = GameManager::getInstance().screen.GetSize().x;
	GLfloat screenY = GameManager::getInstance().screen.GetSize().y;

	if (lookAtX > screenX - BVIEW_HALF_W)
		lookAtX = screenX - BVIEW_HALF_W;
	else if (lookAtX < BVIEW_HALF_W)
		lookAtX = BVIEW_HALF_W;

	if (lookAtY > screenY - BVIEW_HALF_H)
		lookAtY = screenY - BVIEW_HALF_H;
	else if (lookAtY < BVIEW_HALF_H)
		lookAtY = BVIEW_HALF_H;


	gluLookAt(lookAtX, lookAtY, 0.0f,
		lookAtX, lookAtY, -100.0f,
		0.0f, 1.0f, 0.0f);
}