#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "objFile.h"


using namespace std;
using namespace glm;

extern ObjFile* obj;

class MyCallBacks {

private:
    static ObjFile* obj;

public:

    MyCallBacks(ObjFile* o) {
       obj = o;
    }

    /**
     * Keyboard callback function.
     */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
            obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(1.0f, 0.0f, 0.0f)) * obj->xform.rotation;
        if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
            obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(-1.0f, 0.0f, 0.0f)) * obj->xform.rotation;

        if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, 1.0f, 0.0f)) * obj->xform.rotation;
        if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
            obj->xform.rotation = rotate(mat4(1.0f), 0.1f, vec3(0.0f, -1.0f, 0.0f)) * obj->xform.rotation;
    }


    static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
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


    /**
     * Error callback function
     */
    static void error_callback(int error, const char* description)
    {
        fputs(description, stderr);
    }

};

