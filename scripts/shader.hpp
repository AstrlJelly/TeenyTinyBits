#pragma once

#include <map>
#include <string>
#include <filesystem>

#include <glad/glad.h>

#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header

namespace fs = std::filesystem;

class Shader
{
private:
    static const inline std::map<int32_t, std::string> SHADER_TYPE_STRINGS = {
        { GL_COMPUTE_SHADER,  "COMPUTE" },
        { GL_GEOMETRY_SHADER, "GEOMETRY" },
        { GL_FRAGMENT_SHADER, "FRAGMENT" },
        { GL_VERTEX_SHADER,   "VERTEX" },
    };

    // the program ID
    GLuint programID;

    // variable name, location
    std::map<std::string, GLint> uniformLocations;

    void update_uniforms();

    static inline bool s_initialized = false;

    static GLuint compile_shader_from_path(int32_t shaderType, const char* path);
    static void add_all_shader_include_strings();
    
public:
    static const inline fs::path shaderIncludePath = "shaders/include";
  
    // might be useful if shaders will be created outside of this class
    // Shader(GLuint program);

    // constructor reads and builds the shader
    Shader(uint32_t argc, ...);

    // use/activate the shader
    void use();

    // utility uniform functions
    GLint get_uniform_location(const std::string& name) const;
    void set_bool (const std::string& name, bool value)                   const;
    void set_int  (const std::string& name, int32_t value)                const;
    void set_float(const std::string& name, float value)                  const;
    void set_vec3 (const std::string& name, glm::vec3 value)              const;
    void set_vec3 (const std::string& name, double x, double y, double z) const;
    void set_mat4 (const std::string& name, glm::mat4 value)              const;

    // duh
    static bool is_shader(GLuint programID);

    static const std::string& get_shader_type_string(int32_t shaderType);
};

class PipelineShader : public Shader
{
public:
    PipelineShader(const char* vertexPath, const char* fragmentPath);
};

class ComputeShader : public Shader
{
public:
    ComputeShader(const char* computePath);

    void dispatch_indirect();
    void dispatch(int32_t x = 1, int32_t y = 1, int32_t z = 1);
};

struct PhysicsObject
{
    PhysicsObject(glm::vec2 position = glm::vec2(0), glm::vec2 velocity = glm::vec2(0), float radius = 1);
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    // this will make the struct be the same size as the struct in glsl.
    // don't use this for anything...
    float __buffer;
};
