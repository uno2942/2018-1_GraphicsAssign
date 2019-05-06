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
	glm::vec3 width3D; // obj 에 저장된 vertex를 어떠한 변환도 하지 않고 그냥 그릴 때 x, y, z방향으로의 크기
	unsigned int Size; //obj의 triangle 수
};

class MyObjData : public ObjData {
public:
	GLuint VAO;
	GLuint tex;
};