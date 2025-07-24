#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <glm/glm/glm.hpp>
#include <glm/glm/fwd.hpp>
#include "glm/glm/gtc/type_ptr.hpp"

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
private:
    void update_uniforms();
    
public:
    // the program ID
    unsigned int ID;

    // variable name, location
    std::map<std::string, GLuint> uniformLocations;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, glm::mat4 value) const;
};