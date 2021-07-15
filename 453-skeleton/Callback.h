#pragma once
#include "Window.h"
class ShaderProgram;
struct GameObject;
class GLFWwindow;

class MyCallbacks : public CallbackInterface {
public:
	MyCallbacks(ShaderProgram& shader);
	void keyCallback(int key, int scancode, int action, int mods) override;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
	glm::vec3 getClickedMousePosition() const;
	bool getRestartFlag() const;
	void reset();
	bool getWDown() const;
	bool getSDown() const;
private:
	bool restartFlag = false;
	glm::vec3 clickedMousePosition = { 0, 1, 0};
	ShaderProgram& shader;
	bool isWDown = false;
	bool isSDown = false;
};
