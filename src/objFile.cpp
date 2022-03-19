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
	
	/*
	vertices = {
	 vec4(0.5f,  0.5f, 0.0f, 1.0f),  // top right
	 vec4(0.5f, -0.5f, 0.0f, 1.0f),  // bottom right
	vec4(-0.5f, -0.5f, 0.0f, 1.0f),  // bottom left
	vec4(-0.5f,  0.5f, 0.0f, 1.0f)   // top left 
	};
	indices = {  // note that we start from 0!
	ivec3(0, 1, 3),   // first triangle
	ivec3(1, 2, 3)    //second triangle
	};*/
	//TODO
}

void ObjFile::loadObjFile(string filename) {

	ifstream reader(filename);

	if (reader.is_open()) {

		// Clear parameters
		vertices.clear();
		normals.clear();
		indices.clear();
		max = {-100, -100, -100};
		min = { 100, 100, 100 };

		char c[256];
		string str;
		vec3 v;
		ivec3 i;

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
				reader >> i.x >> i.y >> i.z;
				indices.push_back(i - 1);
			}
			str = "";
		}


		//TODO
		for (int i = 0; i < vertices.size(); i++) {
			cout << vertices[i].x << ' ' << vertices[i].y << ' ' << vertices[i].z << endl;
		}
		cout << "" << endl;
		for (int i = 0; i < indices.size(); i++) {
			cout << indices[i].x << ' ' << indices[i].y << ' ' << indices[i].z << endl;
		}
		cout << "" << endl;
		cout << max.x << ' ' << max.y << ' ' << max.z << endl;
		cout << min.x << ' ' << min.y << ' ' << min.z << endl;

		reader.close();
	}
}


void ObjFile::setupObj(GLuint vertexLocation) {
	//calculateNormals();
	//bufferData(vertexLocation);
	xform.scaling = getFitScale();
	xform.translation = getFitTranslate();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////


void ObjFile::draw (GLuint vertexLocation, GLuint normalLocation) {
	glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  

	//glDrawElements (GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); //DELETE
	//glBindVertexArray(vertexBuffer);
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

int ObjFile::numVertices () {
	return vertices.size();
}

vec4 ObjFile::getVertex (int i) {
	return vertices[i];
}

vec3 ObjFile::getNormal (int i) {
	return normals[i];
}

void ObjFile::setVertex (int i, vec4 v) {
	vertices[i] = v;
}

void ObjFile::setNormal (int i, vec3 v) {
	normals[i] = v;
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
	/* TODO
	for (int i = 0; i < vertices.size(); i += 4) {
		normals.push_back (0.0f);
		normals.push_back (0.0f);
		normals.push_back (0.0f);
	}

	for (int i = 0; i < indices.size(); i += 3) {
		int i1 = indices[i];
		int i2 = indices[i+1];
		int i3 = indices[i+2];

		vec4 p1 = getVertex (i1);
		vec4 p2 = getVertex (i2);
		vec4 p3 = getVertex (i3);
		vec3 fNormal = cross (vec3 (p2 - p1), vec3 (p3 - p1));

		float l = length (fNormal);
		//if (fNormal.x != 0.0f && fNormal.y != 0.0f && fNormal.z != 0.0f) {
			fNormal = normalize (fNormal);

			//std::cout << "fNORMAL: " << std::endl;
			std::cout << fNormal.x << ',' << fNormal.y << ',' << fNormal.z << std::endl; //TODO
			*/

			/*setNormal(i1, getNormal(i1) + fNormal);
			setNormal (i2, getNormal(i2) + fNormal);
			setNormal (i3, getNormal(i3) + fNormal);*/

			//setNormal(i1, fNormal);
			//setNormal(i2, fNormal);
			//setNormal(i3, fNormal);
		//}
	//}

	/*for (int i = 0; i < normals.size(); i += 3) {
		auto v = normalize(getNormal(i / 3));
		setNormal (i/3, v);
		std::cout << v.x << ',' << v.y << ',' << v.z << std::endl; //TODO
	}*/
}

void ObjFile::bufferData (GLuint vertexLocation, GLuint normalLocation) {
	/*
	glGenBuffers (1, &vertexBuffer);
	glGenBuffers (1, &indexBuffer); 
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
		sizeof(ivec3) * indices.size(),
		indices.data(),
		GL_STATIC_DRAW );

	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const GLvoid*)0);
	glEnableVertexAttribArray(vertexLocation);
	glEnableVertexAttribArray (normalLocation);
	glVertexAttribPointer (normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vec4) * vertices.size()));
	*/ //TODO
}



mat4 ObjFile::getTransformation() {
	return xform.rotation * xform.scaling * xform.translation;
}