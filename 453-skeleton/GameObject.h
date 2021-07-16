#pragma once
#include <glm/glm.hpp>
#include <functional>

enum class MovementDirection
{
	NoMovement,
	Forward,
	Backward
};

class GameObject {
public:
	GameObject(const float textureId,
			const glm::vec3& position,
			const float scale,
			const std::shared_ptr<GameObject>& parentObject);
	virtual glm::vec3 getPosition() const;
	virtual float getTheta() const;
	virtual float getTextureId() const;
	virtual bool getVisibility() const;
	virtual float getScale() const;
	virtual glm::mat4 getTransformationMatrix() const;
	virtual glm::vec3 getHeading() const;

	virtual void setPosition(const glm::vec3& pos);
	virtual void setTheta(const float angle);
	virtual void setTextureId(const float texId);
	virtual void setVisibility(const bool visibility);
	virtual void setScale(const float scale);
	virtual void setParentObject(std::shared_ptr<GameObject>& parentObject);
	virtual void update() = 0;
protected:
	bool isVisible = true;
	float textureId;
	glm::vec3 position = {0.f, 0.f, 0.f};
	float theta = 0.0f;
	float scale = 1.0f;
	std::shared_ptr<GameObject> parentObject;
};

class Ship : public GameObject {
public:
	Ship(const glm::vec3& position, const float scale);
	void setTargetTheta(const float target);
	float getTargetTheta() const;
	void setMoving(const MovementDirection move);
	MovementDirection getMoving() const;
	void update() override;
private:
	float targetTheta = 0.0f;
	MovementDirection movementDirection = MovementDirection::NoMovement;
};

class Gem : public GameObject {
public:
	Gem(const glm::vec3& gemPos,
		const float gemScale,
		std::function<void(GameObject*)>& gemAni,
		const std::shared_ptr<GameObject>& parentObject);
	void update() override;
	void setAnimation(std::function<void(GameObject*)>& ani);
	void setChild(const std::shared_ptr<GameObject>& childObject);
	std::shared_ptr<GameObject> getChild();
private:
	std::function<void(GameObject*)> animation;
	std::shared_ptr<GameObject> childObject = nullptr;
};

class Fire : public GameObject {
public:
	Fire(const glm::vec3& pos,
		const float scale,
		const float length,
		const std::shared_ptr<GameObject>& parentObject);
	void update() override;
private:
	float rotateAngle = 0;
	float length;
};
