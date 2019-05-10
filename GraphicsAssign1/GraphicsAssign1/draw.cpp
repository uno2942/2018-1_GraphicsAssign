#include"Draw.h"


using namespace std;
using namespace glm;
static map< string, int > mappingFromStringToInt;
static map< string, Object* > mappingFromStringToUnit;
static map<string, MyObjData*> ObjData_map;

const vec4 backgroundColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);
const vec4 ballColor = vec4(1, 0, 0, 1);
const vec4 lineColor = vec4(1, 1, 1, 1);
const vec4 zeroColor = vec4(0, 0, 0, 0);
void drawResult();
void drawObject(Object* unit, MyObjData* myObjData);
void drawScore(int playerScore, int enemyScore);
void myReshape(int width, int height)
{	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	ReshapeFlag = !ReshapeFlag;
}

void PrepareDrawingAtFirstTime() {
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

	mappingFromStringToUnit["leftwall"] = GameManager::getInstance().leftWall;
	mappingFromStringToUnit["rightwall"] = GameManager::getInstance().rightWall;
	mappingFromStringToUnit["frontwall"] = GameManager::getInstance().frontWall;
	mappingFromStringToUnit["backwall"] = GameManager::getInstance().backWall;
	mappingFromStringToUnit["bottomwall"] = GameManager::getInstance().bottomWall;
	mappingFromStringToUnit["ball"] = GameManager::getInstance().ball;
	mappingFromStringToUnit["player"] = GameManager::getInstance().player;
	mappingFromStringToUnit["enemy"] = GameManager::getInstance().enemy;

	string ballObjPath = "sphere.obj";
	string playerObjPath = "12221_Cat_v1_l3.obj";
	string enemyObjPath = "12221_Cat_v1_l3.obj";
	vector<string> WallTexturePath;
	vector<string> PlayerTexturePath;

	WallTexturePath.push_back("normal.bmp");
	WallTexturePath.push_back("diffuse.DDS");
	WallTexturePath.push_back("specular.DDS");

	PlayerTexturePath.push_back("Cat_diffuse.bmp");

	genVAO(mappingFromStringToInt, &ObjData_map, ballObjPath, playerObjPath, enemyObjPath);

	addTexture(mappingFromStringToInt, &ObjData_map, WallTexturePath, PlayerTexturePath);
	MyShader::setInt("diffuseTexture", 0);
	MyShader::setInt("specularTexture", 1);
	MyShader::setInt("normalTexture", 2);
	myCamera::InitiateCamera(GameManager::getInstance().player);
}

void PrepareDrawing() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

}
void display()
{
	static bool isInited = false;

	if (!isInited) {
		PrepareDrawingAtFirstTime();
		isInited = true;
	}

	if (ReshapeFlag) {
		GameManager::getInstance().FreshTime();
		ReshapeFlag = !ReshapeFlag;
	}

	PrepareDrawing();	
	//=============================== Draw Start
	/*
	if (GameManager::getInstance().WhoFinallyWin != 0)
	{
		drawResult();
		return;
	}
	drawScore(GameManager::getInstance().myScore, GameManager::getInstance().enemyScore);
	*/

	myCamera::SetModelAndViewMatrix(camMode);
	GameManager::getInstance().mLight->SetLightToShader();
	MyShader::setMat4("Model", glm::identity<glm::mat4>());

	
	switch (renMode) {
	case NO_HIDDEN_LINE_REMOVAL:
	{glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (map<string, MyObjData*>::iterator iter = ObjData_map.begin(); iter != ObjData_map.end(); ++iter) {
		drawObject(mappingFromStringToUnit[(*iter).first], ObjData_map[(*iter).first]);
	}
	break;
	}
	case HIDDEN_LINE_REMOVAL:
	{
		for (map<string, MyObjData*>::iterator iter = ObjData_map.begin(); iter != ObjData_map.end(); ++iter) {
			drawObject(mappingFromStringToUnit[(*iter).first], ObjData_map[(*iter).first]);
		}
		/*
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0, 1.0);

		MyShader::setVec4("myColor", backgroundColor);

		for (map<string, MyObjData*>::iterator iter = ObjData_map.begin(); iter != ObjData_map.end(); ++iter) {
			drawObject(mappingFromStringToUnit[(*iter).first], ObjData_map[(*iter).first]);
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
		*/
		break;
	}
	}
	glutSwapBuffers();
}

void drawResult() {/*
	MyShader::setMat4("Model", glm::scale(glm::mat4(1.0f), glm::vec3(5, 5, 5)));
	glm::vec3 cameraPos = glm::vec3(0, 0, 1);
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
	glm::mat4 Projection = glm::ortho((float)-0.1, (float)0.1, (float)-0.1, (float)0.1, (float)0.1, (float)10); // 월드 좌표로 표현 수정 필
	MyShader::setMat4("View", view);
	MyShader::setVec4("myColor", glm::vec4(0, 0, 0, 1));
	MyShader::setMat4("Projection", Projection);

	string str;
	if (GameManager::getInstance().WhoFinallyWin == 1)
		str = "PLAYER WIN";
	else
		str = "ENEMY WIN";

	MyShader::setInt("numOfTexture", 0);
	glRasterPos2d(0, 0);
	for (int n = 0; n < str.size(); ++n) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[n]);
	}
	glutSwapBuffers();*/
	return;
}

