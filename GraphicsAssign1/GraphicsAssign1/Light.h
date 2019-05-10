#pragma once
#include"myShader.h"

using namespace std;
using namespace glm;
class myLight {
public:
	myLight() : Ia(0.5), ka(0.5), Ip(2), kd(2), ks(1){

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