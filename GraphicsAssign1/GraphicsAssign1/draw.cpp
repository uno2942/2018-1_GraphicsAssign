#include <cmath>
#include "Draw.h"
#include<algorithm>
#include "objloader.h"
#include"myShader.h"
#define PI 3.1415926535

#define BVIEW_HALF_W 400
#define BVIEW_HALF_H 225
#define WALL_VERTEX_NUM 121 //= 11* 11

using namespace std;

map<string, GLuint> VAO_map;
enum stringToInt {LEFTWALL, RIGHTWALL, FRONTWALL, BACKWALL, BOTTOMWALL, BALL, PLAYER, ENEMY};
static map< string, int > mappingFromStringToInt;

static string ballObjPath = "sphere.obj";
static string playerObjPath = "";
static string enemyObjPath = "";

void myReshape(int width, int height)
{	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
	//나머지 부분을 아래쪽으로: 공을 추적하기 위해 공이 움직일 때마다 카메라 움직여야 함
	
	//lookAtByCamMode(camMode);
}

void display()
{
	static glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 1);
	static glm::vec4 polygonInnerColor = glm::vec4(0.5, 0.5, 0.5, 1);
	static glm::vec4 lineColor = glm::vec4(1, 1, 1, 1);
	Transform* player = GameManager::getInstance().player;
	Transform* enemy = GameManager::getInstance().enemy;
	Transform* ball = GameManager::getInstance().ball;


	if (ReshapeFlag) {
		GameManager::getInstance().FreshTime();
		ReshapeFlag = !ReshapeFlag;
	}

	/* 여긴 좀 해주세요.
	if (GameManager::getInstance().WhoFinallyWin != 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		representResult();
		glLoadIdentity();
		glutSwapBuffers();
		return;
	}
	*/

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT); //Depth도 넣어야 하는 건지 확인 필요

	glUseProgram(MyShader::GetShader());


	if (camMode == CamMode::CHARACTER) {
		glm::vec3 camerapos = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z);
		glm::vec3 cameradirection = camerapos + glm::vec3(0, 0, 1);
		glm::vec3 up = glm::vec3(0, 1, 0);
		MyShader::setMat4("View", glm::lookAt(camerapos, cameradirection, up));
	}
	glm::mat4 trans;
	glm::vec3 unitpos;
	Transform* unit;
	string objPath;
	glm::vec3 rotationAxis;
	switch (renMode) {
	case NO_HIDDEN_LINE_REMOVAL:

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		MyShader::setVec4("myColor", lineColor);
		for (map<string, GLuint>::iterator iter = VAO_map.begin(); iter != VAO_map.end(); ++iter) {
			switch (mappingFromStringToInt[(*iter).first]) {
			case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, WALL_VERTEX_NUM, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case BALL:
				
				trans = glm::identity<glm::mat4>();
				unitpos = glm::vec3(ball->GetCurrentPosition().x, ball->GetCurrentPosition().y, ball->GetCurrentPosition().z);
				trans = glm::translate(trans, unitpos); // 제대로 쓴거 맞나?
				
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->vertexSize , GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY:
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				trans = glm::identity<glm::mat4>();
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::translate(glm::rotate(trans, glm::radians((float)unit->rotation), rotationAxis), unitpos));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->vertexSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		break;

	case HIDDEN_LINE_REMOVAL:
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		MyShader::setVec4("myColor", lineColor);

		for (map<string, GLuint>::iterator iter = VAO_map.begin(); iter != VAO_map.end(); ++iter) {
			switch (mappingFromStringToInt[(*iter).first]) {
			case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, WALL_VERTEX_NUM, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case BALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->vertexSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY:
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				trans = glm::identity<glm::mat4>();
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::translate(glm::rotate(trans, glm::radians((float)unit->rotation), rotationAxis), unitpos));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->vertexSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);

		MyShader::setVec4("myColor", polygonInnerColor);

		for (map<string, GLuint>::iterator iter = VAO_map.begin(); iter != VAO_map.end(); ++iter) {
			switch (mappingFromStringToInt[(*iter).first]) {
			case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, WALL_VERTEX_NUM, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case BALL:
				MyShader::setMat4("Model", glm::translate(glm::mat4(), glm::vec3(ball->position.x, ball->position.y, ball->position.z)));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->vertexSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY:
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				trans = glm::identity<glm::mat4>();
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::translate(glm::rotate(trans, glm::radians((float)unit->rotation), rotationAxis), unitpos));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->vertexSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		glDisable(GL_POLYGON_OFFSET_FILL);

		//이게 맞는 순서인지는 잘 모르겠습니다.
	}
	/* 스코어는 해주세요.
	glLoadIdentity();
	representScore(GameManager::getInstance().myScore, 100.0, 800.0);
	glLoadIdentity();
	representScore(GameManager::getInstance().enemyScore, 1400.0, 800.0);
	*/

	glutSwapBuffers();

}

/*
void drawObjectRecursive(GameObjectNode* root)
{
	glPushMatrix();
	//위치이동
	glTranslated(root->data->object->position.x, root->data->object->position.y, 0.0);

	//회전축으로 이동한 후 회젼, 되돌리기
	glTranslated(root->data->object->rotationAxis.x, root->data->object->rotationAxis.y, 0.0);
	glRotated(root->data->object->rotation, 0., 0., 1.);
	glTranslated(- root->data->object->rotationAxis.x, - root->data->object->rotationAxis.y, 0.0);

	representComponent(*(root->data->object));

//	GameObjectNode *tmp;
	if (root->successor != NULL) drawObjectRecursive(root->successor);
	glPopMatrix();

	if (root->sibling != NULL) drawObjectRecursive(root->sibling);
}
*/




void genPolygonVAO(const Transform *object, string objPath) // size normalized, 1 * 1 * 1
{
	ObjData* drawingObjData;
	
	//load image
	drawingObjData = GetObj(objPath);

	//shader drawing
	float* scaledVertexArray = new float[drawingObjData->vertexSize];

	for (int i = 0; i < drawingObjData->vertexSize; i++) {
		switch (i % 3) {
		case 0: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().x;
		case 1: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().y;
		case 2: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().z;
		}
	}

	// starts to use

	// make buffers
	GLuint VBO, VAO, EBO; //each vertex buffer, vertex array, Elemental buffer

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(drawingObjData->vertices), scaledVertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawingObjData->triangleArray), drawingObjData->triangleArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	string polygonName = object->name;
	for(int i = 0; i < polygonName.size(); i++) {
		polygonName[i] = toupper(polygonName[i]);
	}
	

	VAO_map.insert(map<string, GLuint>::value_type(polygonName, VAO));
	delete[] scaledVertexArray;
	cout << "GenPolygon Working" << endl;
}

