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


// Geometry
class GeomPass {
public:

	GeomPass(ObjFile* o, State* s);
	void generateTextures(unsigned int WindowWidth, unsigned int WindowHeight);
	bool shaderGenerated() { return shader->isGenerated(); }
	void render(int width, int height);
	

	// texture/framebuffer storage
	std::vector<GLuint> texMaps;
	std::vector<std::string> texNames;

private:

	Shader* shader;
	State* state;
	ObjFile* obj;

	GLuint gBuffer;
	
	void BindGBuffer();
	void UnBindGBuffer();
	void BindTextures();
};