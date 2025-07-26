#include "shader.hpp"
#include <GL/glext.h>
#include <cstdarg>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    unsigned int vertex = compile_shader(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragment = compile_shader(fragmentPath, GL_FRAGMENT_SHADER);

    // shader Program
    ID = create_program(2, vertex, fragment);

    this->update_uniforms();
}

unsigned int compile_shader(const char* path, int shaderType)
{
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        shaderFile.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        code   = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* shaderCode = code.c_str();

    // 2. compile shaders
    unsigned int shaderID;
    int success;
    char infoLog[512];
    
    // vertex Shader
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    // print compile errors if any
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(1);
    }

    return shaderID;
}

unsigned int create_program(int count, ...)
{
    std::va_list args;
    va_start(args, count);
    
    unsigned int ID = glCreateProgram();
    for (int i = 0; i < count; i++) {
        glAttachShader(ID, va_arg(args, GLuint));
    }
    glLinkProgram(ID);
    
    int success;
    char infoLog[512];
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    for (int i = 0; i < count; i++) {
        glDeleteShader(va_arg(args, GLuint));
    }

    return ID;
}

void Shader::update_uniforms()
{
    // variables of uniforms
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 255;
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &count);

    uniformLocations = {};
    for (GLint i = 0; i < count; i++)
    {
        glGetActiveUniform(ID, i, bufSize, &length, &size, &type, name);

        uniformLocations.insert({name, i});
    }
}

void Shader::use() 
{ 
    glUseProgram(ID);
}

GLint Shader::get_uniform_location(const std::string &name) const
{
    auto possibleLocation = uniformLocations.find(name);
    if (possibleLocation != uniformLocations.end())
    {
        return possibleLocation->second;
    }
    else
    {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location < 0)
        {
            std::cerr << "Uniform with name \"" << name << "\" was not found in cache. Did you spell it wrong, or are you not using this uniform in your glsl code?\n";
        }
        else
        {
            std::cout << "Uniform with name \"" << name << "\" was not found in cache. Fallback provided.\n";
        }
        return location;
    }
}

void Shader::set_bool(const std::string &name, bool value) const
{
    GLint location = this->get_uniform_location(name);
    glUniform1i(location, static_cast<int>(value));
}
void Shader::set_int(const std::string &name, int value) const
{
    GLint location = this->get_uniform_location(name);
    glUniform1i(location, value);
}
void Shader::set_float(const std::string &name, float value) const
{
    GLint location = this->get_uniform_location(name);
    glUniform1f(location, value);
}
void Shader::set_vec3(const std::string &name, double x, double y, double z) const
{
    this->set_vec3(name, glm::vec3(x, y, z));
}
void Shader::set_vec3(const std::string &name, glm::vec3 value) const
{
    GLint location = this->get_uniform_location(name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}
void Shader::set_mat4(const std::string &name, glm::mat4 value) const
{
    GLint location = this->get_uniform_location(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}