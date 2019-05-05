#pragma once
#include"Draw.h"
#include<soil.h>

void genWallVAO(const Transform* object, map<string, MyObjData*>* ObjData_map);
MyObjData* GetObj(string path);
void genPlayerVAO(const Transform* object, map<string, MyObjData*>* ObjData_map, string objPath);

void genVAO(map< string, int > mappingFromStringToInt, map<string, MyObjData*>* ObjData_map, string ballObjPath, string playerObjPath, string enemyObjPath) { // bind .obj path for each object
	for (int i = 0; i < objectsTreeVectorForDraw.size(); i++) {
		switch (mappingFromStringToInt[objectsTreeVectorForDraw[i].name])
		{
			objectsTreeVectorForDraw[i].root->data;
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

	if (object->xlen == 0) // LEFT and RIGHT wall
	{
		for (int k = 0; k < 10; k++)
		{
			for (int j = 0; j < 10; j++)
			{
				vertices.push_back(0);
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
				vertices.push_back(0);
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
				vertices.push_back(0);
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
	MyObjData* drawingObjData = new MyObjData();
	drawingObjData->numberOfComponent = 1;
	drawingObjData->VAO.push_back(VAO);
	drawingObjData->triangleSizes.push_back(indices.size());
	ObjData_map->insert(map<string, MyObjData*>::value_type(wallName, drawingObjData));
}

void genPlayerVAO(const Transform* object, map<string, MyObjData*>* ObjData_map, string objPath) // obj 파일 경로와 이를 사용하는 object를 입력받아 VAO_map에 (objPath, objData)의 형식으로 삽입
{
	MyObjData* drawingObjData;
	string imagepath[] = { "Image09C37780_09C26A60.png", "Image09C27700_09C272A0.png", "Image09C3F800_09C26E80.png",
	"Image09CDBA00_09CC3480.png", "Image09CD3980_09CC3060.png" };
	GLuint texture;

	drawingObjData = GetObj(objPath);
	for (int j = 0; j < drawingObjData->numberOfComponent; j++)
	{
		//shader drawing
		float* scaledVertexArray = new float[(int)(drawingObjData->vertexSize) * 3]; // 기존 obj data를 load한 vertex 좌표를 object에 입력된 크기로 scale

		for (int i = 0; i < drawingObjData->vertexSize * 3; i++) {
			switch (i % 3) {
			case 0: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().x; break;
			case 1: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().y; break;
			case 2: scaledVertexArray[i] = drawingObjData->vertexArray[i] / drawingObjData->width3D[i % 3] * object->GetSize().z;
			}
		}
		for (int i = 0; i < drawingObjData->triangleSizes[j]; i++) {
			drawingObjData->triangleArray[j][i] -= 1;
		}
		// starts to use

		// make buffers
		GLuint VBO, VAO, EBO; //each vertex buffer, vertex array, Elemental buffer

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * drawingObjData->vertexSize * 3, scaledVertexArray, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawingObjData->triangleSizes[j], drawingObjData->triangleArray[j], GL_STATIC_DRAW);

		texture = SOIL_load_OGL_texture
		(
			imagepath[j].c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
		//load image

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		drawingObjData->VAO.push_back(VAO);
		ObjData_map->insert(map<string, MyObjData*>::value_type(object->name, drawingObjData));
		delete[] scaledVertexArray;
		cout << "GenPolygon Working" << endl;
	}

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