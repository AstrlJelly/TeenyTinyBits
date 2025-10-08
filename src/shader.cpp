#include "shader.hpp"

#include <cstdarg>
#include <fstream>
#include <iostream>

#include "glm/glm/gtc/type_ptr.hpp"

Shader::Shader(uint32_t shaderCount, ...)
{
    if (!s_initialized)
    {
        Shader::add_all_shader_include_strings();

        s_initialized = true;
    }

    std::va_list args;

    GLuint shaderIds[shaderCount];
    va_start(args, shaderCount);
    for (int32_t i = 0; i < shaderCount; i++) {
        int32_t shaderType = va_arg(args, int32_t);
        const char* shaderPath = va_arg(args, const char*);
        shaderIds[i] = compile_shader_from_path(shaderType, shaderPath);
    }
    va_end(args);

    // shader Program
    this->programID = glCreateProgram();
    for (int32_t i = 0; i < shaderCount; i++) {
        GLuint shaderId = shaderIds[i];
        glAttachShader(programID, shaderId);
    }
    glLinkProgram(programID);
    
    int32_t success;
    char infoLog[512];
    // print linking errors if any
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(1);
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    for (int32_t i = 0; i < shaderCount; i++) {
        GLuint shaderId = shaderIds[i];
        glDeleteShader(shaderId);
    }

    this->update_uniforms();
}

ComputeShader::ComputeShader(const char* computePath) 
    : Shader(1,
        GL_COMPUTE_SHADER, computePath
    ) {}

void ComputeShader::dispatch_indirect()
{
    this->use();
    glDispatchComputeIndirect(0);
}

void ComputeShader::dispatch(int32_t x, int32_t y, int32_t z)
{
    this->use();
    glDispatchCompute(x, y, z);
}

PipelineShader::PipelineShader(const char* vertexPath, const char* fragmentPath)
    : Shader(2,
        GL_VERTEX_SHADER, vertexPath,
        GL_FRAGMENT_SHADER, fragmentPath
    ) {}

GLuint Shader::compile_shader_from_path(int32_t shaderType, const char* path)
{
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* shaderCode = code.c_str();

    // compile shader
    uint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);

    fs::path absoluteIncludePath = fs::absolute(shaderIncludePath);
    const char* includePathStr = absoluteIncludePath.c_str();
    const char* searchDirectories[] = { includePathStr };
    glCompileShaderIncludeARB(shaderID, glm::countof(searchDirectories), searchDirectories, nullptr);

    // print compile errors if any
    int32_t success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        int32_t infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::string infoLogStr;
        if (infoLogLength)
        {
            char infoLog[infoLogLength];
            glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);
            infoLogStr = std::string(infoLog);
        }
        else
        {
            infoLogStr = std::string("Info log was empty. Are you sure the shader was compiled?");
        }

        std::cerr << "ERROR::SHADER::" << Shader::get_shader_type_string(shaderType) << "::COMPILATION_FAILED : " << infoLogLength << " chars" << "\n" << infoLogStr << std::endl;
        exit(1);
    }

    return shaderID;
}

void Shader::update_uniforms()
{
    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    GLsizei bufSize;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    // count of uniforms
    GLint count;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);

    uniformLocations = {};
    for (GLint i = 0; i < count; i++)
    {
        glGetActiveUniform(programID, i, bufSize, &length, &size, &type, name);

        uniformLocations.insert({name, i});
    }
}

const std::string& Shader::get_shader_type_string(int32_t shaderType)
{
    return SHADER_TYPE_STRINGS.at(shaderType);
}

void Shader::add_all_shader_include_strings()
{
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(shaderIncludePath))
    {
        const fs::path& path = dir_entry.path();

        std::fstream file{path};
        std::string line;
        std::stringstream sourceStream{};
        while (std::getline(file, line))
        {
            sourceStream << line << "\n";
        }
        const std::string& source = sourceStream.str();

        // offset string to get a substring - "/directory_name/included.glsl"
        u_int16_t offset = shaderIncludePath.string().length();
        std::string includeString = std::string(offset + path.c_str());
        glNamedStringARB(GL_SHADER_INCLUDE_ARB, includeString.length(), includeString.c_str(), source.length(), source.c_str());
    }
}


void Shader::use() 
{ 
    glUseProgram(programID);
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
        GLint location = glGetUniformLocation(programID, name.c_str());
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
    glUniform1i(location, static_cast<int32_t>(value));
}
void Shader::set_int(const std::string &name, int32_t value) const
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

PhysicsObject::PhysicsObject(glm::vec2 position, glm::vec2 velocity, float radius)
{
    this->position = position;
    this->velocity = velocity;
    this->radius = radius;
}