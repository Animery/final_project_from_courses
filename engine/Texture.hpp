#pragma once

#include "Image.hpp"
#include <string>

#define GL_CLAMP_TO_BORDER 0x812D
#define GL_LINEAR 0x2601
#define GL_REPEAT 0x2901
#define GL_NEAREST 0x2600

typedef unsigned int GLuint;
namespace Animate
{

class Image;

class Texture
{
private:
    GLuint      mHandle;
    std::string name_uniform = "";
    // unsigned    id_tex;

public:
    Texture(std::string& name_u, int wrap, int filter);
    ~Texture();
    Texture(const Texture&) = delete;

    void bind() const;
    void unBind() const;
    void setImage(Image& image);
    void loadImage(std::string_view path);

    std::string getName() const { return name_uniform; }
    GLuint      getTexID() { return mHandle; }
};
} // namespace Animate