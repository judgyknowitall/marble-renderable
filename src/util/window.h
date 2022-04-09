#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

#include "objFile.h"
#include "state.h"

class MyWindow
{
public:

    // Window width and height
    int width, height;

    MyWindow() {}
    MyWindow(ObjFile* o, State* s);

    ~MyWindow()
    {
        glfwDestroyWindow(mWindow);
    }

    GLFWwindow* getWindow() {
        return mWindow;
    }
    void handleResizing(function<void(int, int)> func);

    static void error_callback(int error, const char* description);
    void key_callback(int key, int scancode, int action, int mods);

private:

    ObjFile* obj;
    State* state;
    GLFWwindow* mWindow;

    static void onKey(GLFWwindow* window, int key, int scancode, int actions, int mods)
    {
        MyWindow* w = (MyWindow*) glfwGetWindowUserPointer(window);
        w->key_callback(key, scancode, actions, mods);
    }

    static void onError(int error, const char* description)
    {
        fputs(description, stderr);
    }
};