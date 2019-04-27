#include <cmath>
#include "Draw.h"
#include<algorithm>
#include "objloader.h"
#include"myShader.h"
#define PI 3.1415926535

#define BVIEW_HALF_W 400
#define BVIEW_HALF_H 225

using namespace std;

map<string, GLuint> VAO_map;
enum stringToInt {LEFTWALL, RIGHTWALL, FRONTWALL, BACKWALL, BOTTOMWALL, BALL, PLAYER, ENEMY};
static map< string, int > mappingFromStringToInt;

static string ballObjPath = "sphere.obj";
static string playerObjPath = "Type 2020 Miku.obj";
static string enemyObjPath = "Type 2020 Miku.obj";
void genVAO();
void drawNumVAO(glm::vec2 pos, int num);

unsigned int myVAO, myVAO2;
int WALL_INDICES_NUM;

void myReshape(int width, int height)
{	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
	//������ �κ��� �Ʒ�������: ���� �����ϱ� ���� ���� ������ ������ ī�޶� �������� ��
	
	//lookAtByCamMode(camMode);
}

void display()
{
	static bool isInited = false; //display�� �ϱ� ���� �ʱ�ȭ �ϴ� �κ�. VAO ����� �� ���⼭ �ϸ� ���� �� ���ƿ�.
	if (!isInited) {
		GLenum err = glewInit();
		if (err != GLEW_OK)
			exit(1); // or handle the error in a nicer way
		if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
			exit(1); // or handle the error in a nicer way

		glUseProgram(MyShader::GetShader());
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		mappingFromStringToInt["leftwall"] = LEFTWALL;
		mappingFromStringToInt["rightwall"] = RIGHTWALL;
		mappingFromStringToInt["frontwall"] = FRONTWALL;
		mappingFromStringToInt["backwall"] = BACKWALL;
		mappingFromStringToInt["bottomwall"] = BOTTOMWALL;
		mappingFromStringToInt["ball"] = BALL;
		mappingFromStringToInt["player"] = PLAYER;
		mappingFromStringToInt["enemy"] = ENEMY;
		genVAO();
		camMode = HANGING;
		isInited = true;

		{		// ------------------------------------------------------------------
			float vertices[] = {
				10, 10, 0, // left  
				WORLD_COORD_MAP_XLEN-10, 10, 0, // right 
				10, WORLD_COORD_MAP_YLEN-10, 0, // top   
				10, WORLD_COORD_MAP_YLEN-10, 0, // top   
				WORLD_COORD_MAP_XLEN-10, WORLD_COORD_MAP_YLEN-10, 0, // right 
				WORLD_COORD_MAP_XLEN - 10, 10, 0 // top   
			};
			float vertices2[] = {
				-1000, 500, 10, // left  
				1000, 500, 10, // right 
				500, 500, 700// top   
			};

			unsigned int VBO;
			glGenVertexArrays(1, &myVAO);
			glGenBuffers(1, &VBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindVertexArray(myVAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
			// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
			glBindVertexArray(0);


			glGenVertexArrays(1, &myVAO2);
			glGenBuffers(1, &VBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindVertexArray(myVAO2);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
			// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
			glBindVertexArray(0);
		}

		SetModelAndViewMatrix(camMode, 0, 0);
	}

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

	/* ���� �� ���ּ���.
	if (GameManager::getInstance().WhoFinallyWin != 0) {

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);
		representResult();
		glLoadIdentity();
		glutSwapBuffers();
		return;
	}
	*/

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (camMode == CamMode::CHARACTER) {
		glm::vec3 camerapos = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z);
		glm::vec3 cameradirection = camerapos + glm::vec3(0, 0, -1);
		glm::vec3 up = glm::vec3(0, 1, 0);
		MyShader::setMat4("View", glm::lookAt(camerapos, cameradirection, up));
		MyShader::setMat4("Projection", glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, (float)0.1, (float)WORLD_COORD_MAP_ZLEN));

	}

	glm::mat4 trans;
	glm::vec3 unitpos;
	Transform* unit;
	string objPath;
	glm::vec3 rotationAxis;


	switch (renMode) {
	case NO_HIDDEN_LINE_REMOVAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (map<string, GLuint>::iterator iter = VAO_map.begin(); iter != VAO_map.end(); ++iter) {
			switch (mappingFromStringToInt[(*iter).first]) {
			case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				MyShader::setVec4("myColor", lineColor);               
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, WALL_INDICES_NUM, GL_UNSIGNED_INT, 0);
//				MyShader::setVec4("myColor", polygonInnerColor);
//				glBindVertexArray(myVAO2);
//				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);
				break;
			case BALL: //�̹� scale�� ������ ball�� ��ġ��ŭ �����̵��� ��Ŵ.
				
				trans = glm::identity<glm::mat4>();
				unitpos = glm::vec3(ball->GetCurrentPosition().x, ball->GetCurrentPosition().y, ball->GetCurrentPosition().z);
//				trans = glm::translate(trans, unitpos); // ����� ���� �³�?
				
				MyShader::setMat4("Model", glm::translate(trans, unitpos));
				MyShader::setVec4("myColor", lineColor);
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->triangleSize , GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY: // �� ���� roataion �����̵� �� ��
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::rotate(glm::translate(trans, unitpos), (float)unit->rotation, rotationAxis));
				cout << rotationAxis.x << " " << rotationAxis.y << " " << rotationAxis.z << " " << "angle"<< (float)unit->rotation << endl;
				glBindVertexArray((*iter).second);  
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->triangleSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		break;

	case HIDDEN_LINE_REMOVAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		MyShader::setVec4("myColor", lineColor);

		for (map<string, GLuint>::iterator iter = VAO_map.begin(); iter != VAO_map.end(); ++iter) {
			switch (mappingFromStringToInt[(*iter).first]) {
			case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
				MyShader::setMat4("Model", glm::identity<glm::mat4>());
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, WALL_INDICES_NUM, GL_UNSIGNED_INT, 0);
				//				MyShader::setVec4("myColor", polygonInnerColor);
				//				glBindVertexArray(myVAO2);
				//				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);
				break;
			case BALL: //�̹� scale�� ������ ball�� ��ġ��ŭ �����̵��� ��Ŵ.

				trans = glm::identity<glm::mat4>();
				unitpos = glm::vec3(ball->GetCurrentPosition().x, ball->GetCurrentPosition().y, ball->GetCurrentPosition().z);
				//				trans = glm::translate(trans, unitpos); // ����� ���� �³�?

				MyShader::setMat4("Model", glm::translate(trans, unitpos));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->triangleSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY: // �� ���� roataion �����̵� �� ��
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::rotate(glm::translate(trans, unitpos), (float)unit->rotation, rotationAxis));
				cout << rotationAxis.x << " " << rotationAxis.y << " " << rotationAxis.z << " " << "angle" << (float)unit->rotation << endl;
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->triangleSize, GL_UNSIGNED_INT, 0);
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
				glDrawElements(GL_TRIANGLES, WALL_INDICES_NUM, GL_UNSIGNED_INT, 0);
				//				MyShader::setVec4("myColor", polygonInnerColor);
				//				glBindVertexArray(myVAO2);
				//				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);
				break;
			case BALL: //�̹� scale�� ������ ball�� ��ġ��ŭ �����̵��� ��Ŵ.

				trans = glm::identity<glm::mat4>();
				unitpos = glm::vec3(ball->GetCurrentPosition().x, ball->GetCurrentPosition().y, ball->GetCurrentPosition().z);
				//				trans = glm::translate(trans, unitpos); // ����� ���� �³�?

				MyShader::setMat4("Model", glm::translate(trans, unitpos));
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(ballObjPath)->triangleSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case PLAYER: case ENEMY: // �� ���� roataion �����̵� �� ��
				if (mappingFromStringToInt[(*iter).first] == PLAYER) {
					unit = player;
					objPath = playerObjPath;
				}
				else {
					unit = enemy;
					objPath = enemyObjPath;
				}
				rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
				unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
				MyShader::setMat4("Model", glm::rotate(glm::translate(trans, unitpos), (float)unit->rotation, rotationAxis));
				cout << rotationAxis.x << " " << rotationAxis.y << " " << rotationAxis.z << " " << "angle" << (float)unit->rotation << endl;
				glBindVertexArray((*iter).second);
				glDrawElements(GL_TRIANGLES, GetObj(objPath)->triangleSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}		
		glDisable(GL_POLYGON_OFFSET_FILL);

		//�̰� �´� ���������� �� �𸣰ڽ��ϴ�.
	}
	/* ���ھ�� ���ּ���.
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
	//��ġ�̵�
	glTranslated(root->data->object->position.x, root->data->object->position.y, 0.0);

	//ȸ�������� �̵��� �� ȸ��, �ǵ�����
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


void representScore(int score, glm::vec2 pos)
{
	float segSize = 10;

	drawNumVAO(pos, score % 10);
	if (score / 10 > 0) {
		pos.x += segSize * 1.5;
		drawNumVAO(pos, score / 10);
	}
}

void genNumVAO() //2d VAO ����, ������ǥ: ���� �Ʒ�
{
	vector<float> vertices;
	vector<unsigned int> indices;
	float segSize = 10.0; // 7-seg ���� 1�� seg�� ����

						  // 7-seg�� �����ϴ� 6���� ������ ���� �Ʒ����� ������ ������ 6�� ��� vertices�� ����
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			vertices.push_back(i);
			vertices.push_back(j);
		}
	}


	for (int i = 0; i<10; i++)
	{
		//�� ���ں� indices �����Ͽ� �ֱ�
		switch (i)
		{
		case 0: indices.push_back(0); indices.push_back(1); //_
			indices.push_back(1); indices.push_back(5); // _l
			indices.push_back(5); indices.push_back(4); // ��
			indices.push_back(4); indices.push_back(0); break; // ��
			//case 1: ...
		default: break;
		}

		//buffer binding
		GLuint VBO, VAO, EBO; //each vertex buffer, vertex array, Elemental buffer

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//push VAO data to VAO_map
		VAO_map.insert(map<string, GLuint>::value_type(std::to_string(i), VAO));

	}


}

void drawNumVAO(glm::vec2 pos, int num) //2d�� ī�޶� ��ġ�� ���߾� �׸�
{
	int lineNum[10] = { 4, 1, 5, 4, 3, 5, 5, 3, 5, 5 };  //�� ���ڸ� �����ϴ� ���� ����


	if (num < 0 || num > 9) {
		cout << "drawNumVAO range error" << endl;
		return;
	}

	//�����̵� matrix ����
	glm::mat4 trans;
	glm::vec3 unitpos;
	Transform* unit;

	trans = glm::identity<glm::mat3>();
	unitpos = glm::vec3(pos.x, pos.y, 0);
	trans = glm::translate(trans, unitpos);

	//���� define�� 2d shader ����
	MyShader::setMat3("Model", glm::identity<glm::mat3>());
	glBindVertexArray(VAO_map.find(std::to_string(num))->second);
	glDrawElements(GL_LINES, 3 * lineNum[num], GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void genPolygonVAO(const Transform *object, string objPath) // obj ���� ��ο� �̸� ����ϴ� object�� �Է¹޾� VAO_map�� (objPath, objData)�� �������� ����
{
	ObjData* drawingObjData;
	
	//load image
	drawingObjData = GetObj(objPath);

	//shader drawing
	float* scaledVertexArray = new float[(int)(drawingObjData->vertexSize) * 3]; // ���� obj data�� load�� vertex ��ǥ�� object�� �Էµ� ũ��� scale

	for (int i = 0; i < drawingObjData->vertexSize * 3; i++) {
		switch (i % 3) {
		case 0: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().x; break;
		case 1: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().y; break;
		case 2: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().z;
		}
	}
	for (int i = 0; i < drawingObjData->triangleSize; i++) {
		drawingObjData->triangleArray[i] -= 1;
	}
	// starts to use

	// make buffers
	GLuint VBO, VAO, EBO; //each vertex buffer, vertex array, Elemental buffer

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* drawingObjData->vertexSize * 3, scaledVertexArray, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* drawingObjData->triangleSize, drawingObjData->triangleArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	VAO_map.insert(map<string, GLuint>::value_type(object->name, VAO));
	delete[] scaledVertexArray;
	cout << "GenPolygon Working" << endl;
}

void genVAO() { // bind .obj path for each object
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		switch (mappingFromStringToInt[objectsTreeVectorForDraw[i].name])
		{
			objectsTreeVectorForDraw[i].root->data;
		case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
			genWallVAO(objectsTreeVectorForDraw[i].root->data->object);
				break;
		case BALL: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, ballObjPath); break;
		case PLAYER: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, playerObjPath); break;
		case ENEMY: VAO_map.insert(map<string, GLuint>::value_type("enemy", VAO_map["player"])); break;
		default: break;
		}
	}
	cout << "GenVAO working" << endl;
}

void genWallVAO(const Transform* object) // 121 vertics, 200 triangles�� ���� 10*10 ���� ������ ��
{
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		vector<float> vertices;
		vector<unsigned int> indices;
		
		if (object->xlen == 0) // LEFT and RIGHT wall
		{
			for (int k = 0; k < 10; k++)
			{
				for (int j = 0; j < 10; j++)
				{
					vertices.push_back(object->GetCurrentPosition().x);
					vertices.push_back((object->GetCurrentPosition().y - object->ylen / 2) + object->ylen * j / 9.0);
					vertices.push_back((object->GetCurrentPosition().z - object->zlen / 2) + object->zlen * k / 9.0);
				}
			}
		}
		else if (object->zlen == 0) // FRONT and BACK wall
		{
			for (int k = 0; k < 10; k++)
			{
				for (int j = 0; j < 10; j++)
				{
					vertices.push_back((object->GetCurrentPosition().x - object->xlen / 2) + object->xlen * j / 9.0);
					vertices.push_back((object->GetCurrentPosition().y - object->ylen / 2) + object->ylen * k / 9.0);
					vertices.push_back(object->GetCurrentPosition().z);
				}
			}
		}
		else if (object->ylen == 0) // BOTTOM wall
		{
			for (int k = 0; k < 10; k++)
			{
				for (int j = 0; j < 10; j++)
				{
					vertices.push_back((object->GetCurrentPosition().x - object->xlen / 2) + object->xlen * k / 9.0);
					vertices.push_back(object->GetCurrentPosition().y);
					vertices.push_back((object->GetCurrentPosition().z - object->zlen / 2) + object->zlen * j / 9.0);
				}
			}
		}

		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++) 
			{
				indices.push_back(10 * j + k);
				indices.push_back(10 * j + k + 1);
				indices.push_back(10 * j + 10 + k + 1);

				indices.push_back(10 * j + 10 + k);
				indices.push_back(10 * j + 10 + k + 1);
				indices.push_back(10 * j + k);
			}
		}
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		string wallName = object->name;
		WALL_INDICES_NUM = indices.size();
		VAO_map.insert(map<string, GLuint>::value_type(wallName, VAO));
}


ObjData* GetObj(string path) { //path�� �޾� �ش� path�� �ִ� obj ������ �ε��� ObjData ����
	static map<string, ObjData*> objDataMap; // obj ���� ����� - load �� objData�� match �ϴ� ��
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
	glm::vec3 up;
	glm::mat4 view;
	glm::mat4 Projection;
	switch(camMode){
	case CHARACTER: 
		cameraPos = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z);
		cameraTarget = glm::vec3(player->GetCurrentPosition().x, player->GetCurrentPosition().y, player->GetCurrentPosition().z - 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, (float)0.1, (float)WORLD_COORD_MAP_ZLEN); // ���� ��ǥ�� ǥ�� ���� �ʿ�
		break;
	case BEHIND:
		cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_YLEN / 2, WORLD_COORD_MAP_ZLEN);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_YLEN / 2, WORLD_COORD_MAP_ZLEN / 2 -1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::ortho((float)-WORLD_COORD_MAP_XLEN, (float)WORLD_COORD_MAP_XLEN, (float)-WORLD_COORD_MAP_YLEN , (float)WORLD_COORD_MAP_YLEN , (float)0.1, (float)WORLD_COORD_MAP_ZLEN); // ���� ��ǥ�� ǥ�� ���� �ʿ�
		break;
	case HANGING:
		cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN/2 - a, 2*WORLD_COORD_MAP_YLEN, WORLD_COORD_MAP_ZLEN/2 - b);
		cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN/2 ,0, WORLD_COORD_MAP_ZLEN/2);
		glm::vec3 temp1 = glm::normalize(cameraTarget - cameraPos);
		
		if(temp1.x==temp1.z && temp1.x==0)
			up= glm::vec4(0, 0, 1, 1);
		else {
			glm::vec3 temp2 = glm::normalize(temp1 - glm::vec3(0, temp1.y, 0));
			glm::mat4 temp3 = glm::identity<glm::mat4>();
			temp3 = glm::rotate(temp3, glm::radians(90.0f), glm::cross(temp1, temp2));
			up = temp3 * glm::vec4(temp1.x, temp1.y, temp1.z, 1);
		}
		view = glm::lookAt(cameraPos, cameraTarget, up);
		Projection = glm::perspective(glm::radians(80.0f), (float)4./3, 0.1f, (float)10* WORLD_COORD_MAP_YLEN);
		break;
	}

	MyShader::setMat4("View", view);
	MyShader::setMat4("Projection", Projection);
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
	int lineNum = 100; // lineNum���� ���� �ٻ�

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
