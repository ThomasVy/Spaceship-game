#pragma once
#include "Geometry.h"
#include <iostream>
#include "GameObject.h"

CPU_Geometry createGeom();

void drawVisibleGems(std::vector<std::shared_ptr<Gem>>& gems,
				std::shared_ptr<GameObject>& ship,
				std::vector<glm::mat4>& modelMatrices,
				std::vector<float>& texIds,
				int& score);
void setupObjects(std::shared_ptr<GameObject>& ship,
					std::vector<std::shared_ptr<Gem>>& gems,
					const unsigned maxDiamonds);
