#pragma once

#include "engine.hpp"

#include <SDL2/SDL_audio.h>
#include <memory>
#include <map>

static std::string_view get_sound_format_name(uint16_t format_value)
{
    static const std::map<int, std::string_view> format = {
        { AUDIO_U8, "AUDIO_U8" },         { AUDIO_S8, "AUDIO_S8" },
        { AUDIO_U16LSB, "AUDIO_U16LSB" }, { AUDIO_S16LSB, "AUDIO_S16LSB" },
        { AUDIO_U16MSB, "AUDIO_U16MSB" }, { AUDIO_S16MSB, "AUDIO_S16MSB" },
        { AUDIO_S32LSB, "AUDIO_S32LSB" }, { AUDIO_S32MSB, "AUDIO_S32MSB" },
        { AUDIO_F32LSB, "AUDIO_F32LSB" }, { AUDIO_F32MSB, "AUDIO_F32MSB" },
    };

    auto it = format.find(format_value);
    return it->second;
}

static std::size_t get_sound_format_size(uint16_t format_value)
{
    static const std::map<int, std::size_t> format = {
        { AUDIO_U8, 1 },     { AUDIO_S8, 1 },     { AUDIO_U16LSB, 2 },
        { AUDIO_S16LSB, 2 }, { AUDIO_U16MSB, 2 }, { AUDIO_S16MSB, 2 },
        { AUDIO_S32LSB, 4 }, { AUDIO_S32MSB, 4 }, { AUDIO_F32LSB, 4 },
        { AUDIO_F32MSB, 4 },
    };

    auto it = format.find(format_value);
    return it->second;
}

namespace my_engine
{

#pragma pack(push, 1)
class SoundBuffer_impl final : public SoundBuffer
{
public:
    SoundBuffer_impl(std::string_view  path,
                     SDL_AudioDeviceID device_,
                     SDL_AudioSpec     device_audio_spec);
    ~SoundBuffer_impl() final;

    void play(const properties) final;

// private:
    std::unique_ptr<uint8_t[]> tmp_buf;
    uint8_t*                   buffer = nullptr;
    uint32_t                   length = 0;
    uint32_t                   current_index = 0;
    SDL_AudioDeviceID          device;
    bool                       is_playing = false;
    bool                       is_looped  = false;
};
#pragma pack(pop)

} // namespace my_engine