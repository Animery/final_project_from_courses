// #define DEBUG_more

#include "../game_src/GameConst.hpp"
#include "../include/RenderObj_impl.hpp"
#include "../include/SoundBuffer_impl.hpp"
#include "../include/shader.hpp"

// #include "../include/engine.hpp"
// #include "../include/Texture.hpp"

#include <algorithm>
#include <array>
// #include <cassert>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "../imgui_src/imgui.h"
#include "../imgui_src/imgui_impl_opengl3.h"
#include "../imgui_src/imgui_impl_sdl.h"
#include "../include/glad/glad.h"
#include <SDL2/SDL.h>

using clock_timer = std::chrono::high_resolution_clock;
using nano_sec    = std::chrono::nanoseconds;
using milli_sec   = std::chrono::milliseconds;
using time_point  = std::chrono::time_point<clock_timer, nano_sec>;
clock_timer timer;

namespace my_engine
{
static void APIENTRY
callback_opengl_debug(GLenum                       source,
                      GLenum                       type,
                      GLuint                       id,
                      GLenum                       severity,
                      GLsizei                      length,
                      const GLchar*                message,
                      [[maybe_unused]] const void* userParam);

// static std::array<std::string_view, 17> event_names = {
//     { /// input events
//       "left_pressed",
//       "left_released",
//       "right_pressed",
//       "right_released",
//       "up_pressed",
//       "up_released",
//       "down_pressed",
//       "down_released",
//       "select_pressed",
//       "select_released",
//       "start_pressed",
//       "start_released",
//       "button1_pressed",
//       "button1_released",
//       "button2_pressed",
//       "button2_released",
//       /// virtual console events
//       "turn_off" }
// };

std::ostream& operator<<(std::ostream& stream, const input_data& i)
{
    static const std::array<std::string_view, 8> key_names = { { "left",
                                                                 "right",
                                                                 "up",
                                                                 "down",
                                                                 "select",
                                                                 "start",
                                                                 "button1",
                                                                 "button2" } };

    const std::string_view& key_name = key_names[static_cast<size_t>(i.key)];

    stream << "key: " << key_name << " is_down: " << i.is_down;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const hardware_data& h)
{
    stream << "reset console: " << h.is_reset;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const event& e)
{
    switch (e.type)
    {
        case my_engine::event_type::input_key:
            stream << std::get<my_engine::input_data>(e.info);
            break;
        case my_engine::event_type::hardware:
            stream << std::get<my_engine::hardware_data>(e.info);
            break;
    }
    return stream;
}

static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << '.';
    out << static_cast<int>(v.minor) << '.';
    out << static_cast<int>(v.patch);
    return out;
}

struct bind
{
    bind(std::string_view s, SDL_Keycode k, keys_type om_k)
        : name(s)
        , key(k)
        , my_key(om_k)
    {
    }

    std::string_view name;
    SDL_Keycode      key;

    my_engine::keys_type my_key;
};

const std::array<bind, 8> keys{
    { bind{ "up", SDLK_w, keys_type::up },
      bind{ "left", SDLK_a, keys_type::left },
      bind{ "down", SDLK_s, keys_type::down },
      bind{ "right", SDLK_d, keys_type::right },
      bind{ "button1", SDLK_LCTRL, keys_type::button1 },
      bind{ "button2", SDLK_SPACE, keys_type::button2 },
      bind{ "select", SDLK_ESCAPE, keys_type::select },
      bind{ "start", SDLK_RETURN, keys_type::start } }
};

static bool check_input(const SDL_Event& e, const bind*& result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&e](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    // if (it != end(keys))
    if (it != keys.end())
    {
        result = &(*it);
        return true;
    }
    return false;
}

class engine_impl : public engine
{
public:
    engine_impl();
    ~engine_impl();
    std::string initialize(std::string_view config, game* game) final;
    float       get_time_from_init() final;

