#pragma once

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <array>

class UI {

public:

	bool loadObj = false;

	// obj file
	std::string objFile;


	// Called after window is set up
	UI(GLFWwindow* window) {
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	// Called before shutting down graphics
	void cleanUp() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// Called every frame
	void draw() {

		using namespace ImGui;

		// New Frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();

		// Create panel
		if (Begin("OBJ Loader")) {

			static std::array<char, 64> buffer;

			InputText("(OBJ) file", buffer.data(), buffer.size());
			loadObj = Button("Load");
			if (loadObj) {
				objFile = buffer.data();
			}
			SameLine();
			if (Button("Clear")) {
				buffer.fill(0);
			}

			// Display FPS
			Spacing();
			Separator();
			Text("%.3f ms/frame (%.1f FPS)", 1000.f / GetIO().Framerate, GetIO().Framerate);

			End();
		}


		// Render Panel
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};