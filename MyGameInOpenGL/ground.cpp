#include "ground.h"

// constructor
Ground::Ground(const char* textureMaterialSrc, const char* textureMaskSrc) {
	groundMesh = new GroundMesh();
	material = new Material(textureMaterialSrc);
	//mask = nullptr;
	//if (textureMaskSrc) mask = new Material(textureMaskSrc);
}

void Ground::draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
	material->use(0);
	//if (mask) {
		//mask->use(1); // texture unit 1
		//shaderProgram.setBool("useMask", true);
	//} else {
		//shaderProgram.setBool("useMask", false);
	//}
	// model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
	model = glm::scale(model, glm::vec3(50.0f, 0.0f, 50.0f));
	shaderProgram.setMat4("model", model);
	// camera/view transformation
	glm::mat4 view = camera.GetViewMatrix();
	shaderProgram.setMat4("view", view);
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	shaderProgram.setMat4("projection", projection);

	groundMesh->draw();
}