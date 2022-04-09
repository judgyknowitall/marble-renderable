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


// Constants to help with location bindings
#define VERTEX_DATA 0
#define VERTEX_NORMAL 1



class GeomPass {
public:
	GeomPass(ObjFile* o);
	void render(int width, int height);
	bool shaderGenerated() { return shader->isGenerated(); }

private:
	Shader* shader;
	ObjFile* obj;
	GLuint VAO;

	void generateBuffer();
};