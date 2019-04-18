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
	GLdouble left;
	GLdouble right;
	GLdouble bottom;
	GLdouble top;
	GLdouble mynear;
	GLdouble myfar;
	
	static void SetCamera(Vector3 _position, Vector3 _viewingVector, GLdouble _left, GLdouble _right, GLdouble _bottom
		, GLdouble _top, GLdouble _mynear, GLdouble _myfar) {
		if (mCamera != NULL)
		{
			delete mCamera;
			myCamera::mCamera = new myCamera(_position, _viewingVector, _left, _right, _bottom, _top, _mynear, _myfar);
		}
	}

	static myCamera* GetCamera() {
		return myCamera::mCamera;
	}
private:
	myCamera(Vector3 _position, Vector3 _viewingVector, GLdouble _left, GLdouble _right, GLdouble _bottom
		, GLdouble _top, GLdouble _mynear, GLdouble _myfar) {
		position = _position;
		viewingVector = _viewingVector;
		left = _left;
		right = _right;
		bottom = _bottom;
		top = _top;
		mynear = _mynear;
		myfar = _myfar;
		isCameraGen = true;
	}
};