#include <time.h>
#include "SetupObjects.h"
#include "GameObject.h"
#include "Animation.h"
#include <iostream>
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
	return glm::length(vectorBetweenObjects) <= object1->getScale();	
}
bool checkGameProgression(std::shared_ptr<Ship>& ship,
						std::vector<std::shared_ptr<Gem>>& gems,
						std::vector<std::shared_ptr<Fire>>& fires,
						int& score) {
	for (const auto& gem : gems) {
		if (gem->getVisibility()) {
			if (isTouching(ship, gem))
			{
				ship->setScale(ship->getScale() + 0.05);
				gem->setVisibility(false);
				gem->getChild()->setVisibility(false);
				score++;
			}
		}
	}
	for (auto& fire : fires) {
		if (fire->getVisibility()){
			if (isTouching(ship, fire)) {
				return true;
			}
		}
	}
	return false;
}
void setMatricesAndTextures(std::vector<std::shared_ptr<Gem>>& gems,
				std::shared_ptr<Ship>& ship,
				std::vector<glm::mat4>& modelMatrices,
				std::vector<std::shared_ptr<Fire>>& fires,
				std::vector<float>& texIds) {
	modelMatrices.push_back(ship->getTransformationMatrix());
	texIds.push_back(ship->getTextureId());
	for (auto& gem : gems)
	{
		if (gem->getVisibility())
		{
			modelMatrices.push_back(gem->getTransformationMatrix());
			texIds.push_back(gem->getTextureId());
		}
	}
	for (auto& fire : fires) {
		if (fire->getVisibility()) {
			modelMatrices.push_back(fire->getTransformationMatrix());
			texIds.push_back(fire->getTextureId());
		}
	}
}
void setupObjects(std::shared_ptr<Ship>& ship,
				std::vector<std::shared_ptr<Gem>>& gems,
				std::vector<std::shared_ptr<Fire>>& fires,
				const unsigned maxDiamonds) {
	ship = std::make_shared<Ship>(glm::vec3(0.0, 0.0, 0.0), 0.07);
	srand(time(0));
	gems.clear();
	fires.clear();
	for (unsigned diamondIndex = 0; diamondIndex < maxDiamonds; diamondIndex++)
	{
		double randomX = rand() * 1.5 / RAND_MAX - 0.75;
		double randomY = rand() * 1.5 / RAND_MAX - 0.75;
		std::function<void(GameObject*)> diamondAni = [](GameObject*) {};
		auto diamond = std::make_shared<Gem>(glm::vec3(randomX, randomY, 0.0f), 0.07, diamondAni, nullptr);
		auto fire = std::make_shared<Fire>(glm::vec3(0.15, 0, 0), 0.035, 0.15, diamond);
		diamond->setChild(fire);
		gems.push_back(diamond);
		fires.push_back(fire);
	}
}
