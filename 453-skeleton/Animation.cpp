#include <cmath>
#include "Animation.h"
#include "Callback.h"
#include "GameObject.h"
#include <glm/gtx/transform.hpp>
void moveShip(const std::shared_ptr<MyCallbacks>& callbacks, std::shared_ptr<Ship>& ship) {
	ship->setMoving(MovementDirection::NoMovement);
	if (callbacks->getSDown()) {
		ship->setMoving(MovementDirection::Backward);
	}
	else if (callbacks->getWDown()) {
		ship->setMoving(MovementDirection::Forward);
	}
	static glm::vec3 oldMouse(0.0, 1.0, 0.0);
	if (oldMouse != callbacks->getClickedMousePosition()) {
		oldMouse = callbacks->getClickedMousePosition();
		glm::vec3 mouseVec = glm::normalize(callbacks->getClickedMousePosition() - ship->getPosition());
		glm::vec3 originalHeading = glm::vec3(0.0, 1.0, 0.0);
		float dot = std::acos(glm::dot(mouseVec, originalHeading));
		glm::vec3 cross = glm::cross(mouseVec, originalHeading);
		if (cross.z > 0) {
			dot = -dot;
		}
		ship->setTargetTheta(dot);
	}
}
