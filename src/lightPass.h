#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <functional>

#include "util/shader.h"
#include "util/objFile.h"
#include "util/ui.h"


class LightPass {
public:

	LightPass(GLuint* gBuffer);
	void render(mat4 light_rotate, function<void()> bindTextures);
	void renderQuad();

private:

	Shader* shader;
	GLuint* gBuffer;
	GLuint quadVBO, quadVAO = 0;
};