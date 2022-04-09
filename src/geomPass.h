#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

#include "util/shader.h"
#include "util/objFile.h"
#include "util/ui.h"

// 1st Pass
// Geometry
class GeomPass {
public:

	GeomPass(ObjFile* o);
	void generateBuffer(unsigned int WindowWidth, unsigned int WindowHeight);
	bool shaderGenerated() { return shader->isGenerated(); }

	void render(int width, int height);
	void BindTextures();

	GLuint gBuffer;

private:

	Shader* shader;
	ObjFile* obj;

	GLuint gPosition, gNormal, gAlbedo;	// Textures
	GLuint rboDepth;

	void BindGBuffer();
	void UnBindGBuffer();
};