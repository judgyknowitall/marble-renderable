// Original created by Troy Alderson
// Department of Computer Science
// University of Calgary


#include "objFile.h"
#include "state.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <iostream>

#define MARBLE_TEX_WIDTH 10
#define MARBLE_TEX_HEIGHT 10


using namespace std;
using namespace glm;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// SET UP
///////////////////////////////////////////////////////////////////////////////////////////////////////

ObjFile::ObjFile (string filename) {
	loadObjFile(filename);
}


void ObjFile::setupObj() {
	calculateNormals();
	calculateColors();
	bufferData();
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
// Caculate vertices and normals
///////////////////////////////////////////////////////////////////////////////////////////////////////

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


void ObjFile::calculateNormals() {

	for (int i = 0; i < vertices.size(); i++) {
		normals.push_back({ 0.0f, 0.0f, 0.0f });
	}

	for (int i = 0; i < indices.size(); i += 3) {
		int i1 = indices[i];
		int i2 = indices[i + 1];
		int i3 = indices[i + 2];

		vec4 p1 = getVertex(i1);
		vec4 p2 = getVertex(i2);
		vec4 p3 = getVertex(i3);
		vec3 fNormal = cross(vec3(p2 - p1), vec3(p3 - p1));

		normals[i1] = getNormal(i1) + fNormal;
		normals[i2] = getNormal(i2) + fNormal;
		normals[i3] = getNormal(i3) + fNormal;
	}

	for (int i = 0; i < normals.size(); i++) {
		normals[i] = normalize(getNormal(i));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate Marble Texture
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define noiseWidth 320
#define noiseHeight 240
float noise[noiseHeight][noiseWidth];

float smoothNoise(float x, float y)
{
	//get fractional part of x and y
	float fractX = x - int(x);
	float fractY = y - int(y);

	//wrap around
	int x1 = (int(x) + noiseWidth) % noiseWidth;
	int y1 = (int(y) + noiseHeight) % noiseHeight;

	//neighbor values
	int x2 = (x1 + noiseWidth - 1) % noiseWidth;
	int y2 = (y1 + noiseHeight - 1) % noiseHeight;

	//smooth the noise with bilinear interpolation
	float value = 0.0;
	value += fractX * fractY * noise[y1][x1];
	value += (1 - fractX) * fractY * noise[y1][x2];
	value += fractX * (1 - fractY) * noise[y2][x1];
	value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

	return value;
}


float turbulence(float x, float y, float size)
{
	float value = 0.0, initialSize = size;

	while (size >= 1)
	{
		value += smoothNoise(x / size, y / size) * size;
		size /= 2.0;
	}

	return(128.0 * value / initialSize);
}


float MarbleTexture(float x, float y) {
	//xPeriod and yPeriod together define the angle of the lines
	//xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern
	float xPeriod = 5.0; //defines repetition of marble lines in x direction
	float yPeriod = 10.0; //defines repetition of marble lines in y direction
	//turbPower = 0 ==> it becomes a normal sine pattern
	float turbPower = 5.0; //makes twists
	float turbSize = 32.0; //initial size of the turbulence

	//float xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight + turbPower * turbulence(x, y, turbSize) / 256.0;
	float xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight;
	float c = abs(sin(xyValue * 3.14159));

	return c;
}



void ObjFile::calculateColors() {

	marbleNoise.clear();
	textureCoords.clear();
	
	// Create Texture
	for (float s = 0; s < MARBLE_TEX_WIDTH; s++) {
		for (float t = 0; t < MARBLE_TEX_HEIGHT; t++) {
			//float c = MarbleTexture(s,t);	// Basic sine function
			//cout << c << endl;
			//marbleNoise.push_back(vec3(c,c,c));
			marbleNoise.push_back(vec3(s / MARBLE_TEX_WIDTH, .5f, 0.f));
			cout << s / MARBLE_TEX_WIDTH << endl;
		}
	}

	// Texture Mapping
	for (auto vertex : vertices) {
		textureCoords.push_back(vec2(vertex.x, vertex.z));
		//cout << vertex.x << ',' << vertex.y << ',' << vertex.z << endl;
	}

	cout << vertices.size() << ',' << marbleNoise.size() << endl;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
///////////////////////////////////////////////////////////////////////////////////////////////////////


void ObjFile::bufferData() {

	if (VAO == NULL) glGenVertexArrays(1, &VAO);
	if (vertexBuffer == NULL) glGenBuffers(1, &vertexBuffer);
	if (indexBuffer == NULL) glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	auto vSize = sizeof(vec4) * vertices.size();
	auto nSize = sizeof(vec3) * normals.size();
	auto tSize = sizeof(vec2) * textureCoords.size();

	glBufferData(GL_ARRAY_BUFFER,
		vSize + nSize + tSize,
		NULL,
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		vSize,
		vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER,
		vSize,
		nSize,
		normals.data());
	glBufferSubData(GL_ARRAY_BUFFER,
		vSize + nSize,
		tSize,
		textureCoords.data());

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * indices.size(),
		indices.data(),
		GL_STATIC_DRAW);

	interpretData();

	// Generate Texture
	if (MarbleTexture != NULL) glDeleteTextures(1, &marbleTexture);
	glGenTextures(1, &marbleTexture);
	glBindTexture(GL_TEXTURE_2D, marbleTexture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, MARBLE_TEX_WIDTH, MARBLE_TEX_HEIGHT, 0, GL_RGB, GL_FLOAT, &marbleNoise[0]);
}


void ObjFile::draw () {
	// Bind Texture
	glBindTexture(GL_TEXTURE_2D, marbleTexture);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	interpretData();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void ObjFile::interpretData() {
	auto vSize = sizeof(vec4) * vertices.size();
	auto nSize = sizeof(vec3) * normals.size();

	glEnableVertexAttribArray(VERTEX_DATA);
	glVertexAttribPointer(VERTEX_DATA, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(VERTEX_NORMAL);
	glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(vSize));
	glEnableVertexAttribArray(VERTEX_COLOR);
	glVertexAttribPointer(VERTEX_COLOR, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(vSize + nSize));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// GETTERS / SETTERS
///////////////////////////////////////////////////////////////////////////////////////////////////////


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

mat4 ObjFile::getTransformation() {
	return xform.rotation * xform.scaling * xform.translation;
}