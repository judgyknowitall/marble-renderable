#include "shader.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////
// Shader Set up
/////////////////////////////////////////////////////////////////////////////////////////


// Constructor
// Create shaders and link program
Shader::Shader(string vsFilename, string fsFilename) {

    // First setup the shaders
    GLuint hVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    myShaderProgram = (GLuint)NULL;

    // Read in shaders
    if (!loadShaderFile(vsFilename.c_str(), hVertexShader) || 
        !loadShaderFile(fsFilename.c_str(), hFragmentShader)) {
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
        
    }

    // Compile Shaders
    glCompileShader(hVertexShader);
    glCompileShader(hFragmentShader);

    // Check for any error generated during shader compilation
    if (!checkShaderCompilation(hVertexShader, "VERTEX") || 
        !checkShaderCompilation(hFragmentShader, "FRAGMENT")) {
        glDeleteShader(hVertexShader);
        glDeleteShader(hFragmentShader);
    }

    // Create the shader program and bind locations for the vertex
    // attributes before linking. The linking process can also generate errors
    myShaderProgram = glCreateProgram();
    glAttachShader(myShaderProgram, hVertexShader);
    glAttachShader(myShaderProgram, hFragmentShader);

    // Link Program
    glLinkProgram(myShaderProgram);
    if (!checkShaderCompilation(myShaderProgram, "PROGRAM")) {
        cout << "ERROR: The shader program (" << vsFilename << " + " << fsFilename << ") failed to link." << endl;
        glDeleteProgram(myShaderProgram);
        myShaderProgram = (GLuint)NULL;
    }

    glDeleteShader(hVertexShader);
    glDeleteShader(hFragmentShader);
}


// Load the shader from the specified file. 
bool Shader::loadShaderFile(const char* filename, GLuint shader) {

    string shaderCode;
    ifstream shaderFile;
    // ensure ifstream objects can throw exceptions
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filename);          // open fle
        stringstream shaderStream;
        shaderStream << shaderFile.rdbuf(); // read file's buffer contents into streams
        shaderFile.close();                 // close file handlers
        shaderCode = shaderStream.str();    // convert stream into string
    }
    catch (ifstream::failure& e) {
        cout << "ERROR: The shader " << filename << " could not be found." << endl;
        return false;
    }

    // Load the string into the shader object
    const char* constShaderCode = shaderCode.c_str();
    glShaderSource(shader, 1, &constShaderCode, NULL);

    return true;
}



// Check for any error generated during shader compilation/linking errors
bool Shader::checkShaderCompilation(GLuint shader, string type)
{
    GLint success;
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[8192];
            glGetShaderInfoLog(shader, 8192, NULL, infoLog);
            cout << "ERROR: The " << type << " shader failed to compile!" << endl;
            cout << "   " << (const char*)infoLog << endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << (const char*)infoLog << endl;
            return false;
        }
    }
    return true;
}



/////////////////////////////////////////////////////////////////////////////////////////
// Rendering Functions
/////////////////////////////////////////////////////////////////////////////////////////

// Use Program
void Shader::use() {
    glUseProgram(myShaderProgram);
}

// Matrix 4
void Shader::setMat4(const string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Vec 3
void Shader::setVec3(const string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(myShaderProgram, name.c_str()), 1, &value[0]);
}

// Int
void Shader::setInt(const string& name, int value) const
{
    glUniform1i(glGetUniformLocation(myShaderProgram, name.c_str()), value);
}