#pragma once

#include <vector>
#include<glew.h>
#include<freeglut.h>
#include <glm/glm.hpp>


class ObjData{
public:
	std::vector<glm::vec3> vertices; //3���� vertex���� vector
	std::vector<glm::vec2> uvs; //�� ������� ����
	std::vector<glm::vec3> normals; //������ noraml ���� vector
	std::vector<glm::vec3> triangles; // �� ����� �����ϴ� point���� ��� vector
	float* vertexArray; // 3*���� �� vertices vector�� 1���� �迭�� ��ģ ��
	unsigned int* triangleArray; // triganles�� 1���� �迭�� ��ģ��
	glm::vec3 width3D; // obj �� ����� vertex�� ��� ��ȯ�� ���� �ʰ� �׳� �׸� �� x, y, z���������� ũ��

	unsigned int vertexSize; // obj�� vertex ��
	unsigned int triangleSize; //obj�� triangle ��
};