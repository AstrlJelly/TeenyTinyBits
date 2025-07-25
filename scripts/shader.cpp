#include "shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw;
    }
    
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    this->update_uniforms();
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