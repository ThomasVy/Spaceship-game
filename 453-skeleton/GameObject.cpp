#include "GameObject.h"
#include <glm/gtx/transform.hpp>

GameObject::GameObject(const float textureId,
	const glm::vec3& position,
	const float scale,
	const std::shared_ptr<GameObject>& parentObject)
	: textureId(textureId)
	, position(position)
	, scale(scale)
	, parentObject(parentObject)
{}

glm::vec3 GameObject::getPosition() const {
	if (parentObject == nullptr) {
		return position;
	}
	else {
		return parentObject->getPosition() + position;
	}
}
float GameObject::getTheta() const {
	return theta;
}
float GameObject::getTextureId() const {
	return textureId;
}
bool GameObject::getVisibility() const {
	return isVisible;
}
float GameObject::getScale() const {
	return scale;
}
glm::vec3 GameObject::getHeading() const {
	glm::mat4 identityMatrix(1.0f);
	glm::mat4 rotationMatrix = glm::rotate(identityMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	return rotationMatrix * glm::vec4(0, 1, 0, 0);
}
void GameObject::setParentObject(std::shared_ptr<GameObject>& parentObject) {
	this->parentObject = parentObject;
}
void GameObject::setPosition(const glm::vec3& pos) {
	position = pos;
}
void GameObject::setTheta(const float angle) {
	theta = angle;
}
void GameObject::setTextureId(const float texId) {
	textureId = texId;
}
void GameObject::setVisibility(const bool visibility) {
	isVisible = visibility;
}
void GameObject::setScale(const float scale) {
	this->scale = scale;
}
glm::mat4 GameObject::getTransformationMatrix() const {
	glm::mat4 identityMatrix(1.0f);
	glm::mat4 translation = glm::translate(identityMatrix, position);
	glm::mat4 rotationMatrix = glm::rotate(identityMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale, scale, 1.0f));
	if (parentObject == nullptr) {
		return translation * rotationMatrix * scaleMatrix;
	}
	else {
		return glm::translate(identityMatrix, parentObject->getPosition()) * translation * rotationMatrix * scaleMatrix;
	}
}
Ship::Ship(const glm::vec3& position, const float scale)
	: GameObject(0.0, position, scale, nullptr)
{}
MovementDirection Ship::getMoving() const {
	return movementDirection;
}
float Ship::getTargetTheta() const {
	return targetTheta;
}
void Ship::update() {
	float ANGLE_STEP = glm::radians(360.0f) / (60.0f * 5.0f);
	const float SPEED = 2.0 / (60.f * 10.0f);
	//update position
	switch (getMoving())
	{
	case MovementDirection::Forward:
		position += SPEED * getHeading();
		break;
	case MovementDirection::Backward:
		position -= SPEED * getHeading();
		break;
	}

	//update the rotation
	if (std::abs(theta - targetTheta) < ANGLE_STEP) {
		theta = targetTheta;
	}
	else {
		glm::mat4 identityMatrix(1.0f);
		glm::mat4 rotationMatrix = glm::rotate(identityMatrix, targetTheta, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 targetHeading = glm::vec3(rotationMatrix * glm::vec4(0, 1, 0, 1));
		glm::vec3 cross = glm::cross(targetHeading, getHeading());
		if (cross.z > 0) {
			ANGLE_STEP = -ANGLE_STEP;
		}
		theta += ANGLE_STEP;
		if (theta > glm::radians(180.0)) {
			theta -= glm::radians(360.0);
		}
		else if (theta < glm::radians(-180.0)) {
			theta += glm::radians(360.0);
		}
	}
}
void Ship::setTargetTheta(const float target){
	targetTheta = target;
}
void Ship::setMoving(const MovementDirection move) {
	movementDirection = move;
}

Gem::Gem(const glm::vec3& gemPos,
		const float gemScale,
		std::function<void(GameObject*)>& ani,
		const std::shared_ptr<GameObject>& parentObject)
	: GameObject(1.0, gemPos, gemScale, parentObject)
	, animation(ani)
{}

void Gem::setAnimation(std::function<void(GameObject*)>& ani) {
	animation = ani;
}

void Gem::update() {
	animation(this);
}
void Gem::setChild(const std::shared_ptr<GameObject>& childObject) {
	this->childObject = childObject;
}
std::shared_ptr<GameObject> Gem::getChild() {
	return childObject;
}

Fire::Fire(const glm::vec3& pos,
			const float scale,
			const float len,
			const std::shared_ptr<GameObject>& parentObject)
	: GameObject(2.0, pos, scale, parentObject)
	, length(len) {}

void Fire::update() {
	const float ROTATION_STEP = glm::radians(360.0f) / (60.0f * 5.0f);
	rotateAngle += ROTATION_STEP;
	position = { length * std::cos(rotateAngle), length * std::sin(rotateAngle), 0.0f };
	if (rotateAngle > glm::radians(360.0)) {
		rotateAngle -= glm::radians(360.0);
	}
}
