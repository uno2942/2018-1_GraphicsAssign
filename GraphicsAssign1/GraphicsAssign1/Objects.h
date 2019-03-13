#pragma once
#include<glut.h>

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
};

class Object {
public:
	enum Shape { BOX, CIRCLE };
	Shape shape;
	Object() {

	}
	Vector2& CurrentPosition();
	Vector2 SetPosition(Vector2 position);
	Vector2 SetPosition(GLdouble x, GLdouble y);
	Vector2 CurrentVelocity();
	Vector2 SetVelocity(Vector2 velocity);
	Vector2 SetVelocity(GLdouble x, GLdouble y);
protected:
	int width;
	int height;
	Vector2 position;
	Vector2 velocity;
};

class Circle : public Object {
public: 
	Circle() {
		shape = CIRCLE;
	}
};

class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
};