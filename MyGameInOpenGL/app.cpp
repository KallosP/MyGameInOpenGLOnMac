#include "app.h"

// settings
// --------------
unsigned int SCR_WIDTH = 1920; 
unsigned int SCR_HEIGHT = 1080;
// background
// ----------
float r = 66;
float g = 167;
float b = 245;
// camera
// -------------
Camera camera{glm::vec3(0.0f, 2.0f, 5.0f)}; // brace initialization, always treated as variable (fixes vexing parse issue)
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float camSpeed = 10.0f;
float camBoost = 40.0f;
// timing
// ------------
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// terrain settings
// ----------------
int Size = 512;
int Iterations = 500;
float MinHeight = 0.0f;
float MaxHeight = 350.0f;
// lower values = more rugged terrain, higher values = smoother terrain (between 0.0 and 1.0)
float Filter = 0.21f;
// r > 1 => smoother terrain, r < 1 => more rugged terrain, r = 1 => balanced terrain
float Roughness = 0.85f;
float WorldScale = 5.0f;
float TextureScale = 50.0f;


std::vector<const char*> terrainTextures = {"rock02_2.jpg", "rock01.jpg", "tilable-IMG_0044-verydark.png", "snow.png"};

enum class TerrainType {
    FaultFormation,
    Midpoint
};


App::App() {
	// Initialize GLFW and GLAD
    initGLFW();
	initImGui();
}

// TODO: 
// - Add a GUI to edit terrain on the fly
// - CLEAN UP AND UNDERSTAND YOUR CODE BEFORE MOVING ON	(rewatch tutorial series if needed)

