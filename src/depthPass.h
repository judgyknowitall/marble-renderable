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


// Depth Map
class DepthPass {
public:

	DepthPass(ObjFile* o, State* s);
	void render(unsigned int windowWidth, unsigned int windowHeight);

	GLuint depthMapTex = NULL;	// depth Texture

private:

	State* state;
	Shader* shader;
	ObjFile* obj;
	GLuint depthMapFBO;	// Frame Buffer

	void generateTexture();
	void BindBuffer();
	void UnBindBuffer();
	void BindTexture();
};