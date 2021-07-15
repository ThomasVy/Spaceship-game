#pragma once
#include <glm/glm.hpp>

enum class MovementDirection
{
	NoMovement,
	Forward,
	Backward
};

class GameObject {
public:
	GameObject(const float textureId, const glm::vec3& position, const float scale);
	glm::vec3 getPosition() const;
	float getTheta() const;
	float getTextureId() const;
	bool getVisibility() const;
	float getScale() const;
	glm::mat4 getTransformationMatrix() const;
	glm::vec3 getHeading() const;
	MovementDirection getMoving() const;

	void setPosition(const glm::vec3& pos);
	void setTheta(const float angle);
	void setTextureId(const float texId);
	void setVisibility(const bool visibility);
	void setScale(const float scale);
	void setTargetTheta(const float target);
	void setMoving(const MovementDirection move);
	void update();
private:
	bool isVisible = true;
	float textureId;
	glm::vec3 position = {0.f, 0.f, 0.f};
	float theta = 0.0f;
	float scale = 1.0f;
	float targetTheta = 0.0f;
	MovementDirection movementDirection = MovementDirection::NoMovement;
};

class Gem : public GameObject {
public:
	Gem(const glm::vec3& gemPos, const float gemScale, const float fireScale);
	GameObject& getFire();
private:
	GameObject fire;
};
