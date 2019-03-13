#include"Objects.h"

Vector2& Object::CurrentPosition() {
	return position;
}

Vector2 Object::SetPosition(Vector2 _position) {
	position = _position;
	return position;
}

Vector2 Object::SetPosition(GLdouble x, GLdouble y) {
	position.x = x;
	position.y = y;
	return position;
}

Vector2 Object::CurrentVelocity() {
	return velocity;
}

Vector2 Object::SetVelocity(Vector2 _velocity) {
	velocity = _velocity;
	return velocity;
}

Vector2 Object::SetVelocity(GLdouble x, GLdouble y) {
	velocity.x = x;
	velocity.y = y;
	return velocity;
}
