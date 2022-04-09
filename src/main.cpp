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

#include <iostream>
#include <string>

#include "util/window.h"
#include "geomPass.h"
#include "util/objFile.h"
#include "util/ui.h"



using namespace std;
using namespace glm;






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
    // Window width and height
    int width, height;

    // Setup object
    ObjFile* obj = new ObjFile("models/cube.obj");

    // Set up Graphics
    MyWindow* window = new MyWindow(obj);
    UI ui(window->getWindow());    // imgui

    // Create Passes
    GeomPass geomPass(obj);

    // Finish setting up obj
    if (obj->numVertices() == 0) {
        cout << "Could not load file " << argv[1] << ".\n";
        exit(EXIT_FAILURE);
    }
    obj->setupObj(VERTEX_DATA, VERTEX_NORMAL);
  
    // MAIN LOOP
    if( geomPass.shaderGenerated() ) {
        while (!glfwWindowShouldClose(window->getWindow())){
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
            }
        
            // Render each pass
            geomPass.render(width, height);

            ui.draw();
        
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

