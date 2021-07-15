#pragma once
#include <glm/glm.hpp>
#include <memory>

class GameObject;
class MyCallbacks;

void setShipMovement(const std::shared_ptr<MyCallbacks>& callbacks, std::shared_ptr<GameObject>& ship);
