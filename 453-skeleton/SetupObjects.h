#pragma once
#include "Geometry.h"
#include <iostream>
#include "GameObject.h"

CPU_Geometry createGeom();
bool checkGameProgression(std::shared_ptr<Ship>& ship,
						std::vector<std::shared_ptr<Gem>>& gems,
						std::vector<std::shared_ptr<Fire>>& fires,
						int& score);
void setMatricesAndTextures(std::vector<std::shared_ptr<Gem>>& gems,
							std::shared_ptr<Ship>& ship,
							std::vector<glm::mat4>& modelMatrices,
							std::vector<std::shared_ptr<Fire>>& fires,
							std::vector<float>& texIds);
void setupObjects(std::shared_ptr<Ship>& ship,
					std::vector<std::shared_ptr<Gem>>& gems,
					std::vector<std::shared_ptr<Fire>>& fires,
					const unsigned maxDiamonds);
