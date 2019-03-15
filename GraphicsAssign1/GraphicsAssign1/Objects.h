#pragma once
#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<cmath>
using namespace std;

struct Vector2 {
public:
	GLdouble x;
	GLdouble y;
	Vector2(GLdouble _x, GLdouble _y) {
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
	Vector2 operator-(const Vector2& rhs) {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	Vector2& operator+=(const Vector2& rhs) {
		(*this).x += rhs.x;
		(*this).y += rhs.y;
		return *this;
	}

	Vector2 operator*(const GLdouble d) {
		return Vector2((*this).x * d, (*this).y * d);
	}
	static GLdouble abs(const Vector2& a);
	
	static Vector2 normalize(const Vector2& a) {
		GLdouble veclen = sqrt(a.x*a.x + a.y*a.y);
		return Vector2(a.x / veclen, a.y / veclen);
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