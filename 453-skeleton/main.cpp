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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

CPU_Geometry createGeom() {
	CPU_Geometry retGeom;

	// For full marks (Part IV), you'll need to use the following vertex coordinates instead.
	// Then, you'd get the correct scale/translation/rotation by passing in uniforms into
	// the vertex shader.
	retGeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(-1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(-1.f, 1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, -1.f, 0.f));
	retGeom.verts.push_back(glm::vec3(1.f, 1.f, 0.f));


	// texture coordinates
	retGeom.texCoords.push_back(glm::vec2(0.f, 1.f));
	retGeom.texCoords.push_back(glm::vec2(0.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(0.f, 1.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 0.f));
	retGeom.texCoords.push_back(glm::vec2(1.f, 1.f));
	return retGeom;
}

// An example struct for Game Objects.
// You are encouraged to customize this as you see fit.
struct GameObject {
	// Struct's constructor deals with the texture.
	// Also sets default position, theta, scale, and transformationMatrix
	GameObject(const glm::mat4& startingTransformationMatrix)
		: cgeom(createGeom()),
		originalTransformationMatrix(startingTransformationMatrix),
		transformationMatrix(startingTransformationMatrix)
	{
	}

	void reset()
	{
		transformationMatrix = originalTransformationMatrix;
		isVisible = true;
	}

	glm::vec4 getPosition()
	{
		return  transformationMatrix * position;
	}

	glm::vec4 getHeading()
	{
		return  transformationMatrix * heading;
	}

	const CPU_Geometry cgeom;
	glm::mat4 transformationMatrix;
	bool isVisible = true;

private:
	const glm::mat4 originalTransformationMatrix;
	const glm::vec4 position = { 0.f, 0.f, 0.f, 1.f };
	const glm::vec4 heading = { 0.f, 0.5f, 0.f, 0.f };

};

enum class MovementDirection
{
	NoMovement,
	Forward,
	Backward
};

// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	void keyCallback(int key, int scancode, int action, int mods) override
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

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override
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

	glm::vec4 getClickedMousePosition() const
	{
		return clickedMousePosition;
	}

	MovementDirection getMovement() const
	{
		return movement;
	}

	bool getRestartFlag() const
	{
		return restartFlag;
	}

	void setMovement(MovementDirection movementDir)
	{
		movement = movementDir;
	}

	void reset()
	{
		restartFlag = false;
		clickedMousePosition = { 0, 0, 0, 1 };
		movement = MovementDirection::NoMovement;
	}

private:
	bool restartFlag = false;
	glm::vec4 clickedMousePosition = { 0, 0, 0, 1 };
	ShaderProgram& shader;
	MovementDirection movement = MovementDirection::NoMovement;
};

