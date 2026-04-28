#include "ground_mesh.h"

GroundMesh::GroundMesh() {
	float vertices[] = {
		// positions          // texture coords
		-0.5f,  0.0f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.0f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.0f,  0.5f,  1.0f, 1.0f,

		 0.5f,  0.0f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.0f,  0.5f,  0.0f, 1.0f,
		-0.5f,  0.0f, -0.5f,  0.0f, 0.0f
	};

	// create the vertex buffer object, vertex array object, and element buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// copies data (vertices) from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ALWAYS must bind a buffer before calling glBufferData (saying: "ok the VBO is now in focus, that is what we're currently operating on")

	// define how the GPU should read the vertex data (per vertex attribute) and enable the vertex attributes
	// ------------------
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // "turn on the vertex attribute at location 0" (i.e. the position attribute, we defined this in the vertex shader)
	// texture position attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // "same...location 2" (i.e. texture position)

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// same here, we're cleaning up
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glEnable(GL_DEPTH_TEST);
}

void GroundMesh::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GroundMesh::~GroundMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
