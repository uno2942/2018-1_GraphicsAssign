#include"Draw.h"

void genWallVAO(const Transform* object, map<string, MyObjData*>* ObjData_map);
MyObjData* GetObj(string path);
void genPolygonVAO(const Transform* object, map<string, MyObjData*>* ObjData_map, string objPath);

void genVAO(map< string, int > mappingFromStringToInt, map<string, MyObjData*>* ObjData_map, string ballObjPath, string playerObjPath, string enemyObjPath) { // bind .obj path for each object
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		switch (mappingFromStringToInt[objectsTreeVectorForDraw[i].name])
		{
		case LEFTWALL: case RIGHTWALL: case FRONTWALL: case BACKWALL: case BOTTOMWALL:
			genWallVAO(objectsTreeVectorForDraw[i].root->data->object, ObjData_map);
			break;
		case BALL: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, ObjData_map, ballObjPath); break;
		case PLAYER: genPolygonVAO(objectsTreeVectorForDraw[i].root->data->object, ObjData_map, playerObjPath); break;
		case ENEMY: ObjData_map->insert(map<string, MyObjData*>::value_type("enemy", (*ObjData_map)["player"])); break;
		default: break;
		}
	}
	cout << "GenVAO working" << endl;
}

MyObjData* GetObj(string path) { //path를 받아 해당 path에 있는 obj 파일을 로드한 ObjData만 리턴 나중에 VAO를 넣어줌
	static map<string, MyObjData*> objDataMap; // obj 파일 상대경로 - load 한 objData를 match 하는 맵
	if (objDataMap.find(path) == objDataMap.end())
	{
		MyObjData* objData = new MyObjData();
		loadOBJ(path.c_str(), objData);
		objDataMap.insert(pair<string, MyObjData*>(path, objData));
	}
	return objDataMap.find(path)->second;
}

void genWallVAO(const Transform* object, map<string, MyObjData*>* ObjData_map) {

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	vector<float> vertices;
	vector<unsigned int> indices;
	vector<float> realdata;
	if (object->name == "leftwall") // LEFT wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back(0);//vertices
				vertices.push_back((- object->ylen / 2) + object->ylen * j / 9.0);
				vertices.push_back((- object->zlen / 2) + object->zlen * k / 9.0);
				vertices.push_back(1);//normals
				vertices.push_back(0);
				vertices.push_back(0);
				vertices.push_back(k / 9.);//uvs
				vertices.push_back(- j / 9.);
			}
		}
	}
	else if (object->name == "rightwall") //RIGHT wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back(0);//vertices
				vertices.push_back((- object->ylen / 2) + object->ylen * j / 9.0);
				vertices.push_back((- object->zlen / 2) + object->zlen * k / 9.0);
				vertices.push_back(-1);//normals
				vertices.push_back(0);
				vertices.push_back(0);
				vertices.push_back(k / 9.);//uvs
				vertices.push_back(- j / 9.);
			}
		}
	}
	else if (object->name == "frontwall") // FRONT wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back((- object->xlen / 2) + object->xlen * j / 9.0);
				vertices.push_back((- object->ylen / 2) + object->ylen * k / 9.0);
				vertices.push_back(0);
				vertices.push_back(0);//normals
				vertices.push_back(0);
				vertices.push_back(1);
				vertices.push_back(k / 9.);//uvs
				vertices.push_back(- j / 9.);
			}
		}
	}
	else if (object->name == "backwall") // BACK wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back((- object->xlen / 2) + object->xlen * j / 9.0);
				vertices.push_back((- object->ylen / 2) + object->ylen * k / 9.0);
				vertices.push_back(0);
				vertices.push_back(0);//normals
				vertices.push_back(0);
				vertices.push_back(-1);
				vertices.push_back(k / 9.);//uvs
				vertices.push_back(- j / 9.);
			}
		}
	}
	else if (object->name == "bottomwall") // BOTTOM wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back((- object->xlen / 2) + object->xlen * k / 9.0);
				vertices.push_back(0);
				vertices.push_back((- object->zlen / 2) + object->zlen * j / 9.0);
				vertices.push_back(0);//normals
				vertices.push_back(1);
				vertices.push_back(0);
				vertices.push_back(k / 9.);//uvs
				vertices.push_back(j / 9.);
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

	for (int i = 0; i < indices.size(); i++) {
		realdata.push_back(vertices[8 * indices[i]]);
		realdata.push_back(vertices[8 * indices[i] + 1]);
		realdata.push_back(vertices[8 * indices[i] + 2]);
		realdata.push_back(vertices[8 * indices[i] + 3]);
		realdata.push_back(vertices[8 * indices[i] + 4]);
		realdata.push_back(vertices[8 * indices[i] + 5]);
		realdata.push_back(vertices[8 * indices[i] + 6]);
		realdata.push_back(vertices[8 * indices[i] + 7]);
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, realdata.size() * sizeof(float), &realdata[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//uvs
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	string wallName = object->name;
	MyObjData* drawingObjData = new MyObjData();
	drawingObjData->VAO = VAO;
	drawingObjData->Size = indices.size();
	ObjData_map->insert(map<string, MyObjData*>::value_type(wallName, drawingObjData));
}

void genPolygonVAO(const Transform * object, map<string, MyObjData*>* ObjData_map, string objPath) // obj 파일 경로와 이를 사용하는 object를 입력받아 VAO_map에 (objPath, objData)의 형식으로 삽입
{
	MyObjData* drawingObjData;

	//load image
	drawingObjData = GetObj(objPath);

	//shader drawing
	long long arraysize = drawingObjData->vertices.size() * (3 + 2 + 3);
	float* myarray = new float[arraysize];

	for (int i = 0; i < drawingObjData->vertices.size(); i++) {
		myarray[8 * i] = drawingObjData->vertices[i].x * 10; //* object->scaleFactor;
		myarray[8 * i + 1] = drawingObjData->vertices[i].y * 10;
		myarray[8 * i + 2] = drawingObjData->vertices[i].z * 10;
		myarray[8 * i + 3] = drawingObjData->normals[i].x;
		myarray[8 * i + 4] = drawingObjData->normals[i].y;
		myarray[8 * i + 5] = drawingObjData->normals[i].z;
		myarray[8 * i + 6] = drawingObjData->uvs[i].x;
		myarray[8 * i + 7] = drawingObjData->uvs[i].y;
	}
	// starts to use

	// make buffers
	GLuint VAO, VBO; //each vertex buffer, vertex array, Elemental buffer

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraysize, myarray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//uvs
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	drawingObjData->VAO = VAO;
	drawingObjData->Size = drawingObjData->vertices.size();
	ObjData_map->insert(map<string, MyObjData*>::value_type(object->name, drawingObjData));
	cout << "GenPolygon Working" << endl;
}



/*
void genWallVAO(const Transform* object) // 121 vertics, 200 triangles를 가진 10*10 격자 형태의 벽
{
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	vector<float> vertices;
	vector<unsigned int> indices;

	//generateVertices
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

	//generate Indices
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
	objDataMap.insert(map<string, GLuint>::value_type(wallName, VAO));
}
*/