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
class SDL_Window;

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

struct window_mode
{
    size_t width         = 640;
    size_t heigth        = 480;
    bool   is_fullscreen = false;
};

// class engine;
class RenderObj;
class gfx_prog;
class SoundBuffer;

class game
{
private:
public:
    virtual ~game();
    virtual bool getIsRunning()                                   = 0;
    virtual void on_initialize()                                  = 0;
    virtual void on_event(my_engine::event&)                      = 0;
    virtual void on_update(std::chrono::microseconds frame_delta) = 0;
    // virtual void on_render(RenderObj&, Texture&) const         = 0;
    virtual void on_render() = 0;
};

my_engine::game* create_game();
void             destroy_game(my_engine::game* e);

// global function engine
std::string initialize(std::string_view   title,
                       const window_mode& desired_window_mode);

SDL_Window* getWindow();
float       get_time_from_init();
/// pool event from input queue
/// return true if more events in queue
bool read_event(event& e);

// void update_imGui();
void render_imgui();

void render(RenderObj&, Texture&, const matrix2x3&);
void swap_buffers();

void uninitialize();
// global function engine

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

gfx_prog* create_gfx_prog(const std::string& path,
                          const std::string& name_shader);
void      destroy_gfx_prog(gfx_prog* e);

class RenderObj
{
public:
    virtual ~RenderObj();
    virtual void load_mesh_from_file(const std::string_view path) = 0;
    virtual void bind() const                                     = 0;
    virtual void setProg(gfx_prog* prog_)                         = 0;
    // virtual void linkProg()                                           = 0;
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

RenderObj* create_RenderObj();
void       destroy_RenderObj(RenderObj* e);

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

SoundBuffer* create_sound_buffer(std::string_view path);
void         destroy_sound_buffer(SoundBuffer*);

} // namespace my_engine
