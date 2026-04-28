#pragma once
#include "config.h"
#include "player.h"

class Physics {
public:
    void update(Player& player, Entity& ground, float dt);
	glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	// bounce
	float restitution = 0.5f;

private:
    void applyGravity(Player& player, float dt);
    void resolveGroundCollision(Player& player, Entity& ground);
};
