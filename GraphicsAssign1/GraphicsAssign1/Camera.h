#pragma once
#include"myShader.h"
#include"Objects.h"
#include"Units.h"
using namespace std;
using namespace glm;
enum CamMode { CHARACTER, BEHIND, HANGING };
extern CamMode camMode;

class myCamera
{
public:
	static bool isCameraGen;
	static Transform* player;
	static GLfloat a, b;
	static vec3 nowcameraPos;
	static vec3 nowcameraTarget;
	static vec3 nowup;
	static void InitiateCamera(Transform* _player) {
		if (isCameraGen ==false)
		{
			player = _player;
			isCameraGen = true;
		}
	}
	static void SetModelAndViewMatrix(CamMode camMode) { //reference: https://heinleinsgame.tistory.com/12
		if (!isCameraGen)
		{
			cout << "Camera not instantiated";
			return;
		}
		vec3 rotationAxis = vec3(0, 0, 0);
		vec3 unitpos = vec3(0, 0, 0);
		vec3 cameraPos = vec3(0, 0, 0);
		vec3 cameraTarget = vec3(0, 0, 0);
		vec3 up = vec3(0, 0, 0);
		mat4 view;
		mat4 Projection;
		switch (camMode) {
		case CHARACTER:
			unitpos = glm::vec3(player->GetCurrentPosition().x - ((player->zlen + 20) /2) * sin(player->rotation + 3.141592), player->GetCurrentPosition().y * 1.5, player->GetCurrentPosition().z - ((player->zlen + 20) / 2) * cos(player->rotation + 3.141592));
			rotationAxis = glm::vec3(player->rotationAxis.x, player->rotationAxis.y, player->rotationAxis.z);

			glm::mat4 rotatemat = glm::rotate(glm::mat4(1.0f), (float)player->rotation + 3.141592f, rotationAxis);

			cameraPos = glm::vec3(unitpos);
			cameraTarget = glm::vec3(glm::vec4(unitpos, 1) + rotatemat * glm::vec4(0, 0, -1, 1));
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			view = glm::lookAt(cameraPos, cameraTarget, up);
			Projection = glm::perspective(glm::radians(80.0f), (float)4. / 3, 0.1f, (float)10 * WORLD_COORD_MAP_YLEN);
			break;
		case BEHIND:
			cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_YLEN / 2, WORLD_COORD_MAP_ZLEN - 2);
			cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN / 2, WORLD_COORD_MAP_YLEN / 2, WORLD_COORD_MAP_ZLEN / 2 - 3);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			view = glm::lookAt(cameraPos, cameraTarget, up);
			Projection = glm::perspective(glm::radians(80.0f), (float)4. / 3, 0.1f, (float)10 * WORLD_COORD_MAP_YLEN);
			break;
		case HANGING:
			cameraPos = glm::vec3(WORLD_COORD_MAP_XLEN / 2 - a, 2 * WORLD_COORD_MAP_YLEN, WORLD_COORD_MAP_ZLEN / 2 - b);
			cameraTarget = glm::vec3(WORLD_COORD_MAP_XLEN / 2, 0, WORLD_COORD_MAP_ZLEN / 2);
			glm::vec3 temp1 = glm::normalize(cameraTarget - cameraPos);

			if (temp1.x == temp1.z && temp1.x == 0)
				up = glm::vec4(0, 0, -1, 1);
			else {
				glm::vec3 temp2 = glm::normalize(temp1 - glm::vec3(0, temp1.y, 0));
				glm::mat4 temp3 = glm::identity<glm::mat4>();
				temp3 = glm::rotate(temp3, glm::radians(90.0f), glm::cross(temp1, temp2));
				up = temp3 * glm::vec4(temp1.x, temp1.y, temp1.z, 1);
			}
			view = glm::lookAt(cameraPos, cameraTarget, up);
			Projection = glm::perspective(glm::radians(80.0f), (float)4. / 3, 0.1f, (float)10 * WORLD_COORD_MAP_YLEN);
			break;
		}
		nowcameraPos = cameraPos;
		nowcameraTarget = cameraTarget;
		nowup = up;
		MyShader::setVec3("CameraPos", nowcameraPos);
		MyShader::setMat4("View", view);
		MyShader::setMat4("Projection", Projection);
	}


	static void SetHangingxy(GLfloat _x, GLfloat _y) {
		a = _x;
		b = _y;
	}
};