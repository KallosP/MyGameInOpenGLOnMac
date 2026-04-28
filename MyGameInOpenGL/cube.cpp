#include "cube.h"
#include "camera.h" // required for forward declaration

// constructor
Cube::Cube(const char* textureMaterialSrc, const char* textureMaskSrc) {
	cubeMesh = new CubeMesh();
	// why do we need setInt? -> assigns our data we decalred here in this program (e.g. material and mask) to the corresponding variables we declared on the GPU in the fragment shader
	// why do we need to call use() (i.e. glActiveTexture/glBindTexture) for every texture in the render loop? -> because we have to continuously redraw those textures every frame. And the only way to draw a texture is if they are bound 
	// what does it mean to bind a texture? -> means we're telling opengl that's the one we're currently working with
	material = new Material(textureMaterialSrc);
	//mask = nullptr;
	//if (textureMaskSrc) mask = new Material(textureMaskSrc);
}

void Cube::draw(Shader& shaderProgram, Camera& camera, 
	float SCR_WIDTH, float SCR_HEIGHT, glm::vec3* pos, 
	glm::vec3 scale, float yaw) 
{
	material->use(0);
	//if (mask) {
		//mask->use(1); // texture unit 1
		//shaderProgram.setBool("useMask", true);
	//} else {
		//shaderProgram.setBool("useMask", false);
	//}
	// model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, *pos);
	model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));
	model = glm::scale(model, scale);
	shaderProgram.setMat4("model", model);
	// camera/view transformation
	glm::mat4 view;
	// transforms the cube from world space to camera space, so it appears correctly
	// relative to the camera's position/orientation (which is stationary, the objects/world
	// move around the camera)
	view = camera.GetViewMatrix();
	//}
	shaderProgram.setMat4("view", view);
	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
	shaderProgram.setMat4("projection", projection);

	cubeMesh->draw();
}
