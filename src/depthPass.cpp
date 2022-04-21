#include "depthPass.h"

// Shader filenames
#define VS_FILE "shaders/depthPass.vs.glsl"
#define FS_FILE "shaders/depthPass.fs.glsl"

// depth map resolution
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024


using namespace std;
using namespace glm;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Constructor: create shaderprogram
DepthPass::DepthPass(ObjFile* o, State* s) : obj(o), state(s) {

	// Configure Global opengl State
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_RESCALE_NORMAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);


	// Set up draw buffer
	glGenFramebuffers(1, &depthMapFBO);

	// Create Shader
	shader = new Shader(VS_FILE, FS_FILE);
	shader->use();
	shader->setInt("position", 0);

	generateTexture();
}


void DepthPass::generateTexture() {

	// delete existing texture
	if (depthMapTex != NULL) glDeleteTextures(1, &depthMapTex);

	// Generate Depth Map Texture
	glGenTextures(1, &depthMapTex);
	glBindTexture(GL_TEXTURE_2D, depthMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DepthPass::render(unsigned int windowWidth, unsigned int windowHeight) {
	
	BindBuffer();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();
	
	// Proj * View Matrix
	mat4 lightSpace = state->getLightSpaceMatrix();
	shader->setMat4("lightSpaceMatrix", lightSpace);

	// Object Transformation Matrix
	mat4 model = obj->getTransformation();
	shader->setMat4("model", model);

	// Render Scene
	BindTexture();
	obj->draw();

	// Clean up
	UnBindBuffer();
	glViewport(0, 0, windowWidth, windowHeight);
}


void DepthPass::BindBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
}


void DepthPass::UnBindBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthPass::BindTexture() {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}



