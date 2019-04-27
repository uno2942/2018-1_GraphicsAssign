#pragma once

#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include<cmath>

#include "MyDataType.h"
using namespace std;
class Object;
class Oval;
class Box;
class Triangle;
class ObjectWithComponent;

class Object {
public:
	enum Shape { OVAL, BOX, TRIANGLE };
	Shape shape;
	Object();
	Object(GLdouble _xlen,  GLdouble _zlen, GLdouble _ylen, GLdouble _rotation, Shape _shape, int r = 0, int g = 0, int b = 0, 
		GLdouble a = 1, GLdouble x = 0, GLdouble z = 0, GLdouble y = 0, GLdouble vecx = 0, GLdouble vecz = 0, GLdouble vecy = 0) : xlen(_xlen), ylen(_ylen), zlen(_zlen), rotation(_rotation), shape(_shape), myColor(r, g, b, a) {
		position = Vector3(x, y, z);
		velocity = Vector3(vecx, vecz, vecy);

	}
	Object(GLdouble _xlen, GLdouble _zlen, GLdouble _ylen, GLdouble _rotation, Shape _shape, MyColor mColor, 
		GLdouble x = 0,  GLdouble z = 0, GLdouble y = 0, GLdouble vecx = 0, GLdouble vecz = 0, GLdouble vecy=0) : xlen(_xlen), ylen(_ylen), zlen(_zlen), rotation(_rotation), shape(_shape), myColor(mColor) {
		position = Vector3(x, z, y);
		velocity = Vector3(vecx, vecz, vecy);

	}
	GLdouble xlen;
	GLdouble zlen;
	GLdouble ylen;

	string name;

	Vector3 position;
	Vector3 velocity;
	Vector3 rotationAxis = Vector3(0, 0, 0);
	GLdouble rotation;
	MyColor myColor;
	bool isFixed = false;
	inline const Vector3& GetCurrentPosition() const { return position; }
	inline const GLdouble& GetCurrentRotation() const { return rotation; }
	inline const Vector3& GetCurrentVelocity() const {
		return velocity;
	}
	inline const Vector3& SetPosition(Vector3 _position) {
		position = _position;
		return position;
	}
	inline const Vector3& SetPosition(GLdouble x, GLdouble z, GLdouble y) {
		position.x = x;
		position.z = z;
		position.y = y;
		return position;
	}
	inline const GLdouble& SetRotation(GLdouble _rotation) {
		rotation = _rotation;
		return rotation;
	}
	inline const Vector3& SetVelocity(Vector3 _velocity) {
		velocity = _velocity;
		return velocity;
	}
	inline const Vector3& SetVelocity(GLdouble x, GLdouble z, GLdouble y) {
		velocity.x = x;
		velocity.z = z;
		velocity.y = y;
		return velocity;
	}
	inline const Vector3& Set
	(GLdouble x, GLdouble z, GLdouble y) {
		rotationAxis.x = x;
		rotationAxis.z = z;
		rotationAxis.y = y;
		return rotationAxis;
	}
	inline const Vector3 GetSize() const {
		return Vector3(xlen, zlen, ylen);
	}
};
typedef Object Transform;

class Oval : public Object {
public:
	Oval() {
		shape = OVAL;
	}
	Oval(string name, GLdouble _xlen,GLdouble _zlen, GLdouble _ylen, int r = 0, int g = 0, int b = 0, GLdouble a = 1, GLdouble _rotation = 0) : Object(_xlen,_zlen, _ylen, _rotation, OVAL, r, g, b, a) {
		shape = OVAL;
		this->name = name;
	}
	Oval(string name, GLdouble _xlen,  GLdouble _zlen, GLdouble _ylen, MyColor mColor, GLdouble _rotation = 0) : Object(_xlen, _zlen, _ylen, _rotation, OVAL, mColor) {
		shape = OVAL;
		this->name = name;
	}
};
class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
	Box(string name, GLdouble _xlen, GLdouble _zlen, GLdouble _ylen, int r = 0, int g = 0, int b = 0, GLdouble a = 1, GLdouble _rotation = 0) : Object(_xlen, _zlen, _ylen, _rotation, BOX, r, g, b, a) {
		shape = BOX;
		this->name = name;
	}
	Box(string name, GLdouble _xlen, GLdouble _zlen, GLdouble _ylen, MyColor mColor, GLdouble _rotation = 0) : Object(_xlen, _zlen, _ylen, _rotation, BOX, mColor) {
		shape = BOX;
		this->name = name;
	}
};
class Triangle : public Object {
public:
	Triangle() {
		shape = TRIANGLE;
	}
	Triangle(string _name, GLdouble _xlen, GLdouble _zlen, GLdouble _ylen, int r = 0, int g = 0, int b = 0, GLdouble a = 1, GLdouble _rotation = 0) : Object(xlen,zlen, ylen, _rotation, TRIANGLE, r,g, b, a) {
		shape = TRIANGLE;
		name = _name;
	}
	Triangle(string _name, GLdouble _xlen, GLdouble _zlen, GLdouble _ylen, MyColor mColor, GLdouble _rotation = 0) : Object(xlen,zlen, ylen, _rotation, TRIANGLE, mColor) {
		shape = TRIANGLE;
		name = _name;
	}
};


class ObjectWithComponent {
public:	
	class CollisionComponent_ {
	public:
		Transform* collisionObject=NULL; //Collision object를 의미.
		Node<ObjectWithComponent*, string>* gameObjectNode=NULL; //이 Component를 가진 GameObjectNode를 가르킴.
		CollisionComponent_(Transform* collisionObject, Node<ObjectWithComponent*, string>* parentObject);
		~CollisionComponent_();
		const Vector3 GetWorldPos() const;
		inline const GLdouble Getxlen() const {
			return collisionObject->xlen;
		}
		inline const GLdouble Getzlen() const {
			return collisionObject->zlen;
		}
		inline const GLdouble Getylen() const {
			return collisionObject->ylen;
		}
		inline const Object::Shape GetShape() const {
			return collisionObject->shape;
		}
		const Vector3 GetVelocity() const;
		static void ConnectCollisionManagerAddFunction(void(*g) (CollisionComponent_* collisionComponent));
	private:
		typedef void(*FuncPointer) (CollisionComponent_* collisionComponent);
		static FuncPointer f1;
	};
	Transform* object; //Object의 Transform Component
	CollisionComponent_* collisionComponent=NULL; // object의 collision Component
	ObjectWithComponent(Transform* object = NULL, CollisionComponent_* collisionComponent = NULL);
	~ObjectWithComponent();
	void AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble z, GLdouble y, GLdouble xlen, GLdouble zlen, GLdouble ylen, GLdouble rotation, Node<ObjectWithComponent*, string>* parentObject);
	void AddCollisionComponentAsItself(Node<ObjectWithComponent*, string>* parentObject);
};
typedef ObjectWithComponent::CollisionComponent_ CollisionComponent;
typedef ObjectWithComponent GameObject;
typedef Node<ObjectWithComponent*, string> GameObjectNode;
typedef BinaryTree<GameObject*, string> GameObjectTree;