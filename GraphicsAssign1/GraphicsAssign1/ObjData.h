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
	float* vertexArray;
	unsigned int* triangleArray;
	glm::vec3 width3D;

	unsigned int vertexSize;
	unsigned int triangleSize;
};