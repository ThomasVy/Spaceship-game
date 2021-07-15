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

void drawVisibleGems(std::vector<std::shared_ptr<Gem>>& gems,
				std::shared_ptr<GameObject>& ship,
				std::vector<glm::mat4>& modelMatrices,
				std::vector<float>& texIds,
				int& score) {
	for (auto& gem : gems)
	{
		if (gem->getVisibility())
		{
			modelMatrices.push_back(gem->getTransformationMatrix());
			texIds.push_back(gem->getTextureId());
			modelMatrices.push_back(gem->getFire().getTransformationMatrix());
			texIds.push_back(gem->getFire().getTextureId());
			if (isTouching(ship, gem))
			{
				ship->setScale(ship->getScale()*1.2f);
				gem->setVisibility(false);
				score++;
			}
		}
	}
}
void setupObjects(std::shared_ptr<GameObject>& ship,
				std::vector<std::shared_ptr<Gem>>& gems,
				const unsigned maxDiamonds) {
	ship = std::make_shared<GameObject>(0.0, glm::vec3(0.0, 0.0, 0.0), 0.07);
	srand(time(0));
	gems.clear();
	for (unsigned diamondIndex = 0; diamondIndex < maxDiamonds; diamondIndex++)
	{
		double randomX = rand() * 1.5 / RAND_MAX - 0.75;
		double randomY = rand() * 1.5 / RAND_MAX - 0.75;
		auto diamond = std::make_shared<Gem>(glm::vec3(randomX, randomY, 0.0f), 0.07, 0.035);
		gems.push_back(diamond);
	}
}
