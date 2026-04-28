#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "config.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "cube.h"
#include "ground.h"
#include "terrain.h"
#include "fault_formation_terrain.h"
#include "midpoint_disp_terrain.h"
#include "texture_generator.h"
#include <cmath>
#include "player.h"
#include "physics.h"

using namespace std;

class App {
	public:
		App();
		~App();
		void run();
	private:

		std::vector<std::unique_ptr<Material>> setupTerrainTextures(std::vector<const char*> textures);
		const char* createTerrainTexSrc(FaultFormationTerrain& terrain, std::vector<const char*> terrainTextures);
		void processInput(Player* player);
		void initGLFW();
		void initImGui();
		//BaseTerrain setUpTerrain(int Size, int Iterations, float MinHeight, float MaxHeight, float Filter, float Roughness);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void esc_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void processInput(GLFWwindow* window);
		GLFWwindow* window;
		bool showImGui = false;
		int cursorState = GLFW_CURSOR_DISABLED;
		FaultFormationTerrain faultFormTerrain;

		bool playerView = false;
};
