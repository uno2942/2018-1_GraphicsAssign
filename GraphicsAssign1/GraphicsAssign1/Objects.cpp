#include "Objects.h"

Object::Object() : shape(Shape::BOX), rotation(0), xlen(0), ylen(0), zlen(0), myColor(0, 0, 0) {
}

ObjectWithComponent::CollisionComponent_::CollisionComponent_(Transform* collisionObject, Node<ObjectWithComponent*, string>* parentObject) {
	this->collisionObject = collisionObject;
	this->gameObjectNode = parentObject;
	if (f1 != NULL)
		f1(this);
}

ObjectWithComponent::CollisionComponent_::~CollisionComponent_() {
	if (collisionObject != NULL)
	{
		delete collisionObject;
		collisionObject = NULL;
	}
}
const Vector3 ObjectWithComponent::CollisionComponent_::GetWorldPos() const {
	Node<ObjectWithComponent*, string>* temp = gameObjectNode;
	Vector3 worldpos = collisionObject->position;
	while (temp != NULL)
	{
		worldpos += temp->data->object->position;
		temp = temp->precessor;
	}
	return worldpos;
}

const Vector3 ObjectWithComponent::CollisionComponent_::GetVelocity() const {
	Node<ObjectWithComponent*, string>* temp = gameObjectNode;
	Vector3 worldvel = collisionObject->velocity;
	while (temp != NULL)
	{
		worldvel += temp->data->object->velocity;
		temp = temp->precessor;
	}
	return worldvel;
}

ObjectWithComponent::CollisionComponent_::FuncPointer ObjectWithComponent::CollisionComponent_::f1 = NULL;
void ObjectWithComponent::CollisionComponent_::ConnectCollisionManagerAddFunction(void(*g) (CollisionComponent_* collisionComponent))
{
	f1 = g;
}


ObjectWithComponent::ObjectWithComponent(Transform* object, CollisionComponent_* collisionComponent) {
	this->object = object;
	this->collisionComponent = collisionComponent;
}
ObjectWithComponent::~ObjectWithComponent() {
	if (object != NULL)
	{
		delete object;
		object = NULL;
	}
	if (collisionComponent != NULL)
	{
		delete collisionComponent;
		collisionComponent = NULL;
	}
}

void ObjectWithComponent::AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble z, GLdouble y, GLdouble xlen, GLdouble zlen, GLdouble ylen, GLdouble rotation, Node<ObjectWithComponent*, string>* parentObject) {
	collisionComponent = new CollisionComponent_(new Transform(xlen, zlen, ylen, rotation, shape, x, z, y), parentObject);
}
void ObjectWithComponent::AddCollisionComponentAsItself(Node<ObjectWithComponent*, string>* parentObject) {
	collisionComponent = new CollisionComponent_(new Transform(object->xlen,object->zlen, object->ylen, object->rotation, object->shape, 0, 0, 0), parentObject);
}
