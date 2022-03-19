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

#include "callbacks.h"
#include "shader.h"
#include "objFile.h"
#include "ui.h"

using namespace std;
using namespace glm;




// ObjFile instance
ObjFile *obj;






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// Called to draw scene
int width, height;
void renderScene(GLuint myShaderProgram) {
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

/*
GLFWwindow* setupWindow() {
   
    MyCallBacks* callbacks = new MyCallBacks(obj);

    glfwSetErrorCallback(callbacks->error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Anti-aliasing - This will have smooth polygon edges
    glfwWindowHint(GLFW_SAMPLES, 4);


    GLFWwindow* window = glfwCreateWindow(800, 800, "OBJ Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, callbacks->key_callback);
    glfwSetMouseButtonCallback(window, callbacks->mouse_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        //Problem: glewInit failed, something is seriously wrong.
        printf("glewInit failed, aborting.\n");
        exit(EXIT_FAILURE);
    }

    // These two lines will print out the version of OpenGL and GLSL
    // that are being used so that problems due to version differences
    // can easily be identified.
    printf("OpenGL version: %s\n", (const char*)glGetString(GL_VERSION));
    printf("GLSL version: %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    return window;
}
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
  //GLFWwindow* window = setupWindow();
  //Shader* shader = new Shader();
  
  //UI ui(window);
  
  // Do any necessary initializations (enabling buffers, setting up
  // shaders, geometry etc., before entering the main loop.)
  // This is done by calling the function setupRenderingContext().
  
  //shader->setupRenderingContext();

  // Set up object
  obj = new ObjFile("models/cube.obj");
  return 0;

  /*
  if (obj->numVertices() == 0) {
      cout << "Could not load file " << argv[1] << ".\n";
      exit(EXIT_FAILURE);
  }
  obj->setupObj(VERTEX_DATA);
  
  // MAIN LOOP
  if( shader->getShaderProgram() ) {
    while (!glfwWindowShouldClose(window))
      {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Load new object
        if (ui.loadObj) {

            obj->loadObjFile(ui.objFile);
            if (obj->numVertices() == 0) {
                cout << "Could not load file " << argv[1] << ".\n";
                exit(EXIT_FAILURE);
            }
            obj->setupObj(VERTEX_DATA);

            ui.loadObj = false;
        }
        
        // Render objects to the scene
        renderScene(shader->getShaderProgram());
        ui.draw();
        
        // Note that buffer swapping and polling for events is done here
        // so please don't do it in the function used to draw the scene.
        glfwSwapBuffers(window);
        glfwPollEvents();
      }
  }

  // Clean up
  delete obj;
  ui.cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
  */
}

