#pragma once

#include "engine.hpp"
#include "matrix.hpp"

// #include "glad/glad.h"

#include <cassert>
#include <string>
#include <string_view>

#define CHECK

#if defined(CHECK)

#define OM_GL_CHECK()                                                          \
    {                                                                          \
        const GLenum err = glGetError();                                       \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << "GL_INVALID_ENUM" << std::endl;               \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << "GL_INVALID_VALUE" << std::endl;              \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << "GL_INVALID_OPERATION" << std::endl;          \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"            \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;              \
                    break;                                                     \
            }                                                                  \
            std::cerr << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__    \
                      << ')' << std::endl;                                     \
            assert(false);                                                     \
        }                                                                      \
    }
#else
#define OM_GL_CHECK()
#endif //

namespace my_engine
{

class gfx_prog_impl : public gfx_prog
{
private:
    GLuint prog_id;
    GLuint vert_shader;
    GLuint frag_shader;
    void   load_file(std::string_view path, std::string* result);
    GLuint create_shader(std::string_view path, GLuint type);

public:
    explicit gfx_prog_impl(const std::string& path,
                           const std::string& name_shader);
    ~gfx_prog_impl();

    void bind_attrib(GLuint index, std::string_view attrib);
    void link();
    void use() const;
    // GLuint getProg_id(){return prog_id;}

    // Check the validate status
    void check_validate();

    void setUniform(const Animate::Texture& tex) const;
    void setUniform(const std::vector<Animate::Texture*>& tex_arr) const;
    void setUniform(std::string_view name, const matrix2x3& m) const;
};

} // namespace my_engine