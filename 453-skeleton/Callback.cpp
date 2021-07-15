#include "Callback.h"
#include "GameObject.h"
#include "ShaderProgram.h"

MyCallbacks::MyCallbacks(ShaderProgram & shader) : shader(shader) {}

void MyCallbacks::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		restartFlag = true;
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS))
	{
		setMovement(MovementDirection::Forward);
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS))
	{
		setMovement(MovementDirection::Backward);
	}
}

void MyCallbacks::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwGetCursorPos(window, &xpos, &ypos);

		//From MouseInput Tutorial on D2L
		glm::vec2 startingVec((float)xpos, (float)ypos);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f);
		glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(width, height);
		glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
		glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f);
		clickedMousePosition = { final.x, final.y, 0.f, 1.f };
	}
}

glm::vec4 MyCallbacks::getClickedMousePosition() const
{
	return clickedMousePosition;
}

MovementDirection MyCallbacks::getMovement() const
{
	return movement;
}

bool MyCallbacks::getRestartFlag() const
{
	return restartFlag;
}

void MyCallbacks::setMovement(MovementDirection movementDir)
{
	movement = movementDir;
}

void MyCallbacks::reset()
{
	restartFlag = false;
	clickedMousePosition = { 0, 0, 0, 1 };
	movement = MovementDirection::NoMovement;
}


glm::mat4 callbackMovementCalculations(std::shared_ptr<MyCallbacks> callbacks,
										const unsigned maxAnimationFrames,
										unsigned& animationCurrentFrame,
										glm::vec4& clickedMousePos,
										std::shared_ptr<GameObject> ship) {
	glm::mat4 accumulatedMatrix(1.f);
	if (callbacks->getMovement() != MovementDirection::NoMovement) {
		animationCurrentFrame = 1;
		glm::mat4 translationMatrix;
		glm::vec4 headingUnitVector = ship->getHeading() / (glm::length(ship->getHeading()) * 5 * maxAnimationFrames);
		if (callbacks->getMovement() == MovementDirection::Forward) {
			translationMatrix = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				headingUnitVector.x, headingUnitVector.y, 0.f, 1.f
			};
		}
		else if (callbacks->getMovement() == MovementDirection::Backward) {
			translationMatrix = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				-headingUnitVector.x, -headingUnitVector.y, 0.f, 1.f
			};
		}
		accumulatedMatrix = translationMatrix * accumulatedMatrix;
		callbacks->setMovement(MovementDirection::NoMovement);
	}

	if (callbacks->getClickedMousePosition() != clickedMousePos)
	{
		animationCurrentFrame = 1;
		clickedMousePos = callbacks->getClickedMousePosition();

		glm::vec4 shipHeading = ship->getHeading();
		glm::vec4 shipPosition = ship->getPosition();
		glm::vec4 newHeading = clickedMousePos - ship->getPosition();
		newHeading /= glm::length(newHeading);

		float rotationInRadians = acos(glm::dot(shipHeading, newHeading) / (glm::length(shipHeading) * glm::length(newHeading)));
		rotationInRadians /= maxAnimationFrames;
		glm::vec3 crossProduct = glm::cross(glm::vec3(shipHeading), glm::vec3(newHeading));
		if (crossProduct.z < 0)
		{
			rotationInRadians *= -1;
		}

		glm::mat4 rotationMatrix = {
			cos(rotationInRadians), sin(rotationInRadians), 0.f, 0.f,
			-sin(rotationInRadians), cos(rotationInRadians), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f,
		};
		glm::mat4 negativeTranslation = {
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			-shipPosition.x, -shipPosition.y, 0.f, 1.f
		};
		glm::mat4 positiveTranslation = {
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			shipPosition.x, shipPosition.y, 0.f, 1.f
		};
		accumulatedMatrix = positiveTranslation * rotationMatrix * negativeTranslation * accumulatedMatrix;
	}
	return accumulatedMatrix;
}
