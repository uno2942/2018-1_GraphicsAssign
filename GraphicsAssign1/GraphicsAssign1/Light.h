#pragma once
#include"myShader.h"

using namespace std;
using namespace glm;
class myLight {
public:
	myLight() : Ia(0.25), ka(0.5), Ip(1), kd(1), ks(1), DIa(0.5), DIp(2){

	}
	void SetLightPosition(vec3 _lightpos) {
		lightPosition = _lightpos;
	}
	void DirectionalLightDirection(vec3 _direction) {
		DlightDirection = _direction;
	}
	void SetDirectionalLightI(float _DIa, float _DIp) {
		DIa = _DIa;
		DIp = _DIp;
	}
	void SetLightToShader() {
		MyShader::setVec3("LightPosition", lightPosition);
		MyShader::setFloat("ambientIaka", Ia * ka);
		MyShader::setFloat("diffuseIpkd", Ip * kd);
		MyShader::setFloat("specularIpks", Ip * ks);

		MyShader::setVec3("LightDirection", DlightDirection);
		MyShader::setFloat("DambientIaka", DIa * ka);
		MyShader::setFloat("DdiffuseIpkd", DIp * kd);
		MyShader::setFloat("DspecularIpks", DIp * ks);
	}
private:
	float Ia;
	float ka;
	float Ip;
	float kd;
	float ks;
	float DIa;
	float DIp;
	vec3 lightPosition;
	vec3 DlightDirection;
};