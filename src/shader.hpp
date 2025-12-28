#pragma once

#include <array>
#include <map>
#include <string>
#include <filesystem>

#include <glad/glad.h>

#include "glm/glm/glm.hpp" // IWYU pragma: keep, common header

namespace fs = std::filesystem;

#define SHADER_TYPES \
    X(COMPUTE)   \
    X(GEOMETRY)  \
    X(FRAGMENT)  \
    X(VERTEX)    \

#define X(type) type = GL_ ## type ## _SHADER,
enum class ShaderType
{
    SHADER_TYPES
};
#undef X


struct ShaderInfo
{
    ShaderType type;
    std::string path;

    ShaderInfo(ShaderType type, std::string path);
};

class Shader
{
private:
    // the program ID
    GLuint programID;

    // variable name, location
    std::map<std::string, GLint> uniformLocations;

    /**
     * @brief Initializes a map of uniform names to their internal locations,
     *        to reduce observation of OpenGL state
     */
    void initialize_uniforms();

    static inline bool s_initialized = false;

    /**
    * @brief Compiles shader code at a given path.
    * s
    * @param shaderType Any shader type
    * @param path Path to shader file
    * @note Variadic functions cannot include non-trivial types (such as const std::string&), so this uses a C string
    * @return GLuint Location of compiled shader
    */
    static GLuint compile_shader_from_path(ShaderType shaderType, const char* path);
    static void add_all_shader_include_strings();
    
public:
    static const inline fs::path shaderIncludePath = "shaders/include";

    // stringize the types as an array
    #define X(type) std::string(#type),
    static const inline std::array shaderTypeStrings{ SHADER_TYPES };
    #undef X
  
    // might be useful if shaders will be created outside of this class
    // Shader(GLuint program);

    // reads and compiles the shader
    Shader(std::initializer_list<ShaderInfo> shaderInfos);

    // use/activate the shader
    void use();


    // utility uniform functions
    /**
     * @brief Get the GL location of a named uniform
     * 
     * @param name Name of the uniform
     * @return GLint The location of the uniform
     */
    GLint get_uniform_location(const std::string& name) const;

    void set_bool (const std::string& name, bool value);
    void set_int  (const std::string& name, int32_t value);
    void set_float(const std::string& name, float value);
    void set_vec3 (const std::string& name, glm::vec3 value);
    void set_vec3 (const std::string& name, double x, double y, double z);
    void set_mat4 (const std::string& name, glm::mat4 value);


    /**
     * @brief Returns true if the provided program ID is a shader
     * @remark Non-trivial processing time. GLFW state is not effeciently observed
     * 
     * @param programID
     */
    static bool is_shader(GLuint programID);

    /**
     * @brief Get the name of the inputted type of shader
     * 
     * @param shaderType Any shader type
     * @return std::string ShaderType stringized at preprocessing time
     */
    static const std::string& get_shader_type_string(ShaderType shaderType);
};

class PipelineShader
{
private:
    Shader pipelineShader;

public:
    PipelineShader(const std::string& vertexPath, const std::string& fragmentPath);

    Shader get_shader() { return pipelineShader; };
};

class ComputeShader
{
private:
    Shader computeShader;

public:
    ComputeShader(const std::string& computePath);

    Shader get_shader() { return computeShader; };

    void dispatch_indirect();
    void dispatch(int32_t x = 1, int32_t y = 1, int32_t z = 1);
};

struct PhysicsObject
{
    PhysicsObject(glm::vec2 position = glm::vec2(0), glm::vec2 velocity = glm::vec2(0), float radius = 1);
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    /*
     * this will make the cpp struct be the same size as the glsl struct.
     * fixes some issues with how padding works, cuz of alignment stuffz
     * don't use this for anything... or do. i'm not your mom (hopefully?)
     */
    float __buffer;
};
