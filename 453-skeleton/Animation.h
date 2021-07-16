#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
class Ship;
class MyCallbacks;
class GameObject;
class Gem;
class Fire;

void moveShip(const std::shared_ptr<MyCallbacks>& callbacks, std::shared_ptr<Ship>& ship);

void rotateClockwise(GameObject* object);

void update(std::shared_ptr<Ship>& ship,
	std::vector<std::shared_ptr<Gem>>& gems,
	std::vector<std::shared_ptr<Fire>>& fires,
	bool gameOver);
