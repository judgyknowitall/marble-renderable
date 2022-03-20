#include "window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800



using namespace std;
using namespace glm;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MyWindow::MyWindow(ObjFile* o) : obj(o) {

    glfwSetErrorCallback(onError);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Anti-aliasing - This will have smooth polygon edges
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create Window
    mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Marble Rendering", NULL, NULL);
    glfwSetWindowUserPointer(mWindow, this);
    if (!mWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mWindow);
    
    // Callbacks
    glfwSetKeyCallback(mWindow, onKey);
    glfwSetMouseButtonCallback(mWindow, onMouse);

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
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Keyboard callback function.
void MyWindow::key_callback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, GL_TRUE);

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(1.0f, 0.0f, 0.0f)) * obj->xform.rotation;
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(-1.0f, 0.0f, 0.0f)) * obj->xform.rotation;

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, 1.0f, 0.0f)) * obj->xform.rotation;
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, -1.0f, 0.0f)) * obj->xform.rotation;
}


// Mouse Callback Function
void MyWindow::mouse_callback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        obj->xform.scalar = 1.01f;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        obj->xform.scalar = 1.0f;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        obj->xform.scalar = 1.0f / 1.01f;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        obj->xform.scalar = 1.0f;
}


