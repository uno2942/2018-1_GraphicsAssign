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
	vector<GLuint> images;
	float* vertexArray;
	vector<unsigned int*> triangleArray;
	glm::vec3 width3D;

	unsigned int vertexSize;
	vector<unsigned int> triangleSizes;
	unsigned int numberOfComponent;

	std::vector<std::string> name;
};

class MyObjData : public ObjData {
public:
	vector<GLuint> VAO;
};