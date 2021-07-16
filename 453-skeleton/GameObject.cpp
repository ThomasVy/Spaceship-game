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
bool GameObject::getCollectable() const {
	return collectable;
}
float GameObject::getScale() const {
	return scale;
}
glm::vec3 GameObject::getHeading() const {
	glm::mat4 identityMatrix(1.0f);
	glm::mat4 rotationMatrix = glm::rotate(identityMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	return rotationMatrix * glm::vec4(0, 1, 0, 0);
}
void GameObject::setParentObject(const std::shared_ptr<GameObject>& parentObject) {
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
void GameObject::setCollectable(const bool collectable) {
	this->collectable = collectable;
}
void GameObject::setScale(const float scale) {
	this->scale = scale;
}
glm::mat4 GameObject::getTransformationMatrix() const {
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
}

glm::mat4 GameObject::getTranslationMatrix() const {
	if (parentObject != nullptr) {
		return parentObject->getTranslationMatrix() * glm::translate(glm::mat4(1.0f), position);
	}
	return glm::translate(glm::mat4(1.0f), position);
}
glm::mat4 GameObject::getRotationMatrix() const {
	return glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 0.0f, 1.0f));
}
glm::mat4 GameObject::getScaleMatrix() const {
	return glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));
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

		for (int childIndex = 0; childIndex < childObjects.size(); childIndex++) {
			int childNumber = childIndex + 1;
			float childFactor = 0.25f + childNumber * 0.1;
			childObjects[childIndex]->setPosition(childFactor*-getHeading());
		}

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
void GameObject::addChild(const std::shared_ptr<GameObject>& childObject) {
	childObjects.push_back(childObject);
}

std::vector<std::shared_ptr<GameObject>> GameObject::getChildren() const {
	return childObjects;
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

void Fire::setLength(float length) {
	this->length = length;
}
