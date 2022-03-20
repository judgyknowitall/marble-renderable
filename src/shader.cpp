#include "shader.h"
#include <iostream>

using namespace std;


////////////////////////////////////////////////////////////////
// Load the shader from the specified file. Returns false if the
// shader could not be loaded
static GLubyte shaderText[8192];
bool Shader::loadShaderFile(const char* filename, GLuint shader) {
    GLint shaderLength = 0;
    FILE* fp;

    // Open the shader file
    fp = fopen(filename, "r");
    if (fp != NULL) {
        // See how long the file is
        while (fgetc(fp) != EOF)
            shaderLength++;

        // Go back to beginning of file
        rewind(fp);

        // Read the whole file in
        fread(shaderText, 1, shaderLength, fp);

        // Make sure it is null terminated and close the file
        shaderText[shaderLength] = '\0';
        fclose(fp);
    }
    else {
        return false;
    }

    // Load the string into the shader object
    GLchar* fsStringPtr[1];
    fsStringPtr[0] = (GLchar*)((const char*)shaderText);
    glShaderSource(shader, 1, (const GLchar**)fsStringPtr, NULL);

    return true;
}




////////////////////////////////////////////////////////////////////////////

// This function does any needed initialization on the rendering
// context. 
void Shader::setupRenderingContext() {
    // Background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // First setup the shaders
    //Now, let's setup the shaders
    GLuint hVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    myShaderProgram = (GLuint)NULL;
    GLint testVal;

    if (!loadShaderFile(vsFilename.c_str(), hVertexShader)) {
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
        cout << "The shader " << vsFilename << " could not be found." << endl;
    }

    if (!loadShaderFile(fsFilename.c_str(), hFragmentShader)) {
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
        cout << "The shader " << fsFilename << " could not be found." << endl;
    }

    glCompileShader(hVertexShader);
    glCompileShader(hFragmentShader);

    // Check for any error generated during shader compilation
    glGetShaderiv(hVertexShader, GL_COMPILE_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        char source[8192];
        char infoLog[8192];
        glGetShaderSource(hVertexShader, 8192, NULL, source);
        glGetShaderInfoLog(hVertexShader, 8192, NULL, infoLog);
        cout << "The shader: " << endl << (const char*)source << endl << " failed to compile:" << endl;
        fprintf(stderr, "%s\n", infoLog);
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
    }
    glGetShaderiv(hFragmentShader, GL_COMPILE_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        char source[8192];
        char infoLog[8192];
        glGetShaderSource(hFragmentShader, 8192, NULL, source);
        glGetShaderInfoLog(hFragmentShader, 8192, NULL, infoLog);
        cout << "The shader: " << endl << (const char*)source << endl << " failed to compile:" << endl;
        fprintf(stderr, "%s\n", infoLog);
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
    }

    // Create the shader program and bind locations for the vertex
    // attributes before linking. The linking process can also generate errors

    myShaderProgram = glCreateProgram();
    glAttachShader(myShaderProgram, hVertexShader);
    glAttachShader(myShaderProgram, hFragmentShader);

    glBindAttribLocation(myShaderProgram, VERTEX_DATA, "position");
    //glBindAttribLocation( myShaderProgram, VERTEX_COLOUR, "vColor" );
    glBindAttribLocation(myShaderProgram, VERTEX_NORMAL, "normal");

    glLinkProgram(myShaderProgram);
    glDeleteShader(hVertexShader);
    glDeleteShader(hFragmentShader);
    glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        char infoLog[1024];
        glGetProgramInfoLog(myShaderProgram, 1024, NULL, infoLog);
        cout << "The shader program (" << vsFilename << " + " << fsFilename << ") failed to link:" << endl << (const char*)infoLog << endl;
        glDeleteProgram(myShaderProgram);
        myShaderProgram = (GLuint)NULL;
    }

    // setup the vertex state array object. All subsequent buffers willbe bound to it.
    glGenVertexArrays(1, &myVAO);
    glBindVertexArray(myVAO);
}