    bool read_event(event& e) final;
    bool is_key_down(const enum keys_type key) final;

    void render(RenderObj&, Texture&);
    void render(RenderObj&, Texture&, const matrix2x3&);
    // void render(RenderObj*, Texture*, const matrix2x3&);
    void swap_buffers() final;

    SoundBuffer* create_sound_buffer(std::string_view path) final;
    void         destroy_sound_buffer(SoundBuffer*) final;

    void uninitialize() final;

private:
    void        initImGui(const char* glsl_version);
    void        initAudio();
    void        renderImGui();
    static void audio_callback(void*, uint8_t*, int);

    // Our state
    bool   show_demo_window    = true;
    bool   show_another_window = false;
    ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    SDL_Window*   window     = nullptr;
    game* m_game = nullptr;

    size_t        width      = gameConst::screen_width;  // * 6 = 1920
    size_t        height     = gameConst::screen_height; // * 6 = 1080
    SDL_GLContext gl_context = nullptr;

    SDL_AudioDeviceID              audio_device;
    SDL_AudioSpec                  audio_device_spec;
    std::vector<SoundBuffer_impl*> sounds;

    bool core_or_es = true;
};

engine_impl::engine_impl()
{
    std::cout << "+++ ctor engine_impl" << std::endl;
}
engine_impl::~engine_impl()
{
    std::cout << "--- destor engine_impl" << std::endl;
}

std::string engine_impl::initialize(std::string_view config, game* temp_game) 
{
    m_game = temp_game;

    std::stringstream serr;

    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    if (SDL_COMPILEDVERSION !=
        SDL_VERSIONNUM(linked.major, linked.minor, linked.patch))
    {
        serr << "warning: SDL2 compiled and linked version mismatch: "
             << compiled << " " << linked << std::endl;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        const char* error_message = SDL_GetError();
        serr << "error: failed call SDL_Init: " << error_message;
        return serr.str();
    }

    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG) !=
        0)
    {
        const char* error_message = SDL_GetError();
        serr << "error: failed call SDL_GL_SetAttribute: " << error_message;
        return serr.str();
    }

    window = SDL_CreateWindow("OpenGL",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width,
                              height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr)
    {
        const char* err_message = SDL_GetError();
        serr << "error: failed call SDL_CreateWindow: " << err_message
             << std::endl;
        SDL_Quit();
        return serr.str();
    }

    int         gl_major_ver, gl_minor_ver, gl_context_profile;
    const char* glsl_version;
    if (core_or_es)
    {
        gl_major_ver       = 4;
        gl_minor_ver       = 2;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
        glsl_version       = "#version 420";
    }
    else
    {
        gl_major_ver       = 3;
        gl_minor_ver       = 2;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;
        glsl_version       = "#version 150";
    }

    const char*      platform_from_sdl = SDL_GetPlatform();
    std::string_view platform(platform_from_sdl);

    using namespace std::string_view_literals;
    auto list_sys = { "Windows"sv, "Mac OS X"sv };
    auto it       = find(list_sys.begin(), list_sys.end(), platform);
    if (it != list_sys.end())
    {
        /* code */
        gl_major_ver       = 4;
        gl_minor_ver       = (platform == "Mac OS X") ? 1 : 3;
        gl_context_profile = SDL_GL_CONTEXT_PROFILE_CORE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_context_profile);

    gl_context = SDL_GL_CreateContext(window);

    {
        std::string profile;
        switch (gl_context_profile)
        {
            case SDL_GL_CONTEXT_PROFILE_CORE:
                profile = "CORE";
                break;
            case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
                profile = "COMPATIBILITY";
                break;
            case SDL_GL_CONTEXT_PROFILE_ES:
                profile = "ES";
                break;

            default:
                profile = "none";
                break;
        }

        std::clog << "OpenGl " << gl_major_ver << '.' << gl_minor_ver << " "
                  << profile << '\n';
    }

    if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0)
    {
        std::clog << "error: failed to initialize glad" << std::endl;
    }

