#pragma once

#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<cmath>

#include "MyDataType.h"
using namespace std;

class Object {
public:
	enum Shape { BOX, CIRCLE };
	Shape shape;
	Object() {

	}
	Object(int _width, int _height) : width(_width), height(_height){

	}

	int width;
	int height;

	string name;

	Vector2 position;
	Vector2 velocity;
	GLdouble rotation;

	inline const Vector2& GetCurrentPosition() const { return position; }
	inline const GLdouble& GetCurrentRotation() const { return rotation; }
	inline const Vector2& GetCurrentVelocity() const {
		return velocity;
	}
	inline const Vector2& SetPosition(Vector2 _position) {
		position = _position;
		return position;
	}
	inline const Vector2& SetPosition(GLdouble x, GLdouble y) {
		position.x = x;
		position.y = y;
		return position;
	}
	inline const GLdouble& SetRotation(GLdouble _rotation) {
		rotation = _rotation;
		return rotation;
	}
	inline const Vector2& SetVelocity(Vector2 _velocity) {
		velocity = _velocity;
		return velocity;
	}
	inline const Vector2& SetVelocity(GLdouble x, GLdouble y) {
		velocity.x = x;
		velocity.y = y;
		return velocity;
	}
	inline const Vector2 GetSize() const {
		return Vector2(width, height);
	}
};

class Circle : public Object {
public: 
	Circle() {
		shape = CIRCLE;
	}
	Circle(int _width, int _height, string name) : Object(_width, _height) {
		shape = CIRCLE;
		this->name = name;
	}
};

class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
	Box(int _width, int _height, string name): Object(_width, _height) {
		shape = BOX;
		this->name = name;
	}
};