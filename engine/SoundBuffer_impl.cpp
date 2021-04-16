#include "SoundBuffer_impl.hpp"


// static std::string_view get_sound_format_name(uint16_t format_value)
// {
//     static const std::map<int, std::string_view> format = {
//         { AUDIO_U8, "AUDIO_U8" },         { AUDIO_S8, "AUDIO_S8" },
//         { AUDIO_U16LSB, "AUDIO_U16LSB" }, { AUDIO_S16LSB, "AUDIO_S16LSB" },
//         { AUDIO_U16MSB, "AUDIO_U16MSB" }, { AUDIO_S16MSB, "AUDIO_S16MSB" },
//         { AUDIO_S32LSB, "AUDIO_S32LSB" }, { AUDIO_S32MSB, "AUDIO_S32MSB" },
//         { AUDIO_F32LSB, "AUDIO_F32LSB" }, { AUDIO_F32MSB, "AUDIO_F32MSB" },
//     };

//     auto it = format.find(format_value);
//     return it->second;
// }

// static std::size_t get_sound_format_size(uint16_t format_value)
// {
//     static const std::map<int, std::size_t> format = {
//         { AUDIO_U8, 1 },     { AUDIO_S8, 1 },     { AUDIO_U16LSB, 2 },
//         { AUDIO_S16LSB, 2 }, { AUDIO_U16MSB, 2 }, { AUDIO_S16MSB, 2 },
//         { AUDIO_S32LSB, 4 }, { AUDIO_S32MSB, 4 }, { AUDIO_F32LSB, 4 },
//         { AUDIO_F32MSB, 4 },
//     };

//     auto it = format.find(format_value);
//     return it->second;
// }

namespace my_engine
{

SoundBuffer_impl::SoundBuffer_impl(std::string_view  path,
                                   SDL_AudioDeviceID device_,
                                   SDL_AudioSpec     device_audio_spec)
    : device(device_)
{
    // SDL_RWops* file = SDL_RWFromFile(path.data(), "rb");
    // if (file == nullptr)
    // {
    //     throw std::runtime_error(std::string("error: can't load audio file ")
    //     +
    //                              path.data());
    // }

    // SDL_AudioSpec wav_spec;
    // if (SDL_LoadWAV_RW(file, 1, &wav_spec, &buffer, &length) == nullptr)
    // {

    //     throw std::runtime_error(std::string("error: can't load wav") +
    //                              path.data());
    // }

    SDL_AudioSpec file_audio_spec;
    if (SDL_LoadWAV(path.data(), &file_audio_spec, &buffer, &length) == NULL)
    {
        throw std::runtime_error(std::string("error: can't load wav ") +
                                 path.data());
    }
    // else
    // {
    //     /* Do stuff with the WAV data, and then... */
    //     SDL_FreeWAV(buffer);
    // }
    std::cout << "--------------------------------------------\n";
    std::cout << "audio format for: " << path << '\n'
              << "format: " << get_sound_format_name(file_audio_spec.format)
              << '\n'
              << "sample_size: "
              << get_sound_format_size(file_audio_spec.format) << '\n'
              << "channels: " << static_cast<uint32_t>(file_audio_spec.channels)
              << '\n'
              << "frequency: " << file_audio_spec.freq << '\n'
              << "length: " << length << '\n'
              << "time: "
              << static_cast<double>(length) /
                     (file_audio_spec.channels *
                      static_cast<uint32_t>(file_audio_spec.freq) *
                      get_sound_format_size(file_audio_spec.format))
              << "sec" << std::endl;
    std::cout << "--------------------------------------------\n";

    if (file_audio_spec.channels != device_audio_spec.channels ||
        file_audio_spec.format != device_audio_spec.format ||
        file_audio_spec.freq != device_audio_spec.freq)
    {
        SDL_AudioCVT cvt;
        SDL_BuildAudioCVT(&cvt,
                          file_audio_spec.format,
                          file_audio_spec.channels,
                          file_audio_spec.freq,
                          device_audio_spec.format,
                          device_audio_spec.channels,
                          device_audio_spec.freq);
        if (cvt.needed) // obviously, this one is always needed.
        {
            std::cout << "**********\nStart convert\n**********" << std::endl;
            // read your data into cvt.buf here.
            cvt.len = static_cast<int>(length);
            // we have to make buffer for inplace conversion
            size_t new_buffer_size =
                static_cast<size_t>(cvt.len * cvt.len_mult);
            tmp_buf.reset(new uint8_t[new_buffer_size]);
            uint8_t* buf_tmp = tmp_buf.get();
            // copy old buffer to new memory
            std::copy_n(buffer, length, buf_tmp);
            // cvt.buf has cvt.len_cvt bytes of converted data now.
            SDL_FreeWAV(buffer);
            cvt.buf = buf_tmp;
            if (0 != SDL_ConvertAudio(&cvt))
            {
                std::cout << "failed to convert audio from file: " << path
                          << " to audio device format" << std::endl;
            }

            buffer = tmp_buf.release();
            length = static_cast<uint32_t>(cvt.len_cvt);
        }
        else
        {
            // TODO no need to convert buffer, use as is
        }
    }

    std::cout << "+++ ctor SoundBuffer_impl" << std::endl;
}

SoundBuffer_impl::~SoundBuffer_impl()
{
    if (tmp_buf != nullptr)
    {
        SDL_FreeWAV(buffer);
        std::cout << "SDL_FreeWAV(buffer)" << std::endl;
    }
    buffer = nullptr;
    length = 0;

    std::cout << "--- destor SoundBuffer_impl" << std::endl;
}

void SoundBuffer_impl::play(const properties prop)
{ 
    // Lock callback function
    SDL_LockAudioDevice(device);

    // here we can change properties
    // of sound and dont collade with multithreaded playing
    current_index = 0;
    is_playing    = true;
    is_looped     = (prop == properties::looped);

    // unlock callback for continue mixing of audio
    SDL_UnlockAudioDevice(device);
}

bool SoundBuffer_impl::check_playing() 
{
    return is_playing;
}
} // namespace my_engine

