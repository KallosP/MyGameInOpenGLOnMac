#include "player.h"
#include "camera.h"
#include "shader.h"

// TODO: replace cube with real player/ATV model later
Player::Player(Cube& cubeModel) {
	this->cubeModel = &cubeModel;
}

void Player::update() {
	// Calculate/update forward vector of player
	Forward.x = cos(glm::radians(Yaw));
	Forward.y = 0.0f;
	Forward.z = -sin(glm::radians(Yaw)); // -Z is forward in Right handed coord system
	Forward = glm::normalize(Forward);
}

void Player::draw(Shader & shaderProgram, Camera & camera, float SCR_WIDTH, float SCR_HEIGHT)	
{
	cubeModel->draw(shaderProgram, camera, SCR_WIDTH,SCR_HEIGHT, &Position, Size, Yaw);
}
