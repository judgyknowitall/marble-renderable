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


class LightPass {
public:

	LightPass(std::vector<GLuint>* texMaps, std::vector<std::string>* texNames);
	void render(mat4 light_rotate);
	void renderQuad();
	void AttachTextures();

private:

	Shader* shader;
	std::vector<GLuint>* texMaps; 
	std::vector<std::string>* texNames;
	GLuint quadVBO, quadVAO = 0;
};