#ifdef DEBUG_more
    if (platform != "Mac OS X") // not supported on Mac
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        // glDebugMessageControl(
        //     GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(
            GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    if (SDL_GL_SetSwapInterval(0) != 0)
    {
        const char* error_message = SDL_GetError();
        std::cout << "error: failed call SDL_GL_SetSwapInterval: "
                  << error_message << std::endl;
    }
    else
    {
        std::cout << "call SDL_GL_SetSwapInterval: " << std::endl;
    }

    // Init ImGui
    initImGui(glsl_version);

    // Init Audio
    initAudio();

    glEnable(GL_BLEND);
    OM_GL_CHECK()
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    OM_GL_CHECK()

    glClearColor(0.f, 0.5f, 0.5f, 0.0f);
    OM_GL_CHECK()

    glViewport(0, 0, width, height);
    OM_GL_CHECK()

    return "";
}

void engine_impl::initImGui(const char* glsl_version)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    std::cout << "glsl_version: " << glsl_version << std::endl;

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // handle those errors in your application (e.g. use an assertion, or
    // display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and
    // stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame
    // below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 24.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // Init ImGui
}

void engine_impl::initAudio()
{

    // initialize audio
    audio_device_spec.freq     = 48000;
    audio_device_spec.format   = AUDIO_S16LSB;
    audio_device_spec.channels = 2;
    audio_device_spec.samples  = 1024; // must be power of 2
    audio_device_spec.callback = engine_impl::audio_callback;
    audio_device_spec.userdata = this;

    const int num_audio_drivers = SDL_GetNumAudioDrivers();

    // for (int i = 0; i < num_audio_drivers; ++i)
    // {
    //     std::cout << "audio_driver #:" << i << " " << SDL_GetAudioDriver(i)
    //               << '\n';
    // }
    std::cout << std::flush;

    const char* default_audio_device_name = nullptr;

    // SDL_FALSE - mean get only OUTPUT audio devices
    const int num_audio_devices = SDL_GetNumAudioDevices(SDL_FALSE);
    if (num_audio_devices > 0)
    {
        default_audio_device_name =
            SDL_GetAudioDeviceName(/*num_audio_devices - 1*/ 0, SDL_FALSE);
        for (int i = 0; i < num_audio_devices; ++i)
        {
            std::cout << "audio device #" << i << ": "
                      << SDL_GetAudioDeviceName(i, SDL_FALSE) << '\n';
        }
    }
    std::cout << std::flush;

    audio_device = SDL_OpenAudioDevice(default_audio_device_name,
                                       0,
                                       &audio_device_spec,
                                       nullptr,
                                       SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audio_device == 0)
    {
        std::cerr << "failed open audio device: " << SDL_GetError();
        throw std::runtime_error("audio failed");
    }
    else
    {
        std::cout << "--------------------------------------------\n";
        std::cout << "audio device selected: " << default_audio_device_name
                  << '\n'
                  << "freq: " << audio_device_spec.freq << '\n'
                  << "format: "
                  << get_sound_format_name(audio_device_spec.format) << '\n'
                  << "channels: "
                  << static_cast<uint32_t>(audio_device_spec.channels) << '\n'
                  << "samples: " << audio_device_spec.samples << '\n'
                  << std::flush;

        // unpause device
        SDL_PauseAudioDevice(audio_device, SDL_FALSE);
    }
}

