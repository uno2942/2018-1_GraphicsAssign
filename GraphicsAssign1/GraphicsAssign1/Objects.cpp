#include "Objects.h"

Object::Object() {

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
const Vector2 ObjectWithComponent::CollisionComponent_::GetWorldPos() const {
	Node<ObjectWithComponent*, string>* temp = gameObjectNode;
	Vector2 worldpos = collisionObject->position;
	while (temp != NULL)
	{
		worldpos += temp->data->object->position;
		temp = temp->precessor;
	}
	return worldpos;
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

void ObjectWithComponent::AddCollisionComponent(Object::Shape shape, GLdouble x, GLdouble y, int width, int height, GLdouble rotation, Node<ObjectWithComponent*, string>* parentObject) {
	collisionComponent = new CollisionComponent_(new Transform(width, height, rotation, shape, x, y), parentObject);
}
void ObjectWithComponent::AddCollisionComponentAsItself(Node<ObjectWithComponent*, string>* parentObject) {
	collisionComponent = new CollisionComponent_(new Transform(object->width, object->height, object->rotation, object->shape, 0, 0), parentObject);
}

