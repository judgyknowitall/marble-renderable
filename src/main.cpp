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
#include "util/objFile.h"
#include "util/ui.h"
#include "state.h"
#include "depthPass.h"
#include "geomPass.h"
#include "lightPass.h"



using namespace std;
using namespace glm;



// MAIN
int main(int argc, char **argv)
{
    // Setup object
    ObjFile* obj = new ObjFile("models/dragon.obj");
    State* state = new State();

    // Set up Graphics
    MyWindow* window = new MyWindow(obj, state);
    UI ui(window->getWindow(), state);    // imgui
    
    // Create Rendering Passes
    cout << "Setting up Each Pass..." << endl;
    DepthPass depthPass(obj, state);
    GeomPass geomPass(obj, state);
    LightPass lightPass(&geomPass.texMaps, &geomPass.texNames, depthPass.depthMapTex, state);

    // Finish setting up obj
    if (obj->numVertices() == 0) {
        cout << "Could not load file " << argv[1] << ".\n";
        exit(EXIT_FAILURE);
    }
    obj->setupObj();
  
    // MAIN LOOP
    cout << "Running Render Loop..." << endl;
    if( geomPass.shaderGenerated() ) {
        while (!glfwWindowShouldClose(window->getWindow())){

            // Window Resizing
            window->handleResizing([&](int w, int h) -> void {
                geomPass.generateTextures(w, h);
            });

            // Load new object
            if (ui.loadObj) {
                obj->loadObjFile(ui.objFile);
                if (obj->numVertices() == 0) {
                    cout << "Could not load file " << argv[1] << ".\n";
                    exit(EXIT_FAILURE);
                }
                obj->setupObj();
            }

            // Clear frame
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
            // Render each pass
            depthPass.render(window->width, window->height);
            geomPass.render(window->width, window->height);
            lightPass.render();

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

