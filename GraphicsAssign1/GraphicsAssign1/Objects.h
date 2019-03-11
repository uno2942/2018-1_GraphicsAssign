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

	}
};

class Object {
public:
	enum Shape { BOX, CIRCLE };
	Shape shape;
	Object() {

	}
	Vector2 CurrentPosition();
	Vector2 SetPosition(Vector2 position);
	Vector2 SetPosition();
	Vector2 CurrentVelocity();
	Vector2 SetVelocity(Vector2 velocity);
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