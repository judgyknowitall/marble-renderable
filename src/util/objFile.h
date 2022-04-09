// CPSC 453: Introduction to Computer Graphics
//
// Header file for a class to handle obj file data
//
// Created by Troy Alderson
// Department of Computer Science
// University of Calgary
//
// Used with permission for CPSC 453
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;


// Transformation matrices
struct Transformation {
	mat4 rotation = mat4(1.0f);
	mat4 scaling = mat4(1.0f);
	mat4 translation = mat4(1.0f);
	float scalar = 1.0f;
};



class ObjFile {
	public:
		Transformation xform;

		ObjFile (string filename);
		void loadObjFile(string filename);
		void setupObj();
		void resetParams();

		void draw ();

		int numVertices ();
		vec4 getVertex (int i);
		vec3 getNormal (int i);

		mat4 getTransformation();
		

	private:
		GLuint vertexBuffer = NULL, indexBuffer = NULL;
		GLuint VAO;
		vector<vec4> vertices;
		vector<vec3> normals;
		vector<GLuint> indices;

		vec3 max, min;

		void calculateNormals();
		void bufferData();

		mat4 getFitScale();
		mat4 getFitTranslate();
};

