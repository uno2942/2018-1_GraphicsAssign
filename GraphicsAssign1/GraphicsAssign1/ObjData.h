#pragma once

#include <vector>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>

class ObjData{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs; 
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> triangles;
	glm::vec3 width3D; // obj �� ����� vertex�� ��� ��ȯ�� ���� �ʰ� �׳� �׸� �� x, y, z���������� ũ��
	unsigned int Size; //obj�� triangle ��
};

class MyObjData : public ObjData {
public:
	GLuint VAO;
	GLuint tex;
};