void App::run() {
	Shader shaderProgram("shader.vs", "shader.fs");
	shaderProgram.use();
	// Assign the uniform variables declared on the GPU (in the GLSL fragment shader code) with
	// the corresponding data we've declared here on the CPU (material & mask)
	shaderProgram.setInt("material", 0); // telling GPU which texture slot to sample from (these are uniforms)
	shaderProgram.setInt("mask", 1); 

	//auto textureMats = setupTerrainTextures(terrainTextures);
	//faultFormTerrain.InitTerrain(WorldScale, TextureScale);
	////BaseTerrain terrain = setUpTerrain(Size, Iterations, MinHeight, MaxHeight, Filter, Roughness);
	//faultFormTerrain.CreateFaultFormation(Size, Iterations, MinHeight, MaxHeight, Filter);
	////terrain.CreateMidpointDisplacement(Size, Roughness, MinHeight, MaxHeight);

	//Material* terrainMat = new Material(createTerrainTexSrc(faultFormTerrain, terrainTextures));

	// FIXME: cube is being rendered with terrain textures and game sometimes freezes/crashes
	//		- potentially a massive memory leak somewhere with how the terrain is being rendered (possibly with Material class)

	// Create cube object
	Cube cube("source.gif");
	// Create a player object as cube
	Player player(cube);

	// Create ground object
	Cube ground("grass.jpg");
	ground.Size = glm::vec3(100.0f, 0.0f, 100.0f);

	Physics physics;

	// Render loop (every iteration is called a frame)
	while (!glfwWindowShouldClose(window)) // checks if GLFW has been instructed to close
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Now check if ImGui wants input
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse && !io.WantCaptureKeyboard) {
			// input
			processInput(&player); // checks for user input (e.g. keyboard input)
		}

		if (playerView) {
			// Update player position (forward vector)
			player.update();
			// Update camera position to follow player
			camera.follow(&player, deltaTime);
		}


		// rendering commands
		glClearColor((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the buffer of the depth info from the previous frame

		// Setup ImGui frame 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// activate the shader program
		//shaderProgram.use(); 
		//terrain.Render(camera, terrainMat, rock02, rock01, grass, snow, (float)SCR_WIDTH, (float)SCR_HEIGHT);
		//if (firstRender) {
		//	terrain.Render(camera, terrainMat, textureMats, (float)SCR_WIDTH, (float)SCR_HEIGHT);
		//	firstRender = false;
		//}

		//faultFormTerrain.Render(camera, *terrainMat, textureMats, (float)SCR_WIDTH, (float)SCR_HEIGHT);

		// TODO: 
		// - implement camera/cube rotation in playerView (need to get a better understanding of code for this)
		// - make the physics more "game like"

		// Update the physics of the player every tick
		physics.update(player, ground, deltaTime);

		player.draw(shaderProgram, camera, (float)SCR_WIDTH, (float)SCR_HEIGHT);
		ground.draw(shaderProgram, camera, (float) SCR_WIDTH, (float) SCR_HEIGHT, &ground.Position, ground.Size, 0.0f);

		// imgui
		if (showImGui) {
			ImGui::Begin("Settings");

			//  Camera
			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Spacing();

				ImGui::Text("Movement");
				ImGui::DragFloat("Speed", &camSpeed, 0.05f, 0.0f, 100.0f, "%.2f");
				ImGui::DragFloat("Boost", &camBoost, 1.0f, 0.0f, 10000.0f);
				ImGui::Checkbox("Player View", &playerView);
				ImGui::DragFloat("Height", &camera.CamHeight, 1.0f, 0.0f, 100.0f, "%.2f");
				ImGui::DragFloat("Distance", &camera.CamDistance, 1.0f, 0.0f, 100.0f, "%.2f");
				ImGui::DragFloat("Player Speed", &camera.Speed, 1.0f, 0.0f, 100.0f, "%.2f");
				ImGui::DragFloat("Turn Speed", &camera.TurnSpeed, 1.0f, 0.0f, 100.0f, "%.2f");
				ImGui::DragFloat("Smoothing", &camera.SmoothSpeed, 1.0f, 0.0f, 100.0f, "%.2f");
				
				ImGui::Spacing();
			}

			// Physics
			if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen)){

				ImGui::Spacing();

				ImGui::Text("Gravity");
				ImGui::DragFloat("Strength", &physics.gravity.y, 10.0f);
				// Amount of bounce when player hits ground
				ImGui::DragFloat("Restitution", &physics.restitution, 0.05f, 0.0f, 1.0f, "%.2f");

				ImGui::Spacing();
			}

			// Objects
			if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Spacing();

				ImGui::Text("Cube");
				ImGui::DragFloat3("Position##Cube", &player.Position.x, 0.1f);
				ImGui::DragFloat3("Size##Cube", &player.Size.x, 0.1f);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Text("Ground");
				ImGui::DragFloat3("Position##Ground", &ground.Position.x, 0.1f);
				ImGui::DragFloat3("Size##Ground", &ground.Size.x, 0.1f, 0.0f, FLT_MAX);

				ImGui::Spacing();
			}

			// Sky
			if (ImGui::CollapsingHeader("Sky", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Spacing();

				// Convert 0–255 → 0–1 for ImGui color widget
				float color[3] = { r / 255.0f, g / 255.0f, b / 255.0f };

				if (ImGui::ColorEdit3("Sky Color", color))
				{
					r = color[0] * 255.0f;
					g = color[1] * 255.0f;
					b = color[2] * 255.0f;
				}

				ImGui::Spacing();
			}

			//ImGui::Text("Terrain");
			//const char* items[] = { "Fault Formation", "Midpoint" };
			//static int selected = 0;
			//if (ImGui::Combo("Terrain Type", &selected, items, IM_ARRAYSIZE(items))) {
			//	// selection changed → rebuild terrain
			//	if (selected == 0) {
			//		//isFaultFormation = true;
			//	}
			//	else {
	
			//	}
			//}
			////ImGui::SliderFloat("WorldScale", &WorldScale, 0.0f, 500.0f);
			//ImGui::SliderInt("Size", &Size, 0, 2048);
			//ImGui::SliderInt("Iterations", &Iterations, 0, 10000);
			//ImGui::SliderFloat("MinHeight", &MinHeight, 0.0f, 500.0f);
			//ImGui::SliderFloat("MaxHeight", &MaxHeight, 0.0f, 500.0f);
			//ImGui::SliderFloat("Filter", &Filter, 0.0f, 0.99f);
			//if (ImGui::IsItemHovered())
			//{
			//	ImGui::SetTooltip("0 = rugged; 1 = smooth");
			//}
			//if (ImGui::Button("Apply")) {
			//	//BaseTerrain terrain = setUpTerrain(Size, Iterations, MinHeight, MaxHeight, Filter, Roughness);
			//	faultFormTerrain.CreateFaultFormation(Size, Iterations, MinHeight, MaxHeight, Filter);
			//	//terrain.CreateMidpointDisplacement(Size, Roughness, MinHeight, MaxHeight);
			//}
			ImGui::End();
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events and swap the buffers
		glfwSwapBuffers(window); // swaps the color buffer with the back buffer (solves issue of flickering and tearing that's present in a single buffer when it's being rendered)
        glfwPollEvents(); // checks if any events are triggered (e.g. keybaord input, mouse movement), calls corresponding functions (which we can register w/ callback functions)
    }
}

