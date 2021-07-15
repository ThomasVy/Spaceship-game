#pragma once
#include "Window.h"
class ShaderProgram;
struct GameObject;
class GLFWwindow;

enum class MovementDirection
{
	NoMovement,
	Forward,
	Backward
};

class MyCallbacks : public CallbackInterface {
public:
	MyCallbacks(ShaderProgram& shader);
	void keyCallback(int key, int scancode, int action, int mods) override;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
	glm::vec4 getClickedMousePosition() const;
	MovementDirection getMovement() const;
	bool getRestartFlag() const;
	void setMovement(MovementDirection movementDir);
	void reset();

private:
	bool restartFlag = false;
	glm::vec4 clickedMousePosition = { 0, 0, 0, 1 };
	ShaderProgram& shader;
	MovementDirection movement = MovementDirection::NoMovement;
};

glm::mat4 callbackMovementCalculations(std::shared_ptr<MyCallbacks> callbacks,
										const unsigned maxAnimationFrames,
										unsigned& animationCurrentFrame,
										glm::vec4& clickedMousePos,
										std::shared_ptr<GameObject> ship);
