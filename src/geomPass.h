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

// 1st Pass
// Geometry
class GeomPass {
public:

	GeomPass(ObjFile* o);
	void generateBuffer(unsigned int WindowWidth, unsigned int WindowHeight);
	bool shaderGenerated() { return shader->isGenerated(); }

	void render(int width, int height);
	void BindTextures();

	std::vector<GLuint> texMaps;
	std::vector<std::string> texNames;

private:

	Shader* shader;
	ObjFile* obj;

	GLuint gBuffer;
	GLuint gPosition, gNormal, gAlbedo;	// Textures
	GLuint rboDepth;
	// texture/framebuffer storage
	

	void BindGBuffer();
	void UnBindGBuffer();
};