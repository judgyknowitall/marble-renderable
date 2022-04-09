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


MyWindow::MyWindow(ObjFile* o, State* s) : obj(o), state(s) {

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
    
    // Set up Key Callbacks
    glfwSetKeyCallback(mWindow, onKey);

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
    switch (key) {

        // Exit
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(mWindow, GL_TRUE);
            break;

        // Rotation
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(1.0f, 0.0f, 0.0f)) * obj->xform.rotation;
            break;

        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(-1.0f, 0.0f, 0.0f)) * obj->xform.rotation;
            break;

        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, 1.0f, 0.0f)) * obj->xform.rotation;
            break;

        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, -1.0f, 0.0f)) * obj->xform.rotation;
            break;

        // Scale
        case GLFW_KEY_Z:
            if (action == GLFW_RELEASE) obj->xform.scalar = 1.0f; 
            else obj->xform.scalar = 1.01f;
            break;

        case GLFW_KEY_X:
            if (action == GLFW_RELEASE) obj->xform.scalar = 1.0f;
            else obj->xform.scalar = 1.0f / 1.01f;
            break;

        // move around the light
        case GLFW_KEY_KP_8:
            state->rotate_light(vec3(-1.0f, 0.0f, 0.0f), 0.1f);
            break;

        case GLFW_KEY_KP_2:
            state->rotate_light(vec3(1.0f, 0.0f, 0.0f), 0.1f);
            break;

        case GLFW_KEY_KP_4:
            state->rotate_light(vec3(0.0f, -1.0f, 0.0f), 0.1f);
            break;

        case GLFW_KEY_KP_6:
            state->rotate_light(vec3(0.0f, 1.0f, 0.0f), 0.1f);
            break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Window Main Loop Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyWindow::handleResizing(function<void(int, int)> func) {
    int newWidth, newHeight;
    glfwGetFramebufferSize(mWindow, &newWidth, &newHeight);
    if ((newWidth != width) || (newHeight != height)) {
        func(newWidth, newHeight);
        width = newWidth;
        height = newHeight;
    }
    glViewport(0, 0, width, height);
}