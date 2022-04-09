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
LightPass::LightPass(GLuint* gbuff) : gBuffer(gbuff) {
    shader = new Shader(VS_FILE, FS_FILE);

    // Configure Global opengl State
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);

    // Do any necessary initializations (enabling buffers, setting up
    // shaders, geometry etc., before entering the main loop.)
    shader->use();
    shader->setInt("gPosition", VERTEX_DATA);
    shader->setInt("gNormal", VERTEX_NORMAL);
    shader->setInt("gAlbedoSpec", VERTEX_COLOR);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RENDERING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LightPass::render(mat4 light_rotate, function<void()> bindTextures) {

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
    bindTextures();

    // Draw Quad
    renderQuad();

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