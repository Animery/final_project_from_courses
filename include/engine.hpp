#pragma once

#include "Texture.hpp"
#include "figure_struct.hpp"
#include "matrix.hpp"
// #include <iosfwd>
#include <chrono>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

typedef int GLint;

namespace my_engine
{
/// dendy gamepad emulation events
enum class event_type
{
    input_key,
    hardware,
    mouse
};

enum class keys_type
{
    left,
    right,
    up,
    down,
    select,
    start,
    button1,
    button2
};

struct input_data
{
    my_engine::keys_type key;
    bool                 is_down;
};

struct hardware_data
{
    bool is_reset;
};

struct event
{
    std::variant<input_data, hardware_data> info;
    double                                  timestamp;
    event_type                              type;
    float                                   x = 0;
    float                                   y = 0;
};

std::ostream& operator<<(std::ostream& stream, const event e);

class engine;
class RenderObj;
class gfx_prog;
class SoundBuffer;

/// return not null on success

class engine
{
public:
    virtual ~engine();
    /// create main window
    /// on success return empty string
    virtual std::string initialize(std::string_view config) = 0;
    virtual float       get_time_from_init()                = 0;

    /// pool event from input queue
    /// return true if more events in queue
    virtual bool read_event(event& e)         = 0;
    virtual bool is_key_down(const keys_type) = 0;

    virtual void render(/*const*/ RenderObj&, /*const*/ Texture&) = 0;
    virtual void render(RenderObj&, Texture&, const matrix2x3&)   = 0;
    // virtual void render(RenderObj*, Texture*, const matrix2x3&)   = 0;
    virtual void swap_buffers() = 0;

    virtual SoundBuffer* create_sound_buffer(std::string_view path) = 0;
    virtual void         destroy_sound_buffer(SoundBuffer*)         = 0;

    virtual void uninitialize() = 0;
};

class gfx_prog
{
public:
    virtual ~gfx_prog();
    virtual void bind_attrib(GLuint index, std::string_view attrib) = 0;
    virtual void link()                                             = 0;
    virtual void use() const                                        = 0;
    // virtual GLuint getProg_id(){return prog_id;}
    // Check the validate status
    virtual void check_validate()                                      = 0;
    virtual void setUniform(Texture& tex) const                        = 0;
    virtual void setUniform(std::string_view name, const matrix2x3& m) = 0;
};

class RenderObj
{
public:
    virtual ~RenderObj();
    virtual void load_mesh_from_file(const std::string_view path) = 0;
    virtual void bind() const                                     = 0;
    virtual void setProg(gfx_prog* prog_)                         = 0;
    // virtual void linkProg()                                              = 0;
    virtual void useProg() const                                         = 0;
    virtual void setUniform(Texture& tex) const                          = 0;
    virtual void setUniform(std::string_view name, const matrix2x3& mat) = 0;
    // virtual void bind_attrib(GLuint index, std::string_view attrib)   = 0;
    virtual void draw(unsigned int type = 0x0004) const = 0;

    virtual void addVertexBufferObject(const std::vector<float>& data) = 0;
    virtual void addVertexBufferObject(const std::vector<vec2>& data)  = 0;
    virtual void addVertexBufferObject(const std::vector<vec3>& data)  = 0;
    virtual void addIndices(const std::vector<unsigned>& data)         = 0;
};

class SoundBuffer
{
public:
    enum class properties
    {
        once,
        looped
    };

    virtual ~SoundBuffer();
    virtual void play(const properties) = 0;
};

class game
{
private:
public:
    virtual ~game();
    virtual bool getIsRunning()                                   = 0;
    virtual void on_initialize()                                  = 0;
    virtual void on_event(my_engine::event&)                      = 0;
    virtual void on_update(std::chrono::milliseconds frame_delta) = 0;
    virtual void on_render(RenderObj&, Texture&) const            = 0;
    virtual void on_render() const                                = 0;
};

engine* create_engine();
void    destroy_engine(engine* e);

RenderObj* create_RenderObj();
void       destroy_RenderObj(RenderObj* e);

gfx_prog* create_gfx_prog(const std::string& path,
                          const std::string& name_shader);
void      destroy_gfx_prog(gfx_prog* e);

my_engine::game* create_game(my_engine::engine* engine);
void             destroy_game(my_engine::game* e);

} // namespace my_engine
