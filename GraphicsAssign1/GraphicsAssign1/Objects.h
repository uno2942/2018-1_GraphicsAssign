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
	Object(int _width, int _height, GLdouble _rotation, Shape shape, GLdouble x = 0, GLdouble y = 0, GLdouble vecx = 0, GLdouble vecy = 0) : width(_width), height(_height), rotation(_rotation) {
		position = Vector2(x, y);
		velocity = Vector2(vecx, vecy);

	}
	int width;
	int height;

	string name;

	Vector2 position;
	Vector2 velocity;
	GLdouble rotation;
	bool isFixed = false;
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
typedef Object Transform;

class Oval : public Object {
public:
	Oval() {
		shape = OVAL;
	}
	Oval(string name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, OVAL) {
		shape = OVAL;
		this->name = name;
	}
};
class Box : public Object { //플레이어와 적의 Object
public:
	Box() {
		shape = BOX;
	}
	Box(string name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, BOX) {
		shape = BOX;
		this->name = name;
	}
};
class Triangle : public Object {
public:
	Triangle() {
		shape = TRIANGLE;
	}
	Triangle(string _name, int _width, int _height, GLdouble _rotation = 0) : Object(_width, _height, _rotation, TRIANGLE) {
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
		const Vector2 GetWorldPos() const;
		inline const int GetWidth() const {
			return collisionObject->width;
		}
		inline const int GetHeight() const {
			return collisionObject->height;
		}
		inline const Object::Shape GetShape() const {
			return collisionObject->shape;
		}
		static void ConnectCollisionManagerAddFunction(void(*g) (CollisionComponent_* collisionComponent));
	private:
		typedef void(*FuncPointer) (CollisionComponent_* collisionComponent);
		static FuncPointer f1;
	};
	Transform* object; //Object의 Transform Component
	CollisionComponent_* collisionComponent=NULL; // object의 collision Component
	ObjectWithComponent(Transform* object = NULL, CollisionComponent_* collisionComponent = NULL);
	~ObjectWithComponent();
	void AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble y, int width, int height, GLdouble rotation, Node<ObjectWithComponent*, string>* parentObject);
	void AddCollisionComponentAsItself(Node<ObjectWithComponent*, string>* parentObject);
};
typedef ObjectWithComponent::CollisionComponent_ CollisionComponent;
typedef ObjectWithComponent GameObject;
typedef Node<ObjectWithComponent*, string> GameObjectNode;
typedef BinaryTree<GameObject*, string> GameObjectTree;

