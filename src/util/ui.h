#pragma once

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <array>

#include "state.h"


class UI {
private: 

	State* state;

public:

	bool showPanel = true;
	bool loadObj = false;

	// obj file
	std::string objFile;


	// Called after window is set up
	UI(GLFWwindow* window, State* s) : state(s) {
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
		if (showPanel && Begin("Panel", &showPanel, ImGuiWindowFlags_MenuBar)) {

			if (BeginMenuBar()) {
				if (BeginMenu("File")) {
					if (MenuItem("Close", "(P)")) {
						showPanel = false;
					}
					ImGui::EndMenu();
				}
				EndMenuBar();
			}

			Spacing();

			// Load Object
			if (CollapsingHeader("OBJ Loader")) {
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
			}

			// Rendering Options
			std::vector<const char*> options = {
				"Phong", "OrenNayar", "Albedo", "Specular", "Depth Map",
				"Position", "Normal", "Depth", "Screen Space"};
			static const char* current_item = "Phong";
			if (BeginCombo("Render Type", current_item)) // The second parameter is the label previewed before opening the combo.
			{
				for (int i = 0; i < options.size(); i++)
				{
					bool is_selected = (current_item == options[i]); // You can store your selection however you want, outside or inside your objects
					if (Selectable(options[i], is_selected)) {
						current_item = options[i];
						state->render_mode = i;
						std::cout << "Render Mode: " << options[i] << std::endl;
					}
					if (is_selected) {
						SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}
				}
				EndCombo();
			}

			// Material Options
			// Marble Texture
			

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