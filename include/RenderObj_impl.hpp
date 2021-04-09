#pragma once

#include "engine.hpp"
// #include "shader.hpp"
#include "Texture.hpp"

#include <vector>

namespace my_engine
{

class RenderObj_impl : public virtual RenderObj
{
public:
    RenderObj_impl();
    ~RenderObj_impl();

    void bind() const final;
    void load_mesh_from_file(const std::string_view path) final;
    void setProg(gfx_prog* prog_) final;
    // void linkProg() final;
    void useProg() const final;
    void setUniform(const Texture& tex) const;
    void setUniform(const std::vector<Texture*>& tex_arr) const;
    void setUniform(std::string_view name, const matrix2x3& mat) const ;
    // void bind_attrib(GLuint index, std::string_view attrib)final ;

    void addVertexBufferObject(const std::vector<float>& data);
    void addVertexBufferObject(const std::vector<vec2>& data);
    void addVertexBufferObject(const std::vector<vec3>& data);
    void addIndices(const std::vector<unsigned>& data) final;

    void draw(unsigned int type = 0x0004) const final;

private:
    GLuint              vao_id;
    std::vector<GLuint> vbo_buffers_id;
    GLuint              ebo_id        = 0;
    size_t              count_indexes = 0;
    gfx_prog*           prog          = nullptr;
};
} // namespace my_engine