#pragma once
#include"myShader.h"

using namespace std;
using namespace glm;
class myLight {
public:
	myLight() : Ia(0), ka(0), Ip(1), kd(0), ks(10), lightPosition(vec3(500, 300, 500)) {

	}
	void SetLightPosition(vec3 _lightpos) {
		lightPosition = _lightpos;
	}
	void SetLightToShader() {
		MyShader::setVec3("LightPosition", lightPosition);
		MyShader::setFloat("ambientIaka", Ia * ka);
		MyShader::setFloat("diffuseIpkd", Ip * kd);
		MyShader::setFloat("specularIpks", Ip * ks);
	}
private:
	float Ia;
	float ka;
	float Ip;
	float kd;
	float ks;
	vec3 lightPosition;
};