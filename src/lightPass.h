#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "util/shader.h"
#include "util/objFile.h"
#include "util/ui.h"
#include "state.h"


// Shading
class LightPass {
public:

	LightPass(std::vector<GLuint>* texMaps, std::vector<std::string>* texNames, GLuint depthTex, State* state);
	void render();
	void renderQuad();
	void AttachTextures();

private:

	State* state;
	Shader* shader;
	GLuint depthMap;
	std::vector<GLuint>* texMaps; 
	std::vector<std::string>* texNames;
	GLuint quadVBO = 0, quadVAO = 0;
};