#include"Camera.h"

CamMode camMode = HANGING;

bool myCamera::isCameraGen = false;
Transform* myCamera::player = NULL;
GLfloat myCamera::a = 0;
GLfloat myCamera::b = 0;
vec3 myCamera::nowcameraPos = vec3(0, 0, 0);
vec3 myCamera::nowcameraTarget = vec3(0, 0, 0);
vec3 myCamera::nowup = vec3(0, 0, 0);