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
	if (key == GLFW_KEY_W )
	{
		if (action == GLFW_PRESS) {
			isWDown = true;
		}
		else if (action == GLFW_RELEASE) {
			isWDown = false;
		}
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS) {
			isSDown = true;
		}
		else if (action == GLFW_RELEASE) {
			isSDown = false;
		}
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

		glm::vec2 startingVec((float)xpos, (float)ypos);
		glm::vec2 shiftedVec = startingVec + glm::vec2(0.5f);
		glm::vec2 scaledToZeroOne = shiftedVec / glm::vec2(width, height);
		glm::vec2 flippedY = glm::vec2(scaledToZeroOne.x, 1.0f - scaledToZeroOne.y);
		glm::vec2 final = flippedY * 2.0f - glm::vec2(1.0f);
		clickedMousePosition = { final.x, final.y, 0.f};
	}
}

glm::vec3 MyCallbacks::getClickedMousePosition() const
{
	return clickedMousePosition;
}


bool MyCallbacks::getRestartFlag() const
{
	return restartFlag;
}

void MyCallbacks::reset()
{
	restartFlag = false;
	clickedMousePosition = {0, 1, 0};
	isWDown = false;
	isSDown = false;
}

bool MyCallbacks::getWDown() const {
	return isWDown;
}

bool MyCallbacks::getSDown() const {
	return isSDown;
}
