// Original created by Troy Alderson
// Department of Computer Science
// University of Calgary


#include "objFile.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <iostream>


using namespace std;
using namespace glm;


ObjFile::ObjFile (string filename) {
	loadObjFile(filename);
}

void ObjFile::loadObjFile(string filename) {

	cout << "Loading Object: \"" << filename << "\"" << endl;

	ifstream reader(filename);

	if (reader.is_open()) {

		// Reset parameters
		resetParams();

		char c[256];
		string str;
		vec3 v;
		GLuint i1, i2, i3;

		while (!reader.eof()) {
			reader >> str;

			// It's a vertex!
			if (str == "v") {
				reader >> v.x >> v.y >> v.z;
				vertices.push_back( vec4 { v, 1.0f } );

				// Update min and max
				for (int i = 0; i < 3; i++) {
					if (v[i] > max[i]) max[i] = v[i];
					if (v[i] < min[i]) min[i] = v[i];
				}
			}

			// It's a face! (index of vertices)
			if (str == "f") {
				reader >> i1 >> i2 >> i3;
				indices.push_back(i1-1);
				indices.push_back(i2-1);
				indices.push_back(i3-1);
			}
			str = "";
		}
		reader.close();
	}
	else {
		cerr << "File \"" << filename << "\" not found!" << endl;
	}
}


void ObjFile::setupObj(GLuint vertexLocation, GLuint normalLocation) {
	calculateNormals();
	bufferData(vertexLocation, normalLocation);
	xform.scaling = getFitScale();
	xform.translation = getFitTranslate();
}


void ObjFile::resetParams() {

	// Vectors
	vertices.clear();
	normals.clear();
	indices.clear();
	max = { -100, -100, -100 };
	min = { 100, 100, 100 };

	// Transformations
	xform.rotation = mat4(1.0f);
	xform.scaling = mat4(1.0f);
	xform.translation = mat4(1.0f);
	xform.scalar = 1.0f;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////


void ObjFile::draw (GLuint vertexLocation, GLuint normalLocation) {

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vec4) * vertices.size()));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

int ObjFile::numVertices () {
	return vertices.size();
}

vec4 ObjFile::getVertex (int i) {
	if (i < 0 || i >= vertices.size()) {
		cerr << "Vertex Index out of range! (" << i << ")" << endl;
	}
	return vertices[i];
}

vec3 ObjFile::getNormal (int i) {
	if (i < 0 || i >= normals.size()) {
		cerr << "Normal Index out of range! (" << i << ")" << endl;
	}
	return normals[i];
}

mat4 ObjFile::getFitScale () {
	float radius = length (max - min) * 0.5f;
	return scale (mat4 (1.0f), vec3 (0.95f / radius));
}

mat4 ObjFile::getFitTranslate () {
	vec3 center = (max + min) * 0.5f;
	return translate (mat4(1.0f), -center);
}

void ObjFile::calculateNormals () {

	for (int i = 0; i < vertices.size(); i++) {
		normals.push_back({ 0.0f, 0.0f, 0.0f });
	}

	for (int i = 0; i < indices.size(); i += 3) {
		int i1 = indices[i];
		int i2 = indices[i+1];
		int i3 = indices[i+2];

		vec4 p1 = getVertex (i1);
		vec4 p2 = getVertex (i2);
		vec4 p3 = getVertex (i3);
		vec3 fNormal = cross (vec3 (p2 - p1), vec3 (p3 - p1));

		//fNormal = normalize (fNormal);

		normals[i1] = getNormal(i1) + fNormal;
		normals[i2] = getNormal(i2) + fNormal;
		normals[i3] = getNormal(i3) + fNormal;
	}

	for (int i = 0; i < normals.size(); i++) {
		normals[i] = normalize(getNormal(i));
	}
}

void ObjFile::bufferData (GLuint vertexLocation, GLuint normalLocation) {

	if (vertexBuffer == NULL) glGenBuffers(1, &vertexBuffer);
	if (indexBuffer == NULL) glGenBuffers(1, &indexBuffer);
	
	glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vec4) * vertices.size() +  sizeof(vec3) * normals.size(),
		NULL, 
		GL_STATIC_DRAW);

	glBufferSubData (GL_ARRAY_BUFFER,
		0,
		sizeof(vec4) * vertices.size(),
		vertices.data());
	glBufferSubData (GL_ARRAY_BUFFER,
		sizeof(vec4) * vertices.size(),
		sizeof(vec3) * normals.size(),
		normals.data());
  
	glBufferData (GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * indices.size(),
		indices.data(),
		GL_STATIC_DRAW );

	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const GLvoid*)0);
	glEnableVertexAttribArray(vertexLocation);
	glEnableVertexAttribArray (normalLocation);
	glVertexAttribPointer (normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vec4) * vertices.size()));
}



mat4 ObjFile::getTransformation() {
	return xform.rotation * xform.scaling * xform.translation;
}