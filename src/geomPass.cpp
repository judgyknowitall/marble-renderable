#include "geomPass.h"

// Shader filenames
#define VS_FILE "shaders/phong.vs.glsl"
#define FS_FILE "shaders/phong.fs.glsl"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



GeomPass::GeomPass(ObjFile* o) : obj(o) {
	shader = new Shader(VS_FILE, FS_FILE);

    // Configure Global opengl State
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);

    // Do any necessary initializations (enabling buffers, setting up
    // shaders, geometry etc., before entering the main loop.)
    shader->use();
    shader->setInt("position", VERTEX_DATA);
    shader->setInt("normal", VERTEX_NORMAL);

    // Generate Buffer
    generateBuffer();
}


void GeomPass::generateBuffer() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void GeomPass::render(int width, int height) {

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();

    //Scale based on input
    obj->xform.scaling = scale(mat4(1.0f), vec3(obj->xform.scalar)) * obj->xform.scaling;

    //Create and pass model view matrix
    mat4 modelView = lookAt(vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    modelView *= obj->getTransformation();
    shader->setMat4("mv_matrix", modelView);

    //Create and pass projection matrix
    mat4 proj = perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    shader->setMat4("proj_matrix", proj);

    //Set shader uniforms
    shader->setVec3("light_pos", glm::vec3(1.f, 1.f, -1.f));

    // Note that this version of the draw command uses the
    // bound index buffer to get the vertex coordinates.
    obj->draw(VERTEX_DATA, VERTEX_NORMAL);
}