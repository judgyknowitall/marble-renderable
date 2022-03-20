// CPSC 453: Introduction to Computer Graphics
//
// Example program that demonstrates VBOs and shader usage with GLFW.
// The use of an index buffer is also demonstrated.
//
// Usman Alim
// Department of Computer Science
// University of Calgary
//
// Modified by Troy Alderson


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "window.h"
#include "shader.h"
#include "objFile.h"
#include "ui.h"

using namespace std;
using namespace glm;


// Window width and height
int width, height;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Called to draw scene
void renderScene(GLuint myShaderProgram, ObjFile* obj) {
    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram( myShaderProgram );

    //Scale based on input
    obj->xform.scaling = scale (mat4(1.0f), vec3 (obj->xform.scalar)) * obj->xform.scaling;

    //Create and pass model view matrix
    mat4 modelView = lookAt (vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    modelView *= obj->getTransformation();
    glUniformMatrix4fv (glGetUniformLocation(myShaderProgram, "mv_matrix"), 1, GL_FALSE, value_ptr (modelView));

    //Create and pass projection matrix
    mat4 proj = perspective (45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glUniformMatrix4fv (glGetUniformLocation(myShaderProgram, "proj_matrix"), 1, GL_FALSE, value_ptr (proj));

    //Set shader uniforms
    glUniform3f (glGetUniformLocation(myShaderProgram, "light_pos"), 1.0f, 1.0f, -1.0f);
  
  
    // Note that this version of the draw command uses the
    // bound index buffer to get the vertex coordinates.
    obj->draw(VERTEX_DATA, VERTEX_NORMAL);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char **argv)
{
    // Setup object
    ObjFile* obj = new ObjFile("models/cube.obj");

    // Set up Graphics
    MyWindow* window = new MyWindow(obj);
    Shader* shader = new Shader();
    UI ui(window->getWindow());    // imgui
  
    // Do any necessary initializations (enabling buffers, setting up
    // shaders, geometry etc., before entering the main loop.)
    shader->setupRenderingContext();

    // Finish setting up obj
    if (obj->numVertices() == 0) {
        cout << "Could not load file " << argv[1] << ".\n";
        exit(EXIT_FAILURE);
    }
    obj->setupObj(VERTEX_DATA, VERTEX_NORMAL);
  
    // MAIN LOOP
    if( shader->getShaderProgram() ) {
    while (!glfwWindowShouldClose(window->getWindow()))
        {
        glfwGetFramebufferSize(window->getWindow(), &width, &height);
        glViewport(0, 0, width, height);

        // Load new object
        if (ui.loadObj) {

            obj->loadObjFile(ui.objFile);
            if (obj->numVertices() == 0) {
                cout << "Could not load file " << argv[1] << ".\n";
                exit(EXIT_FAILURE);
            }
            obj->setupObj(VERTEX_DATA, VERTEX_NORMAL);

            ui.loadObj = false;
        }
        
        // Render objects to the scene
        renderScene(shader->getShaderProgram(), obj);
        //ui.draw();
        
        // Note that buffer swapping and polling for events is done here
        // so please don't do it in the function used to draw the scene.
        glfwSwapBuffers(window->getWindow());
        glfwPollEvents();
        }
    }

    // Clean up
    ui.cleanUp();
    delete obj;
    delete window;
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

