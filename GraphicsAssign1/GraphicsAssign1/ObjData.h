#pragma once

#include <vector>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>
using namespace std;
class ObjData{
public:
	vector<vector<glm::vec3>> vertices;
	vector<vector<glm::vec2>> uvs;
	vector<vector<glm::vec3>> normals;
	vector<vector<glm::vec3>> triangles;
	glm::vec3 width3D; // obj 에 저장된 vertex를 어떠한 변환도 하지 않고 그냥 그릴 때 x, y, z방향으로의 크기
	vector<unsigned int> Size; //obj의 triangle 수
	vector<string> mtlName;
};

class MyObjData : public ObjData {
public:
	std::vector<GLuint> VAO;
	std::vector<GLuint> tex;
};