void engine_impl::audio_callback(void*    engine_ptr,
                                 uint8_t* stream,
                                 int      stream_size)
{
    // no sound default
    SDL_memset(stream, 0, stream_size);
    // std::fill_n(stream, stream_size, '\0');

    engine_impl* e = static_cast<engine_impl*>(engine_ptr);

    for (SoundBuffer_impl* snd : e->sounds)
    {
        if (snd->is_playing)
        {
            uint32_t rest         = snd->length - snd->current_index;
            uint8_t* current_buff = &snd->buffer[snd->current_index];

            if (rest <= static_cast<uint32_t>(stream_size))
            {
                // copy rest to buffer
                SDL_MixAudioFormat(stream,
                                   current_buff,
                                   e->audio_device_spec.format,
                                   rest,
                                   SDL_MIX_MAXVOLUME);
                snd->current_index += rest;
            }
            else
            {
                SDL_MixAudioFormat(stream,
                                   current_buff,
                                   e->audio_device_spec.format,
                                   static_cast<uint32_t>(stream_size),
                                   SDL_MIX_MAXVOLUME);
                snd->current_index += static_cast<uint32_t>(stream_size);
            }

            if (snd->current_index == snd->length)
            {
                if (snd->is_looped)
                {
                    // start from begining
                    snd->current_index = 0;
                }
                else
                {
                    snd->is_playing = false;
                }
            }
        }
    }
}

float engine_impl::get_time_from_init()
{
    std::uint32_t ms_from_library_initialization = SDL_GetTicks();
    float         seconds = ms_from_library_initialization * 0.001f;
    return seconds;
}

// Some problem need fix
bool engine_impl::read_event(my_engine::event& ev)
{
    // collect all events from SDL
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event))
    {
        {
            const bind* binding = nullptr;

            if (sdl_event.type == SDL_QUIT)
            {
                ev.info      = my_engine::hardware_data{ true };
                ev.timestamp = sdl_event.common.timestamp * 0.001;
                ev.type      = my_engine::event_type::hardware;
                return true;
            }
            else if (sdl_event.type == SDL_KEYDOWN ||
                     sdl_event.type == SDL_KEYUP)
            {
                if (check_input(sdl_event, binding))
                {
                    bool is_down = sdl_event.type == SDL_KEYDOWN;
                    ev.info = my_engine::input_data{ binding->my_key, is_down };
                    ev.timestamp = sdl_event.common.timestamp * 0.001;
                    ev.type      = my_engine::event_type::input_key;
                    return true;
                }
            }
            else if (sdl_event.type == SDL_MOUSEMOTION)
            {
                ev.x    = sdl_event.motion.x;
                ev.y    = sdl_event.motion.y;
                ev.type = my_engine::event_type::mouse;
                return true;
            }
        }
    }
    return false;
}

bool engine_impl::is_key_down(const enum keys_type _key)
{
    const auto it = std::find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.my_key == _key;
    });

    if (it != end(keys))
    {
        const std::uint8_t* state         = SDL_GetKeyboardState(nullptr);
        int                 sdl_scan_code = SDL_GetScancodeFromKey(it->key);
        return state[sdl_scan_code];
    }
    return false;
}

void engine_impl::render(RenderObj& vao, Texture& tex)
{
    tex.bind();
    vao.setUniform(tex); // 0 - magic
    vao.draw();
}

void engine_impl::render(RenderObj& vao, Texture& tex, const matrix2x3& mat)
{
    vao.useProg();
    tex.bind();
    vao.setUniform(tex); // 0 - magic
    vao.setUniform("u_matrix", mat);
    vao.draw();
}

