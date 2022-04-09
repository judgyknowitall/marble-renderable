#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


class Shader {

private:

	GLuint myVAO;				// Vertex array object
	GLuint myShaderProgram;		// Shader program to use

	bool loadShaderFile(const char* filename, GLuint shader);
	bool checkShaderCompilation(GLuint shader, std::string type);

public:
	Shader(std::string vsFilename, std::string fsFilename);

	void use();
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setInt(const std::string& name, int value) const;

	bool isGenerated() { return true; }
};