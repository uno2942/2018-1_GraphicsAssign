#define BVIEW_HALF_W 400
#define BVIEW_HALF_H 225

#include "Draw.h"
#include<glew.h>
#include<freeglut.h>
#define PI 3.1415926535
#include <cmath>

void myReshape(int width, int height)
{	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
	//������ �κ��� �Ʒ�������: ���� �����ϱ� ���� ���� ������ ������ ī�޶� �������� ��
	
	//lookAtByCamMode(camMode);
}

void display()
{
	if (ReshapeFlag) {
		GameManager::getInstance().FreshTime();
		ReshapeFlag = !ReshapeFlag;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//
	
	if(camMode == WHOLE || GameManager::getInstance().WhoFinallyWin != 0)
		gluOrtho2D(-100, 1700, -100, 1000);
	else
		gluOrtho2D(-BVIEW_HALF_W - 100, BVIEW_HALF_W + 100, -BVIEW_HALF_H - 100, BVIEW_HALF_H + 100);
		

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (GameManager::getInstance().WhoFinallyWin != 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		representResult();
		glLoadIdentity();
		glutSwapBuffers();
		return;
	}
	
	if (camMode == WHOLE)
		gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f);
	else
		lookAtBall(*GameManager::getInstance().ball);
	
	


	glClear(GL_COLOR_BUFFER_BIT);
	
	vector<GameObjectTree>::iterator iter; 
	
	for (iter = objectsTreeVectorForDraw.begin(); iter != objectsTreeVectorForDraw.end(); iter++)
	{
		
		drawObjectRecursive(iter->root);
	}
	


	/*
	representBox(GameManager::getInstance().screen, 255, 255, 255);
	representBox(GameManager::getInstance().enemyBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().playerBox, 255, 255, 0); //yellow
	representBox(GameManager::getInstance().net, 204, 204, 204); //grey
	representCircle(GameManager::getInstance().ball);
	*/
	glLoadIdentity();
	representScore(GameManager::getInstance().myScore, 100.0, 800.0);
	glLoadIdentity();
	representScore(GameManager::getInstance().enemyScore, 1400.0, 800.0);
	

	glutSwapBuffers();

}

void drawObjectRecursive(GameObjectNode* root)
{
	glPushMatrix();
	//��ġ�̵�
	glTranslatef(root->data->object->position.x, root->data->object->position.y, 0.0f);

	//ȸ�������� �̵��� �� ȸ��, �ǵ�����
	glTranslatef(root->data->object->rotationAxis.x, root->data->object->rotationAxis.y, 0.0f);
	glRotatef(root->data->object->rotation, 0, 0, 1);
	glTranslatef(- root->data->object->rotationAxis.x, - root->data->object->rotationAxis.y, 0.0f);

	representComponent(*(root->data->object));

	GameObjectNode *tmp;

	for (tmp = root->successor; tmp != NULL; tmp = tmp->sibling)
	{
		
		drawObjectRecursive(tmp);
		
	}
	
	glPopMatrix();
}

void representComponent(const Transform &object)
{
	if (object.shape == Object::OVAL)
		representCircle(object);
	else if (object.shape == Object::BOX)
		representBox(object);
	else
		representTriangle(object);
}



void representBox(const Transform& box)
{
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat w = box.GetSize().x;
	GLfloat h = box.GetSize().y;
	
	
	glColor4f(box.myColor.r / 255.0, box.myColor.g / 255.0, box.myColor.b / 255.0, box.myColor.a);
	glBegin(GL_QUADS);

	glVertex2f(x, y);
	glVertex2f(x, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y);
	glEnd();
}

void representCircle(const Transform &circle)
{
	int lineNum = 100; // lineNum���� ���� �ٻ�

	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat r_x = circle.GetSize().x / 2;
	GLfloat r_y = circle.GetSize().y / 2;

	
	glColor4f(circle.myColor.r / 255.0, circle.myColor.g / 255.0, circle.myColor.b / 255.0, circle.myColor.a);
	glBegin(GL_POLYGON);

	for (int i = 0; i <= lineNum; i++)
	{
		glVertex2f(x + r_x + r_x * sinf(i * PI * 2 / lineNum), y + r_y + r_y * cosf(i * PI * 2 / lineNum));
	}

	glEnd();
}

void representTriangle(const Transform & triangle)
{
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat w = triangle.GetSize().x;
	GLfloat h = triangle.GetSize().y;

	
	glColor4f(triangle.myColor.r / 255.0, triangle.myColor.g / 255.0, triangle.myColor.b / 255.0, triangle.myColor.a);
	glBegin(GL_TRIANGLES);

	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w / 2, y + h);
	glEnd();
}

void lookAtBall(const Object& ball) // to be modified
{

	GLfloat lookAtX = ball.GetCurrentPosition().x + ball.GetSize().x / 2;
	GLfloat lookAtY = ball.GetCurrentPosition().y + ball.GetSize().y / 2;
	GLfloat screenX = WORLDCOORDWINDOWWIDTH;
	GLfloat screenY = WORLDCOORDWINDOWHEIGHT;
	
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
	printf("%f\n", lookAtX);
}

void representScore(int score, GLfloat x, GLfloat y)
{
	glTranslatef(x, y, 0.0f);
	glScalef(0.5f, 0.5f, 1.0f);
	char s[5];
	_itoa_s(score, s, 10);

	if (score / 10 > 0) {
		_itoa_s(score / 10, s, 10);
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[0]);
	}

	_itoa_s(score % 10, s, 10);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[0]);
	/*
	for (int i = 0; i < strlen(s); i++)
	{
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&s[i]);
	}*/


}

void representResult(void)
{
	glLoadIdentity();

	glTranslatef(100.0f, 600.0f, 0.0f);
	glScalef(1.5f, 1.5f, 1.0f);
	char winMessage[8] = "YOU WIN";
	char loseMessage[9] = "YOU LOSE";

	if (GameManager::getInstance().WhoFinallyWin == 1) 
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&winMessage);
	else if (GameManager::getInstance().WhoFinallyWin == 2) 
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)&loseMessage);
		
}