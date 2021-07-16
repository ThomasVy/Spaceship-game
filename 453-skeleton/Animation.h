#pragma once
#include <glm/glm.hpp>
#include <memory>

class Ship;
class MyCallbacks;

void moveShip(const std::shared_ptr<MyCallbacks>& callbacks, std::shared_ptr<Ship>& ship);
