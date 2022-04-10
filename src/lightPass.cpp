#include "lightPass.h"
#include "state.h"

// Shader filenames
#define VS_FILE "shaders/deferredPass.vs.glsl"
#define FS_FILE "shaders/lightPass.fs.glsl"


using namespace std;
using namespace glm;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Constructor: create shaderprogram
LightPass::LightPass(vector<GLuint>* tMaps, vector<string>* tNames) :
    texMaps(tMaps), texNames(tNames) 
{
    shader = new Shader(VS_FILE, FS_FILE);

    // Configure Global opengl State
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);

    // Do any necessary initializations (enabling buffers, setting up
    // shaders, geometry etc., before entering the main loop.)
    shader->use();
    for (int i = 0; i < texMaps->size(); i++) {
        shader->setInt(texNames->at(i).c_str(), i);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LightPass::render(mat4 light_rotate) {

    glClearColor(0.f, 0.f, 0.f, 1.f); //TODO: bg colour later
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();

    // Light Position
    vec4 L = light_rotate * vec4(0.7071067811865475, 0, 0.7071067811865475, 1);
    float scale = sqrt(L.x * L.x + L.y * L.y + L.z * L.z);
    shader->setVec3("L", vec3(L.x / scale, L.y / scale, L.z / scale));
    
    // View Position
    vec3 view = vec3(VIEW_POS_X, VIEW_POS_Y, VIEW_POS_Z);
    shader->setVec3("V", view);

    // AttachTextures
    AttachTextures();

    // Draw Quad
    renderQuad();
}

void LightPass::AttachTextures() {
    for (int i = 0; i < texMaps->size(); i++) {
        shader->setInt(texNames->at(i).c_str(), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texMaps->at(i));
    }
}


 // Draw Quad
void LightPass::renderQuad() {
    if (quadVAO == 0) {

        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTEX_DATA);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(VERTEX_NORMAL);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

