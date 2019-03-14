#pragma once
#include<glew.h>
#include<freeglut.h>
#include<iostream>
using namespace std;

struct Vector2 {
public:
	GLdouble x;
	GLdouble y;
	Vector2(double _x, double _y) {
		x = _x;
		y = _y;
	}

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2 operator+(const Vector2& rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	Vector2& operator+=(const Vector2& rhs) {
		(*this).x += rhs.x;
		(*this).y += rhs.y;
		return *this;
	}
	friend ostream& operator<<(ostream& os, const Vector2& dt){
		os << '('<<dt.x<<',' << dt.y<<')';
		return os;
	}
};

class Object {
public:
	enum Shape { BOX, CIRCLE };
	Shape shape;
	Object() {

	}
	Object(int _width, int _height) : width(_width), height(_height){

	}
	const Vector2& GetCurrentPosition() const;
	const Vector2& SetPosition(Vector2 position);
	const Vector2& SetPosition(GLdouble x, GLdouble y);
	const Vector2& GetCurrentVelocity() const ;
	const Vector2& SetVelocity(Vector2 velocity);
	const Vector2& SetVelocity(GLdouble x, GLdouble y);
	const Vector2 GetSize() const;

	int width;
	int height;
	string name;
	Vector2 position;
	Vector2 velocity;
};

class Circle : public Object {
public: 
	Circle() {
		shape = CIRCLE;
		name = "circle";
	}
	Circle(int _width, int _height) : Object(_width, _height) {
		shape = BOX;
		name = "circle";
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