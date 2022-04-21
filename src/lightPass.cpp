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
LightPass::LightPass(vector<GLuint>* tMaps, vector<string>* tNames, GLuint depthTex, State* s) :
    texMaps(tMaps), texNames(tNames), depthMap(depthTex), state(s)
{
    shader = new Shader(VS_FILE, FS_FILE);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LightPass::render() {

    vec4 bg = state->background_colour;
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();

    // Light Position
    shader->setVec3("L", state->getLightPos());
    
    // View Position
    vec3 view = vec3(VIEW_POS_X, VIEW_POS_Y, VIEW_POS_Z);
    shader->setVec3("V", view);

    // Other Uniforms
    shader->setInt("render_mode", state->render_mode);
    shader->setVec4("bg", bg);
    shader->setFloat("sigma_t", state->k_sss);
    shader->setVec3("k_schlick", state->k_schlick);

    // AttachTextures
    AttachTextures();

    // Draw Quad
    renderQuad();
}

void LightPass::AttachTextures() {

    int i = 0;

    // GBuffer
    while (i < texMaps->size()) {
        shader->setInt(texNames->at(i), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texMaps->at(i));
        i++;
    }

    // Depth Map
    shader->setInt("depthMap", i);
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, depthMap);
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

