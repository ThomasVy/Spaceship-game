#include "GameObject.h"
#include <glm/gtx/transform.hpp>

GameObject::GameObject(const float textureId, const glm::vec3& position, const float scale)
	: textureId(textureId)
	, position(position)
	, scale(scale)
{}

glm::vec3 GameObject::getPosition() const {
	return position;
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
MovementDirection GameObject::getMoving() const {
	return movementDirection;
}

void GameObject::setPosition(const glm::vec3& pos) {
	position = pos;
}
void GameObject::setTheta(const float angle) {
	theta = theta;
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
void GameObject::setTargetTheta(const float target){
	targetTheta = target;
}
void GameObject::setMoving(const MovementDirection move) {
	movementDirection = move;
}

void GameObject::update() {
	float ANGLE_STEP = glm::radians(360.0f)/(60.0f * 5.0f);
	const float SPEED = 2.0 / (60.f * 10.0f);
	//update position
	switch (movementDirection)
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
			theta += glm::radians (360.0);
		}
	}
}
glm::mat4 GameObject::getTransformationMatrix() const {
	glm::mat4 identityMatrix(1.0f);
	glm::mat4 transformationMatrix = glm::translate(identityMatrix, position);
	glm::mat4 rotationMatrix = glm::rotate(identityMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale, scale, 1.0f));
	return transformationMatrix * rotationMatrix * scaleMatrix;
}

Gem::Gem(const glm::vec3& gemPos, const float gemScale, const float fireScale)
	: GameObject(1.0, gemPos, gemScale)
	, fire(2.0, glm::vec3(gemPos.x+0.15, gemPos.y, gemPos.z), fireScale)
{}

GameObject& Gem::getFire() {
	return fire;
}
