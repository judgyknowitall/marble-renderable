#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


// Constants to help with location bindings
#define VERTEX_DATA 0
#define VERTEX_NORMAL 1

class Shader {

private:

	// Vertex array object
	GLuint myVAO;

	// Shader program to use
	GLuint myShaderProgram;

	// Shader filenames
	std::string vsFilename = "shaders/phong.vs.glsl";
	std::string fsFilename = "shaders/phong.fs.glsl";

public:

	GLuint getShaderProgram() { return myShaderProgram; }
	GLuint* getVAO() { return &myVAO; }

	bool loadShaderFile(const char* filename, GLuint shader);
	void setupRenderingContext();
};