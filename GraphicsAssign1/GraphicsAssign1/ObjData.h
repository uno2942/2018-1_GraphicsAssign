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
	glm::vec3 width3D; // obj �� ����� vertex�� ��� ��ȯ�� ���� �ʰ� �׳� �׸� �� x, y, z���������� ũ��
	vector<unsigned int> Size; //obj�� triangle ��
	vector<string> mtlName;
};

class MyObjData : public ObjData {
public:
	std::vector<GLuint> VAO;
	std::vector<GLuint> tex;
};