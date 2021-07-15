#pragma once
#include <glm/glm.hpp>
struct GameObject {
public:
	GameObject(const glm::mat4& startingTransformationMatrix, const float textureId);
	void reset();
	glm::vec4 getPosition() const;
	glm::vec4 getHeading() const;
	glm::mat4 transformationMatrix;
	bool isVisible = true;
	float textureId;
private:
	const glm::mat4 originalTransformationMatrix;
	const glm::vec4 position = { 0.f, 0.f, 0.f, 1.f };
	const glm::vec4 heading = { 0.f, 0.5f, 0.f, 0.f };
};

struct Gem : public GameObject {
public:
	Gem(const glm::mat4& startingTransformationMatrix);
	GameObject fire;
};
