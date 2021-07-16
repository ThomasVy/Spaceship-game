#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "ImguiUtil.h"
#include "Callback.h"
#include "GameObject.h"
#include "SetupObjects.h"
#include "Animation.h"

int main() {
	// WINDOW
	glfwInit();
	Window window(800, 800, "Spaceship Game"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	auto callbacks = std::make_shared<MyCallbacks>(shader);
	window.setCallbacks(callbacks); // can also update callbacks to new ones

	const unsigned maxDiamonds = 5;

	//SETUP
	std::shared_ptr<Ship> ship;
	std::vector<std::shared_ptr<Gem>> gems;
	std::vector<std::shared_ptr<Fire>> fires;
	setupObjects(ship, gems, fires, maxDiamonds);

	const CPU_Geometry cgeom = createGeom();
	GPU_Geometry ggeom;
	ggeom.setVerts(cgeom.verts);
	ggeom.setTexCoords(cgeom.texCoords);

	// RENDER LOOP
	int score = 0;
	bool gameOver = false, lose = false;
	shader.use();
	auto loc = glGetUniformLocation(shader.getProgram(), "samplers");
	int samplers[3] = { 0, 1, 2 };
	glUniform1iv(loc, 3, samplers);
	// But for most other cases, you'd want GL_LINEAR interpolation.
	// GL_NEAREST looks a bit better for low-res pixel art than GL_LINEAR.
	Texture shipTexture("textures/ship.png", GL_NEAREST);
	Texture diamondTexture("textures/diamond.png", GL_NEAREST);
	Texture fireTexture("textures/fire.png", GL_NEAREST);
	glBindTextureUnit(0, shipTexture.getTextureID());
	glBindTextureUnit(1, diamondTexture.getTextureID());
	glBindTextureUnit(2, fireTexture.getTextureID());

	while (!window.shouldClose()) {
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		if (callbacks->getRestartFlag() == true)
		{
			score = 0;
			callbacks->reset();
			setupObjects(ship, gems, fires, maxDiamonds);
			gameOver = false;
		}
		if (!gameOver) {
			lose = checkGameProgression(ship, gems, fires, score);
			moveShip(callbacks, ship);
		}
		update(ship, gems, fires, gameOver);
		std::vector<glm::mat4> modelMatrices;
		std::vector<float> texIds;
		setMatricesAndTextures(gems,
			ship,
			modelMatrices,
			fires,
			texIds);
		ggeom.setMatrixTrans(modelMatrices);
		ggeom.setTextureID(texIds);
		ggeom.bind();
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, texIds.size());

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui
		imguiSetup();
		drawScoreBoard(score);
		if (score == maxDiamonds) {
			drawWinScreen();
			gameOver = true;
		}
		else if (lose) {
			drawLoseScreen();
			gameOver = true;
		}
		imguiRender();

		window.swapBuffers();
	}

	imguiShutdown();
	glfwTerminate();
	return 0;
}