void engine_impl::renderImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    // (void)io;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).

    // if (show_demo_window)
    // ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to created a named window.
    {
        static float f       = 0.0f;
        static int   counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                       // and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can
                                                  // use a format strings too)
        ImGui::Checkbox("Demo Window",
                        &show_demo_window); // Edit bools storing our window
                                            // open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat(
            "float",
            &f,
            0.0f,
            1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3(
            "clear color",
            (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button(
                "Button")) // Buttons return true when clicked (most widgets
                           // return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("bullets = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin(
            "Another Window",
            &show_another_window); // Pass a pointer to our bool variable (the
                                   // window will have a closing button that
                                   // will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine_impl::swap_buffers()
{

    renderImGui();

    // time_point start = timer.now();
    SDL_GL_SwapWindow(window);
    // time_point end_last_frame = timer.now();

    // milli_sec frame_delta =
    //     std::chrono::duration_cast<milli_sec>(end_last_frame - start);
    // // std::cout << "frame_delta:\t" << frame_delta.count() << std::endl;

    glClear(GL_COLOR_BUFFER_BIT);
    OM_GL_CHECK()
}

SoundBuffer* engine_impl::create_sound_buffer(std::string_view path)
{
    SoundBuffer_impl* s =
        new SoundBuffer_impl(path, audio_device, audio_device_spec);
    SDL_LockAudioDevice(audio_device); // TODO fix lock only push_back
    sounds.push_back(s);
    SDL_UnlockAudioDevice(audio_device);
    return s;
}

void engine_impl::destroy_sound_buffer(SoundBuffer* sound)
{
    delete sound;
}

void engine_impl::uninitialize()
{
    for (auto&& it : sounds)
    {
        destroy_sound_buffer(it);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Create/destroy engine
static bool already_exist = false;

engine* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    engine* result = new engine_impl();
    already_exist  = true;
    return result;
}

void destroy_engine(engine* e)
{
    if (already_exist == false)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

RenderObj* create_RenderObj()
{
    RenderObj* result = new RenderObj_impl();
    // std::cout << &result << std::endl;
    return result;
}

void destroy_RenderObj(RenderObj* e)
{
    // std::cout << &e << std::endl;
    if (nullptr == e)
    {
        std::cerr << " can't delete RenderObj" << std::endl;
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

gfx_prog* create_gfx_prog(const std::string& path,
                          const std::string& name_shader)
{
    gfx_prog* result = new gfx_prog_impl(path, name_shader);
    return result;
}

void destroy_gfx_prog(gfx_prog* e)
{
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

engine::~engine()
{
    std::cout << "--- destor engine" << std::endl;
}

gfx_prog::~gfx_prog()
{
    std::cout << "--- destor gfx_prog" << std::endl;
}

RenderObj::~RenderObj()
{
    std::cout << "--- destor RenderObj" << std::endl;
}

SoundBuffer::~SoundBuffer()
{
    std::cout << "--- destor SoundBuffer" << std::endl;
}

game::~game()
{
    std::cout << "--- destor game" << std::endl;
}

// ERRORS
static const char* source_to_strv(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* type_to_strv(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* severity_to_strv(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
    }
    return "unknown";
}

// 30Kb on my system, too much for stack
static std::array<char, GL_MAX_DEBUG_MESSAGE_LENGTH> local_log_buff;

static void APIENTRY
callback_opengl_debug(GLenum                       source,
                      GLenum                       type,
                      GLuint                       id,
                      GLenum                       severity,
                      GLsizei                      length,
                      const GLchar*                message,
                      [[maybe_unused]] const void* userParam)
{
    // The memory formessageis owned and managed by the GL, and should onlybe
    // considered valid for the duration of the function call.The behavior of
    // calling any GL or window system function from within thecallback function
    // is undefined and may lead to program termination.Care must also be taken
    // in securing debug callbacks for use with asynchronousdebug output by
    // multi-threaded GL implementations.  Section 18.8 describes thisin further
    // detail.

    auto& buff{ local_log_buff };
    int   num_chars = std::snprintf(buff.data(),
                                  buff.size(),
                                  "%s %s %d %s %.*s\n",
                                  source_to_strv(source),
                                  type_to_strv(type),
                                  id,
                                  severity_to_strv(severity),
                                  length,
                                  message);

    if (num_chars > 0)
    {
        // TODO use https://en.cppreference.com/w/cpp/io/basic_osyncstream
        // to fix possible data races
        // now we use GL_DEBUG_OUTPUT_SYNCHRONOUS to garantie call in main
        // thread
        std::cerr.write(buff.data(), num_chars);
    }
}

} // namespace my_engine