void drawScore(int playerScore, int enemyScore) {
	/*
	MyShader::setVec4("myColor", glm::vec4(0, 0, 0, 1));
	MyShader::setMat4("Model", glm::mat4(1.0f));
	glm::vec3 cameraPos = glm::vec3(0, 0, 1);
	glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);

	MyShader::setMat4("View", view);
	glm::mat4 Projection = glm::ortho((float)-0.1, (float)0.1, (float)-0.1, (float)0.1, (float)0.1, (float)1); // 월드 좌표로 표현 수정 필요

	MyShader::setMat4("Projection", Projection);
	string str = to_string(playerScore);

	MyShader::setInt("numOfTexture", 0);
	glRasterPos3d(-0.09, 0.075, 0.9);
	for (int n = 0; n < str.size(); ++n) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[n]);
	}
	string str2 = to_string(enemyScore);;
	glRasterPos3d(0.09, 0.075, 0.9);
	for (int n = 0; n < str2.size(); ++n) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[n]);
	}*/
}

void drawObject(Object* unit, MyObjData* myObjData) {
	mat4 trans = glm::identity<glm::mat4>();
	vec3 unitpos;
	vec3 rotationAxis = glm::vec3(unit->rotationAxis.x, unit->rotationAxis.y, unit->rotationAxis.z);
	unitpos = glm::vec3(unit->GetCurrentPosition().x, unit->GetCurrentPosition().y, unit->GetCurrentPosition().z);
	mat4 model = rotate(glm::translate(trans, unitpos), (float)unit->rotation, rotationAxis);
	float yratio = 1;
	if (unit->GetSize().y == 0 && myObjData->width3D.y == 0)
		yratio = 1;
	else
		yratio = unit->GetSize().y / myObjData->width3D.y;
	if (mappingFromStringToInt[unit->name] == PLAYER || mappingFromStringToInt[unit->name] == ENEMY) {
		model = rotate(model, -3.141592f / 2, vec3(1, 0, 0));
	}
	model = scale(model, vec3(unit->GetSize().x / myObjData->width3D.x, yratio,
		unit->GetSize().z / myObjData->width3D.z));
	MyShader::setMat4("Model", model);

	MyShader::setInt("numOfTexture", (*myObjData).tex.size());

	switch (mappingFromStringToInt[unit->name])
	{
	case PLAYER: case ENEMY: case BACKWALL: case FRONTWALL: case LEFTWALL: case RIGHTWALL: case BOTTOMWALL:
		MyShader::setVec4("myColor", zeroColor);
		break;
	case BALL:
		MyShader::setVec4("myColor", ballColor);
	}
	for (int i = 0; i < (*myObjData).tex.size(); i++)
	{
		if (i == 0)
			glActiveTexture(GL_TEXTURE0);
		else if (i == 1)
			glActiveTexture(GL_TEXTURE1);
		else
			glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, (*myObjData).tex[i]);
	}

	glBindVertexArray(myObjData->VAO);
	glDrawArrays(GL_TRIANGLES, 0, myObjData->Size);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
/*
void representScore(int score, glm::vec2 pos)
{
	float segSize = 10;

	drawNumVAO(pos, score % 10);
	if (score / 10 > 0) {
		pos.x += segSize * 1.5;
		drawNumVAO(pos, score / 10);
	}
}

void genNumVAO() //2d VAO 저장, 기준좌표: 왼쪽 아래
{
	vector<float> vertices;
	vector<unsigned int> indices;
	float segSize = 10.0; // 7-seg 에서 1개 seg의 길이

						  // 7-seg를 구성하는 6개의 점들을 왼쪽 아래부터 오른쪽 위까지 6개 찍어 vertices에 저장
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			vertices.push_back(i);
			vertices.push_back(j);
		}
	}


	for (int i = 0; i<10; i++)
	{
		//각 숫자별 indices 생성하여 넣기
		switch (i)
		{
		case 0: indices.push_back(0); indices.push_back(1); //_
			indices.push_back(1); indices.push_back(5); // _l
			indices.push_back(5); indices.push_back(4); // ㅕ
			indices.push_back(4); indices.push_back(0); break; // ㅁ
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

void drawNumVAO(glm::vec2 pos, int num) //2d로 카메라 위치에 맞추어 그림
{
	int lineNum[10] = { 4, 1, 5, 4, 3, 5, 5, 3, 5, 5 };  //각 숫자를 구성하는 선의 개수


	if (num < 0 || num > 9) {
		cout << "drawNumVAO range error" << endl;
		return;
	}

	//평행이동 matrix 생성
	glm::mat4 trans;
	glm::vec3 unitpos;
	Transform* unit;

	trans = glm::identity<glm::mat3>();
	unitpos = glm::vec3(pos.x, pos.y, 0);
	trans = glm::translate(trans, unitpos);

	//새로 define할 2d shader 만듬
	MyShader::setMat3("Model", glm::identity<glm::mat3>());
	glBindVertexArray(VAO_map.find(std::to_string(num))->second);
	glDrawElements(GL_LINES, 3 * lineNum[num], GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
*/