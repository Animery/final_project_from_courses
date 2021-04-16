#include "shader.hpp"

#include "glad/glad.h"

#include <fstream>
#include <iostream> // for DEBUG
#include <vector>

namespace my_engine
{
void gfx_prog_impl::load_file(std::string_view path, std::string* result)
{
    std::cout << path << "\tloading" << std::endl;
    std::ifstream file(path.data(), std::ios_base::in | std::ios_base::ate);
    file.exceptions(std::ios_base::failbit);
    auto        size = file.tellg();
    std::string text(size, '\0');
    file.seekg(0);
    file.read(&text[0], size);
    *result = text;
}

GLuint gfx_prog_impl::create_shader(std::string_view path, GLuint type)
{
    std::string shader_txt;
    load_file(path.data(), &shader_txt);
    const char* txt    = shader_txt.data();
    GLuint      shader = glCreateShader(type);

    glShaderSource(shader, 1, &txt, nullptr);
    OM_GL_CHECK()

    glCompileShader(shader);
    OM_GL_CHECK()

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    OM_GL_CHECK()
    if (!ok)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        GLchar log[infoLen];
        glGetShaderInfoLog(shader, infoLen, nullptr, log);
        std::cout << "\nERROR\n"
                  << "\ninfoLen: " << infoLen << "\n"
                  << log << std::endl;
    }
    return shader;
}

// void gfx_prog_impl::setUniform(std::string_view name, GLint value) const
// {
//     const GLint location = glGetUniformLocation(prog_id, name.data());
//     OM_GL_CHECK()
//     assert(location != -1);
//     glUniform1i(location, value);
//     OM_GL_CHECK()
// }

void gfx_prog_impl::setUniform(const Animate::Texture& tex) const
{
    assert(&tex != nullptr);

    const GLint location = glGetUniformLocation(prog_id, tex.getName().c_str());
    // std::cout << "tex name:\t" << tex.getName() << std::endl;
    OM_GL_CHECK()
    if (location == -1)
    {
        std::cerr << "can't get uniform location from shader\n";
        throw std::runtime_error("can't get uniform location");
    }
    unsigned int texture_unit = 0;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    OM_GL_CHECK()

    tex.bind();

    // http://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml
    glUniform1i(location, static_cast<int>(0 + texture_unit));
    OM_GL_CHECK()
}

void gfx_prog_impl::setUniform(const std::vector<Animate::Texture*>& tex_arr) const
{
    for (auto&& it : tex_arr)
    {
        assert(it != nullptr);
    }
    const int size = tex_arr.size();

    for (int i = 0; i < size; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        tex_arr[i]->bind();
        OM_GL_CHECK()
        GLint location_ =
            glGetUniformLocation(prog_id, tex_arr[i]->getName().c_str());
        OM_GL_CHECK()
        if (location_ == -1)
        {
            std::cerr << "can't get uniform location from shader\n";
            throw std::runtime_error("can't get uniform location");
        }

        glUniform1i(location_, static_cast<int32_t>(i));
        OM_GL_CHECK()
    }
    // glActiveTexture(GL_TEXTURE0);
}

void gfx_prog_impl::setUniform(std::string_view name, const matrix2x3& m) const
{
    const GLint location = glGetUniformLocation(prog_id, name.data());
    OM_GL_CHECK();
    assert(location != -1);
    // OpenGL wants matrix in column major order
    // clang-format off
        float values[9] = { m.col0.x,  m.col0.y, m.delta.x,
                            m.col1.x, m.col1.y, m.delta.y,
                            0.f,      0.f,       1.f };
    // clang-format on
    glUniformMatrix3fv(location, 1, GL_FALSE, &values[0]);
    OM_GL_CHECK();
}

gfx_prog_impl::gfx_prog_impl(const std::string& path,
                             const std::string& name_shader)
{
    std::cout << "+++ ctor gfx_prog_impl" << std::endl;
    vert_shader = create_shader(path + name_shader + ".vert", GL_VERTEX_SHADER);
    OM_GL_CHECK()

    frag_shader =
        create_shader(path + name_shader + ".frag", GL_FRAGMENT_SHADER);
    OM_GL_CHECK()

    prog_id = glCreateProgram();
    OM_GL_CHECK()
}

void gfx_prog_impl::bind_attrib(GLuint index, std::string_view attrib)
{
    glBindAttribLocation(prog_id, index, attrib.data());
    OM_GL_CHECK()
}

void gfx_prog_impl::link()
{
    glAttachShader(prog_id, vert_shader);
    OM_GL_CHECK()
    glAttachShader(prog_id, frag_shader);
    OM_GL_CHECK()
    glLinkProgram(prog_id);
    OM_GL_CHECK()

    // Check the link status
    GLint linked_status = 0;
    glGetProgramiv(prog_id, GL_LINK_STATUS, &linked_status);
    OM_GL_CHECK()
    if (linked_status == 0)
    {
        GLint infoLen = 0;
        glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &infoLen);

        GLchar log[infoLen];
        glGetProgramInfoLog(prog_id, infoLen, nullptr, log);
        std::cout << "\nERROR\n" << log << std::endl;
    }
}

void gfx_prog_impl::use() const
{
    glUseProgram(prog_id);
}

void gfx_prog_impl::check_validate()
{
    glValidateProgram(prog_id);
    GLint validate_status = 0;
    glGetProgramiv(prog_id, GL_VALIDATE_STATUS, &validate_status);
    OM_GL_CHECK()
    if (validate_status == GL_FALSE)
    {
        GLint infoLen = 0;
        glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &infoLen);
        OM_GL_CHECK()
        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(prog_id, infoLen, nullptr, infoLog.data());
        OM_GL_CHECK()
        std::cerr << "Error validate program:\n" << infoLog.data();
        throw std::runtime_error("error");
    }
}

gfx_prog_impl::~gfx_prog_impl()
{
    glDetachShader(prog_id, vert_shader);
    glDetachShader(prog_id, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glDeleteProgram(prog_id);
    std::cout << "--- destor gfx_prog_impl" << std::endl;
}

} // namespace my_engine