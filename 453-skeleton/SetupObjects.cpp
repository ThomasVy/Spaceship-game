#include <time.h>
#include "SetupObjects.h"
#include "GameObject.h"

CPU_Geometry createGeom() {
	CPU_Geometry retGeom;

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

bool isTouching(const std::shared_ptr<GameObject>& object1,
				const std::shared_ptr<GameObject>& object2) {
	glm::vec3 vectorBetweenObjects = glm::vec3(object2->getPosition()) - glm::vec3(object1->getPosition());
	return glm::length(vectorBetweenObjects) <= 0.14;	
}

void increaseShipSize (std::shared_ptr<GameObject>& ship) {
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
}

void drawVisibleGems(std::vector<std::shared_ptr<Gem>>& gems,
				std::shared_ptr<GameObject>& ship,
				std::vector<glm::mat4>& modelMatrices,
				std::vector<float>& texIds,
				int& score) {
	for (auto& gem : gems)
	{
		if (gem->isVisible)
		{
			modelMatrices.push_back(gem->transformationMatrix);
			texIds.push_back(gem->textureId);
			modelMatrices.push_back(gem->fire.transformationMatrix);
			texIds.push_back(gem->fire.textureId);
			if (isTouching(ship, gem))
			{
				increaseShipSize(ship);
				gem->isVisible = false;
				score++;
			}
		}
	}
}
void setupObjects(std::shared_ptr<GameObject>& ship,
				std::vector<std::shared_ptr<Gem>>& gems,
				const unsigned maxDiamonds) {
	ship = std::make_shared<GameObject>(
		glm::mat4{
			0.09f, 0.f, 0.f, 0.f,
			0.f, 0.06f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}, 0.0
	);
	srand(time(0));
	gems.clear();
	for (unsigned diamondIndex = 0; diamondIndex < maxDiamonds; diamondIndex++)
	{
		double randomX = rand() * 1.5 / RAND_MAX - 0.75;
		double randomY = rand() * 1.5 / RAND_MAX - 0.75;
		auto diamond = std::make_shared<Gem>(
			glm::mat4{
				0.07f, 0.f, 0.f, 0.f,
				0.f, 0.07f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				randomX, randomY, 0.f, 1.f
			}
		);
		gems.push_back(diamond);
	}
}
