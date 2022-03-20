#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "objFile.h"

class MyWindow
{
public:

    MyWindow() {}
    MyWindow(ObjFile* o);

    ~MyWindow()
    {
        glfwDestroyWindow(mWindow);
    }

    GLFWwindow* getWindow() {
        return mWindow;
    }

    static void error_callback(int error, const char* description);
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_callback(int button, int action, int mods);

private:

    ObjFile* obj;
    GLFWwindow* mWindow;

    static void onKey(GLFWwindow* window, int key, int scancode, int actions, int mods)
    {
        MyWindow* w = (MyWindow*) glfwGetWindowUserPointer(window);
        w->key_callback(key, scancode, actions, mods);
    }

    static void onMouseClick(GLFWwindow* window, int button, int action, int mods)
    {
        MyWindow* w = (MyWindow*) glfwGetWindowUserPointer(window);
        w->mouse_callback(button, action, mods);
    }

    static void onError(int error, const char* description)
    {
        fputs(description, stderr);
    }
};