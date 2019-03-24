#pragma once

#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<cmath>

#include "MyDataType.h"
using namespace std;

class Object {
public:
	enum Shape { OVAL, BOX, TRIANGLE};
	Shape shape;
	Object() {

	}
	Object(int _width, int _height, GLdouble _rotation) : width(_width), height(_height), rotation(_rotation) {

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

class Oval : public Object {
public: 
	Oval() {
		shape = OVAL;
	}
	Oval(string name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation) {
		shape = OVAL;
		this->name = name;
	}
};

class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
	Box(string name, int _width, int _height, GLdouble _rotation = 0): Object(_width, _height, _rotation) {
		shape = BOX;
		this->name = name;
	}
};

class Triangle : public Object{
public:
	Triangle() {
		shape = TRIANGLE;
	}
	Triangle(string _name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation) {
		shape = TRIANGLE;
		name = _name;
	}
};