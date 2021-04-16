#include "RenderObj_impl.hpp"
#include "configuration_loader.hxx"

#include "glad/glad.h"

#include <cassert>
#include <iostream>
#include <sstream>

namespace my_engine
{

RenderObj_impl::RenderObj_impl()
{
    glGenVertexArrays(1, &vao_id);
    bind();
    std::cout << "+++ ctor RenderObj_impl" << std::endl;
}

void RenderObj_impl::bind() const
{
    glBindVertexArray(vao_id);
}

void RenderObj_impl::load_mesh_from_file(const std::string_view path)
{
    bind();
    std::stringstream file = filter_comments(path);

    if (!file)
    {
        std::cerr << "can't load object" << std::endl;
        // throw std::runtime_error("can't load object");
    }

    size_t      num_of_vertexes = 0;
    std::string key;

    file >> key;
    if (key != "num_of_vertexes")
    {
        std::cerr << "no key word: num_of_vertexes" << std::endl;
        // throw std::runtime_error("no key word: num_of_vertexes");
    }

    file >> num_of_vertexes;

    // Position
    key.clear();
    file >> key;

    if (key == "position_vec2")
    {
        std::vector<vec2> position;
        position.reserve(num_of_vertexes);
        for (size_t i = 0; i < num_of_vertexes; i++)
        {
            vec2 temp;
            file >> temp;
            position.push_back(temp);
        }
        addVertexBufferObject(position);
    }
    else
    {
        std::cerr << "no key word: position_vec2" << std::endl;
        // throw std::runtime_error("no key word: position_vec2");
    }

    // Tex
    key.clear();
    file >> key;

    if (key == "tex_vec2")
    {
        std::vector<vec2> tex;
        tex.reserve(num_of_vertexes);
        for (size_t i = 0; i < num_of_vertexes; i++)
        {
            vec2 temp;
            file >> temp;
            tex.push_back(temp);
        }
        addVertexBufferObject(tex);
    }
    else
    {
        std::cerr << "no key word: tex_vec2" << std::endl;
        throw std::runtime_error("no key word: tex_vec2");
    }

    // Tex
    key.clear();
    file >> key;

    if (key == "color_vec3")
    {
        std::vector<vec3> color;
        color.reserve(num_of_vertexes);
        for (size_t i = 0; i < num_of_vertexes; i++)
        {
            vec3 temp;
            file >> temp;
            color.push_back(temp);
        }
        addVertexBufferObject(color);
    }
    else
    {
        std::cerr << "no key word: color_vec3" << std::endl;
        // throw std::runtime_error("no key word: color_vec3");
    }

    // Indices
    key.clear();
    file >> key;
    size_t num_of_indices = 0;
    file >> num_of_indices;

    if (key == "indices")
    {
        std::vector<unsigned> indices;
        indices.reserve(num_of_indices);
        for (size_t i = 0; i < num_of_indices; i++)
        {
            unsigned temp;
            file >> temp;
            indices.push_back(temp);
        }
        addIndices(indices);
    }
    else
    {
        throw std::runtime_error("no key word: indices");
    }
}

/**
 * \brief draw buffer.
 *
 * \param type defaut = GL_TRIANGLES
 * \param option
 *	        GL_TRIANGLES
 *	      , GL_TRIANGLE_STRIP
 *	      , GL_LINES
 *	      , GL_LINE_STRIP
 *	      , GL_POINTS
 */
void RenderObj_impl::draw(unsigned int type) const
{
    assert(ebo_id != 0);
    bind();
    for (size_t i = 0; i < vbo_buffers_id.size(); ++i)
    {
        glEnableVertexAttribArray(i);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    // glDrawElements(type, count_indexes, GL_UNSIGNED_INT, nullptr);
    glDrawElements(type, count_indexes, GL_UNSIGNED_INT, nullptr);

    for (size_t i = 0; i < vbo_buffers_id.size(); ++i)
    {
        glDisableVertexAttribArray(i);
    }
}

void RenderObj_impl::setProg(gfx_prog* prog_)
{
    prog = prog_;
}

// void RenderObj_impl::linkProg()
// {
//     if (prog == nullptr)
//     {
//         std::cout << "Prog == nullptr" << std::endl;
//     }
//     else
//     {
//         prog->link();
//     }
// }

void RenderObj_impl::useProg() const
{
    if (prog == nullptr)
    {
        std::cout << "Prog == nullptr" << std::endl;
    }
    else
    {
        prog->use();
    }
}

void RenderObj_impl::setUniform(const Animate::Texture& tex) const
{
    if (prog == nullptr)
    {
        std::cout << "Prog == nullptr" << std::endl;
    }
    else
    {
        prog->setUniform(tex);
    }
}

void RenderObj_impl::setUniform(std::string_view name,
                                const matrix2x3& mat) const
{
    if (prog == nullptr)
    {
        std::cout << "Prog == nullptr" << std::endl;
    }
    else
    {
        prog->setUniform(name, mat);
    }
}

void RenderObj_impl::setUniform(
    const std::vector<Animate::Texture*>& tex_arr) const
{
    if (prog == nullptr)
    {
        std::cout << "Prog == nullptr" << std::endl;
    }
    else
    {
        prog->setUniform(tex_arr);
    }
}

// void RenderObj_impl::bind_attrib(GLuint index, std::string_view attrib)
// {
//     if (prog == nullptr)
//     {
//         std::cout << "Prog == nullptr" << std::endl;
//     }
//     else
//     {
//         prog->bind_attrib(index, attrib.data());
//     }
// }

void RenderObj_impl::addVertexBufferObject(const std::vector<float>& data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(float),
                 data.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(
        vbo_buffers_id.size(), 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    // prog->bind_attrib(vbo_buffers_id.size(), attrib.data());
    vbo_buffers_id.push_back(vbo);
}

void RenderObj_impl::addVertexBufferObject(const std::vector<vec2>& data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(vec2),
                 data.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(
        vbo_buffers_id.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    // prog->bind_attrib(vbo_buffers_id.size(), attrib.data());
    vbo_buffers_id.push_back(vbo);
}

void RenderObj_impl::addVertexBufferObject(const std::vector<vec3>& data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(vec3),
                 data.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(
        vbo_buffers_id.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // prog->bind_attrib(vbo_buffers_id.size(), attrib.data());
    vbo_buffers_id.push_back(vbo);
}

void RenderObj_impl::addIndices(const std::vector<unsigned>& data)
{
    assert(ebo_id == 0); // мы не можем создать ещё один буфер

    count_indexes = data.size();

    glGenBuffers(1, &ebo_id);

    // обратите внимание, что мы используем не GL_ARRAY_BUFFER, а
    // GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 data.size() * sizeof(unsigned),
                 data.data(),
                 GL_STATIC_DRAW);
}

RenderObj_impl::~RenderObj_impl()
{
    // if (prog != nullptr)
    //     delete prog;

    glDeleteBuffers(vbo_buffers_id.size(), vbo_buffers_id.data());
    glDeleteBuffers(1, &ebo_id);
    glDeleteVertexArrays(1, &vao_id);
    std::cout << "--- destor RenderObj_impl" << std::endl;
}

} // namespace my_engine