App::~App() {
    glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

const char* App::createTerrainTexSrc(FaultFormationTerrain& terrain, std::vector<const char*> terrainTextures) {
	// generate terrain texture
	TextureGenerator TexGen;
	for (int i = 0; i < terrainTextures.size(); ++i) {
		TexGen.LoadTile(terrainTextures[i]);
	}
	int TextureSize = 1024;
	// TODO: save the generated texture to a file and return it in GenerateTexture
	const char* terrainTexSrc = TexGen.GenerateTexture(TextureSize, &terrain, MinHeight, MaxHeight);
	printf("Generated terrain texture: %s\n", terrainTexSrc);
	return terrainTexSrc;
}

std::vector<std::unique_ptr<Material>> App::setupTerrainTextures(std::vector<const char*> textures){ 
    std::vector<std::unique_ptr<Material>> textureMats;
	for (int i = 0; i < textures.size(); ++i) {
		textureMats.push_back(std::make_unique<Material>(textures[i]));
	}
    return textureMats;
}

//BaseTerrain App::setUpTerrain(int Size, int Iterations, float MinHeight, float MaxHeight, float Filter, float Roughness) {
//	
//}
//
void App::initImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 330");
}

// Initialize GLFW and set the OpenGL version to 3.3
void App::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window object and set it as the main context of the current thread
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGLGame", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	//glfwSetWindowPos(window, 560, 240); // set window pos 
	glfwMakeContextCurrent(window);
	// Register the callback function to adjust the viewport when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, esc_key_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, cursorState);
	// store pointer to the App object in the window object
	glfwSetWindowUserPointer(window, this);

	// Initialize GLAD (manages function pointers for OpenGL, simplifying the calling of OpenGL functions to what you would expect)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void App::processInput(Player* player)
{
	if (playerView) {
		// Note: player's forward/backward speed is based on camera's velocity
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			player->Position += player->Forward * camera.getVelocity(deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			player->Position -= player->Forward * camera.getVelocity(deltaTime);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			player->Yaw += camera.TurnSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			player->Yaw -= camera.TurnSpeed;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS and player->isGrounded)
			player->rb.Velocity.y += 2.5f;
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera.MovementSpeed = camBoost;
		else
			camera.MovementSpeed = camSpeed;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void App::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

	if (!app->playerView) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		// Disable camera movement from cursor when cursor is disabled (i.e. has settings open)
		if (app->cursorState == GLFW_CURSOR_DISABLED) {
			camera.ProcessMouseMovement(xoffset, yoffset, true);
		}

	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
//void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}


void App::esc_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        app->showImGui = !app->showImGui;
		if (app->cursorState == GLFW_CURSOR_DISABLED) {
			app->cursorState = GLFW_CURSOR_NORMAL;
		}
		else {
			app->cursorState = GLFW_CURSOR_DISABLED;
		}
		glfwSetInputMode(window, GLFW_CURSOR, app->cursorState);
    }
}
