#pragma once
#include "config.h"
#include "entity.h"
#include "cube.h"
#include "rigidbody.h"

class Camera;
class Shader;

class Player : public Entity {
	public:
		// TODO: replace cube with real player/ATV model later, cube is temporary
		Player(Cube& cubeModel);
		void update();
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);

		Rigidbody rb;

		bool isGrounded = true;

		float Yaw = 0.0f;
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	private:
		Cube* cubeModel;
};