glm::mat4 callbackMovementCalculations( std::shared_ptr<MyCallbacks> callbacks,
								const unsigned maxAnimationFrames,
								unsigned& animationCurrentFrame,
								glm::vec4& clickedMousePos,
								std::shared_ptr<GameObject> ship)
{
	glm::mat4 accumulatedMatrix(1.f);
	if (callbacks->getMovement() != MovementDirection::NoMovement)
	{
		animationCurrentFrame = 1;
		glm::mat4 translationMatrix;
		glm::vec4 headingUnitVector = ship->getHeading() / (glm::length(ship->getHeading()) * 5*  maxAnimationFrames);
		if (callbacks->getMovement() == MovementDirection::Forward)
		{
			translationMatrix = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				headingUnitVector.x, headingUnitVector.y, 0.f, 1.f
			};
		}
		else if (callbacks->getMovement() == MovementDirection::Backward)
		{
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
// END EXAMPLES

int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	auto callbacks = std::make_shared<MyCallbacks>(shader);
	window.setCallbacks(callbacks); // can also update callbacks to new ones

	// GL_NEAREST looks a bit better for low-res pixel art than GL_LINEAR.
	// But for most other cases, you'd want GL_LINEAR interpolation.
	const unsigned maxDiamonds = 4;
	unsigned objectsOnScreen = maxDiamonds + 1;

	//OpenGL Draw Instancing variables
	GPU_Geometry ggeom;
	std::vector<glm::vec3> positions; //This is used to collect all the vert positions to feed them in the draw instancing
	std::vector<glm::vec2> texCoords; //This is used to collect all the tex coords to feed them in the draw instancing
	std::vector<float> texIds; //This is the colleciton of texture ids for all objects on screen
	auto ship = std::make_shared<GameObject>(
		glm::mat4{
			0.09f, 0.f, 0.f, 0.f,
			0.f, 0.06f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}
	);
	positions.insert(positions.end(), ship->cgeom.verts.begin(), ship->cgeom.verts.end());
	texCoords.insert(texCoords.end(), ship->cgeom.texCoords.begin(), ship->cgeom.texCoords.end());
	texIds.push_back(0);

	std::vector<std::shared_ptr<GameObject>> gems;
	srand(3);
	for (unsigned diamondIndex = 0; diamondIndex < maxDiamonds; diamondIndex++)
	{
		double randomX = rand()*1.5/ RAND_MAX - 0.75;
		double randomY = rand()*1.5/ RAND_MAX - 0.75;
		auto diamond = std::make_shared<GameObject>(
			glm::mat4{
				0.07f, 0.f, 0.f, 0.f,
				0.f, 0.07f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				randomX, randomY, 0.f, 1.f
			}
		);
		gems.push_back(diamond);
		positions.insert(positions.end(), diamond->cgeom.verts.begin(), diamond->cgeom.verts.end());
		texCoords.insert(texCoords.end(), diamond->cgeom.texCoords.begin(), diamond->cgeom.texCoords.end());
		texIds.push_back(1);
	}
	ggeom.setTexCoords(texCoords);
	ggeom.setVerts(positions);
	ggeom.setTextureID(texIds);

	// RENDER LOOP
	glm::vec4 clickedMousePos = { 0, 0, 0, 1 };
	const unsigned maxAnimationFrames = 10;
	unsigned animationCurrentFrame = maxAnimationFrames;
	int score = 0;
	glm::mat4 accumulatedMatrix(1.f);

	shader.use();
	auto loc = glGetUniformLocation(shader.getProgram(), "samplers");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);
	Texture shipTexture("textures/ship.png", GL_NEAREST);
	Texture diamondTexture("textures/diamond.png", GL_NEAREST);
	glBindTextureUnit(0, shipTexture.getTextureID());
	glBindTextureUnit(1, diamondTexture.getTextureID());

	while (!window.shouldClose()) {
		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		if (callbacks->getRestartFlag() == true)
		{
			score = 0;
			objectsOnScreen = maxDiamonds + 1;
			clickedMousePos = { 0, 0, 0, 1 };
			callbacks->reset();
			animationCurrentFrame = maxAnimationFrames;
			ship->reset();
			for (auto& gem : gems)
			{
				gem->reset();
			}
		}
		if (animationCurrentFrame >= maxAnimationFrames)
		{
			//clear the accumulated matrix to stop animating
			accumulatedMatrix = {
				1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f,  0.f, 0.f, 1.f
			};
		}
		accumulatedMatrix *= callbackMovementCalculations(callbacks,
			maxAnimationFrames,
			animationCurrentFrame,
			clickedMousePos,
			ship
		);
		ship->transformationMatrix = accumulatedMatrix * ship->transformationMatrix;
		std::vector<glm::mat4> modelMatrices;
		modelMatrices.push_back(ship->transformationMatrix);
		for (auto& gem : gems)
		{
			if (gem->isVisible)
			{
				modelMatrices.push_back(gem->transformationMatrix);
				glm::vec3 vectorBetweenShipAndGem = glm::vec3(gem->getPosition()) - glm::vec3(ship->getPosition());
				float lengthBetweenShipAndGem = glm::length(vectorBetweenShipAndGem);
				if (lengthBetweenShipAndGem <= 0.14)
				{
					glm::mat4 scalingMatrix = {
						1.3f, 0.f, 0.f, 0.f,
						0.f, 1.3f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f
					};
					glm::vec4 shipPosition = ship->getPosition();
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
					ship->transformationMatrix = positiveTranslation * scalingMatrix * negativeTranslation * ship->transformationMatrix;
					gem->isVisible = false;
					score++;
					objectsOnScreen--;
				}
			}
		}
		
		ggeom.setMatrixTrans(modelMatrices);
		ggeom.bind();
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, objectsOnScreen);
		
		animationCurrentFrame++;

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui


		// Starting the new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Putting the text-containing window in the top-left of the screen.
		ImGui::SetNextWindowPos(ImVec2(5, 5));

		// Setting flags
		ImGuiWindowFlags textWindowFlags =
			ImGuiWindowFlags_NoMove |				// text "window" should not move
			ImGuiWindowFlags_NoResize |				// should not resize
			ImGuiWindowFlags_NoCollapse |			// should not collapse
			ImGuiWindowFlags_NoSavedSettings |		// don't want saved settings mucking things up
			ImGuiWindowFlags_AlwaysAutoResize |		// window should auto-resize to fit the text
			ImGuiWindowFlags_NoBackground |			// window should be transparent; only the text should be visible
			ImGuiWindowFlags_NoDecoration |			// no decoration; only the text should be visible
			ImGuiWindowFlags_NoTitleBar;			// no title; only the text should be visible

		// Begin a new window with these flags. (bool *)0 is the "default" value for its argument.
		ImGui::Begin("scoreText", (bool*)0, textWindowFlags);

		// Scale up text a little, and set its value
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Score: %d", score); // Second parameter gets passed into "%d"
		if (score >= maxDiamonds)
		{
			ImGui::SetWindowFontScale(5.f);
			ImGui::Text("You Win!"); 
			ImGui::Text("Press R to restart");
		}

		// End the window.
		ImGui::End();

		ImGui::Render();	// Render the ImGui window
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Some middleware thing

		window.swapBuffers();
	}
	// ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
