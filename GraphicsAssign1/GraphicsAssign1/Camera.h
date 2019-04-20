#pragma once
#include"Objects.h"
#include"MyDataType.h"

class myCamera
{
public:
	static myCamera* mCamera;
	static bool isCameraGen;
	Vector3 position;
	Vector3 viewingVector;
	Vector3 upVector;
	GLdouble fovy;
	GLdouble aspect;
	GLdouble mynear;
	GLdouble myfar;
	
	static void SetCamera(Vector3 _position, Vector3 _viewingVector, Vector3 _upVector, GLdouble _fovy, GLdouble _aspect, GLdouble _mynear, GLdouble _myfar) {
		if (mCamera != NULL)
		{
			delete mCamera;
			myCamera::mCamera = new myCamera(_position, _viewingVector, _upVector, _fovy, _aspect, _mynear, _myfar);
		}
	}

	static myCamera* GetCamera() {
		return myCamera::mCamera;
	}
private:
	myCamera(Vector3 _position, Vector3 _viewingVector, Vector3 _upVector, GLdouble _fovy, GLdouble _aspect, GLdouble _mynear, GLdouble _myfar) {
		position = _position;
		viewingVector = _viewingVector;
		upVector = _upVector;
		fovy = _fovy;
		aspect = _aspect;
		mynear = _mynear;
		myfar = _myfar;
		isCameraGen = true;
	}
};