void genVAO() { // bind .obj path for each object
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		switch (VAO_map[objectsTreeVectorForDraw[i].name])
		{
			objectsTreeVectorForDraw[i].root->data;
		case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
			genWallVAO(objectsTreeVectorForDraw[i].root->data->object);
				break;
		case BALL: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, ballObjPath); break;
		case PLAYER: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, playerObjPath); break;
		case ENEMY: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, enemyObjPath); break;
		default: break;
		}
	}
	cout << "GenVAO working" << endl;
}

void genWallVAO(const Transform* object) // 121 vertics, 200 triangles,
{
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		vector<float> vertices;
		vector<unsigned int> indices;
		

		for (int j = -5; j <= 5; j++)
		{
			for (int k = -5; k < 5; k++) {
				if (object->xlen == 0) {
					vertices.push_back(object->GetCurrentPosition().x);
					vertices.push_back(object->GetCurrentPosition().y + object->ylen * j / 9.0);
					vertices.push_back(object->GetCurrentPosition().z + object->zlen * k / 9.0);
				}
				else if (object->ylen == 0) {
					vertices.push_back(object->GetCurrentPosition().x + object->xlen * j / 9.0);
					vertices.push_back(object->GetCurrentPosition().y);
					vertices.push_back(object->GetCurrentPosition().z + object->zlen * k / 9.0);
				}
				else if (object->zlen == 0) {
					vertices.push_back(object->GetCurrentPosition().x + object->xlen * j / 9.0);
					vertices.push_back(object->GetCurrentPosition().y + object->ylen * k / 9.0);
					vertices.push_back(object->GetCurrentPosition().z);
				}
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 10; k++) {
				indices.push_back(10 * j + k);
				indices.push_back(10 * j + k + 1);
				indices.push_back(10 * j + k + 10);

				indices.push_back(10 * j + k + 1);
				indices.push_back(10 * j + k + 10);
				indices.push_back(10 * j + k + 11);

			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		string wallName = object->name;
		
		VAO_map.insert(map<string, GLuint>::value_type(wallName, VAO));
		cout << "Gen Wall VAO working" << endl;
}


ObjData* GetObj(string path) {
	static map<string, ObjData*> objDataMap; // size of objcet shape flags
	if (objDataMap.find(path) == objDataMap.end() )
	{
		ObjData* objData = new ObjData;
		loadOBJ(path.c_str(), objData);
		objDataMap.insert(pair<string, ObjData*>(path, objData));
		
	}
	return objDataMap.find(path)->second;
}


void SetModelAndViewMatrix(CamMode camMode, GLfloat a, GLfloat b) { //reference: https://heinleinsgame.tistory.com/12
	Transform* player = GameManager::getInstance().player;
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 up; // 수정 필요
	glm::mat4 view;
	glm::mat4 Projection; // 월드 좌표로 표현 수정 필요
	switch(camMode){
	case CHARACTER: 
		cameraPos = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z);
		cameraTarget = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z + 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	case BEHIND:
		cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN/2, 10, 0);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN / 2, 10, 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	case HANGING:
		cameraPos = glm::vec3(a, WORLD_COORD_MAP_YLEN, b);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN/2 ,0, WORLD_COORD_MAP_ZLEN/2);
		glm::vec3 temp1 = glm::normalize(cameraTarget - cameraPos);
		glm::vec3 temp2 = glm::normalize(glm::vec3(a, 0, b));
		glm::mat4 temp3 = glm::identity<glm::mat4>();
		temp3 = glm::rotate(temp3, glm::radians(90.0f), glm::cross(temp1, temp2));
		up = temp3 * glm::vec4(temp1.x, temp1.y, temp1.z, 1);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // 월드 좌표로 표현 수정 필요
		break;
	}

	glUniformMatrix4fv(glGetUniformLocation(MyShader::myshader, "View"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(MyShader::myshader, "Projection"), 1, GL_FALSE, &Projection[0][0]);
	cout << "SetMovelView matrix working" << endl;
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


/*
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
	int lineNum = 100; // lineNum각형 으로 근사

	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat r_x = circle.GetSize().x / 2;
	GLfloat r_y = circle.GetSize().y / 2;


	glColor4f(circle.myColor.r / 255.0, circle.myColor.g / 255.0, circle.myColor.b / 255.0, circle.myColor.a);
	glBegin(GL_POLYGON);

	for (int i = 0; i < lineNum/2; i++)
	{
		glVertex2f(x + r_x + r_x * sinf(i * PI * 2 / lineNum), y + r_y + r_y * cosf(i * PI * 2 / lineNum));

		float colR = (circle.myColor.r * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		float colG = (circle.myColor.g * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		float colB = (circle.myColor.b * (lineNum / 2 - i) + 1 * i) / (float)(lineNum / 2);
		glColor4f(colR, colG, colB, 1);
	}
	for (int i = lineNum/2; i < lineNum; i++)
	{
		glVertex2f(x + r_x + r_x * sinf(i * PI * 2 / lineNum), y + r_y + r_y * cosf(i * PI * 2 / lineNum));

		float colR = (circle.myColor.r * (i - lineNum / 2 ) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		float colG = (circle.myColor.g * (i - lineNum / 2) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		float colB = (circle.myColor.b * (i - lineNum / 2) + 1 * (lineNum - i)) / (float)(lineNum / 2);
		glColor4f(colR, colG, colB, 1);
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
*/

/*void lookAtBall(const Object& ball) // to be modified
{

	GLdouble lookAtX = ball.GetCurrentPosition().x + ball.GetSize().x / 2;
	GLdouble lookAtY = ball.GetCurrentPosition().y + ball.GetSize().y / 2;
	GLdouble screenX = WORLDCOORDWINDOWWIDTH;
	GLdouble screenY = WORLDCOORDWINDOWHEIGHT;

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
}*/
