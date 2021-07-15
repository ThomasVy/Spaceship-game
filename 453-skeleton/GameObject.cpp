#include "GameObject.h"
#include <glm/gtx/transform.hpp>

GameObject::GameObject(const glm::mat4& startingTransformationMatrix, const float textureId)
	: originalTransformationMatrix(startingTransformationMatrix),
	textureId(textureId),
	transformationMatrix(startingTransformationMatrix)
{}

void GameObject::reset()
{
	transformationMatrix = originalTransformationMatrix;
	isVisible = true;
}

glm::vec4 GameObject::getPosition() const
{
	return  transformationMatrix * position;
}

glm::vec4 GameObject::getHeading()const
{
	return  transformationMatrix * heading;
}

Gem::Gem(const glm::mat4& startingTransformationMatrix)
	: GameObject(startingTransformationMatrix, 1.0)
	, fire(glm::scale(glm::translate(startingTransformationMatrix,glm::vec3(2.0f, 0.0, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f)), 2.0) {}
