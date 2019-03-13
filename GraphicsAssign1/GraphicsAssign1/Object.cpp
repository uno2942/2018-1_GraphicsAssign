#include"Objects.h"

const Vector2& Object::GetCurrentPosition() const {
	return position;
}

const Vector2& Object::SetPosition(Vector2 _position) {
	position = _position;
	return position;
}

const Vector2& Object::SetPosition(GLdouble x, GLdouble y) {
	position.x = x;
	position.y = y;
	return position;
}

const Vector2& Object::GetCurrentVelocity() const {
	return velocity;
}

const Vector2& Object::SetVelocity(Vector2 _velocity) {
	velocity = _velocity;
	return velocity;
}

const Vector2& Object::SetVelocity(GLdouble x, GLdouble y) {
	velocity.x = x;
	velocity.y = y;
	return velocity;
}

const Vector2& Object::GetSize() const {
	return Vector2(width, height);
}
