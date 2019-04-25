#pragma once

#include <vector>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>


class ObjData{
public:
	std::vector<glm::vec3> vertices; //3차원 vertex들의 vector
	std::vector<glm::vec2> uvs; //모름 사용하지 않음
	std::vector<glm::vec3> normals; //점들의 noraml 들의 vector
	std::vector<glm::vec3> triangles; // 각 면들을 구성하는 point들을 담는 vector
	float* vertexArray; // 3*개수 의 vertices vector를 1차원 배열로 펼친 것
	unsigned int* triangleArray; // triganles을 1차원 배열로 펼친것
	glm::vec3 width3D; // obj 에 저장된 vertex를 어떠한 변환도 하지 않고 그냥 그릴 때 x, y, z방향으로의 크기

	unsigned int vertexSize; // obj의 vertex 수
	unsigned int triangleSize; //obj의 triangle 수
};