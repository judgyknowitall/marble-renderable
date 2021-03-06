#include "geomPass.h"

#include "state.h"

// Shader filenames
#define VS_FILE "shaders/geomPass.vs.glsl"
#define FS_FILE "shaders/geomPass.fs.glsl"



using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Constructor: create shaderprogram
GeomPass::GeomPass(ObjFile* o, State* s) : obj(o), state(s) {
	
    // set up the draw buffers
    glGenFramebuffers(1, &gBuffer);

    // Create Shader
    shader = new Shader(VS_FILE, FS_FILE);
    shader->use();
    shader->setInt("position", 0);
    shader->setInt("normal", 1);
    shader->setInt("texCoord", 2);
}


// Create gBuffer
void GeomPass::generateTextures(unsigned int WindowWidth, unsigned int WindowHeight) {

    // delete any existing textures
    glDeleteTextures(texMaps.size(), texMaps.data());

    // reserve some new ones
    texNames.clear();
    texMaps.resize(5);
    glGenTextures(5, texMaps.data());

    // create each of the textures
    // 0: depth -> depth, 32F
    texNames.push_back("gDepth");
    glBindTexture(GL_TEXTURE_2D, texMaps[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	// disable filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 1: RGB -> albedo, A -> roughness, 16F
    texNames.push_back("gColor");
    glBindTexture(GL_TEXTURE_2D, texMaps[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 2: RGB -> normal (XYZ), 32F
    texNames.push_back("gNormal");
    glBindTexture(GL_TEXTURE_2D, texMaps[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 3: RGB -> view coords, 32F
    texNames.push_back("gPosition");
    glBindTexture(GL_TEXTURE_2D, texMaps[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 4: RGB -> LightSpace coords, 32F
    texNames.push_back("gLightSpacePos");
    glBindTexture(GL_TEXTURE_2D, texMaps[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void GeomPass::render(int width, int height) {

    // Dump varialbes to texture buffer
    BindGBuffer();

    // Clear the window and the depth buffer
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();

    // Object Transformation matrix
    mat4 model = obj->getTransformation();
    shader->setMat4("model", model);

    // Create and pass view matrix
    mat4 view = lookAt(vec3(VIEW_POS_X, VIEW_POS_Y, VIEW_POS_Z), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    shader->setMat4("view", view);
   
    // Create and pass projection matrix
    mat4 proj = perspective(45.0f, (float)width / (float)height, 0.1f, 100.f);
    shader->setMat4("projection", proj);

    // LightSpace Matrix (proj * view)
    mat4 lightSpace = state->getLightSpaceMatrix();
    shader->setMat4("lightSpace", lightSpace);

    // Marble Colours
    shader->setVec3("marble_base_clr", state->marble_base_colour);

    // Marble Texture
    shader->setInt("marbleTexture", 0);
    glActiveTexture(GL_TEXTURE0);

    // Note that this version of the draw command uses the
    // bound index buffer to get the vertex coordinates.
    BindTextures();
    obj->draw();

    UnBindGBuffer();
}

void GeomPass::BindGBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
}

void GeomPass::UnBindGBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


// Bind to gBuffer
void GeomPass::BindTextures() {

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texMaps[0], 0);

    vector<GLenum> attach;
    for (int i = 1; i < texMaps.size(); i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texMaps[i], 0);
        attach.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(attach.size(), attach.data());
}

