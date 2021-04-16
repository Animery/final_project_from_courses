#include "../include/Texture.hpp"
#include "glad/glad.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

namespace Animate
{
    

Texture::Texture(std::string& name_u, int wrap, int filter)
{
    // static unsigned number_tex = 0;
    // id_tex                     = number_tex;
    // std::cout << "id_tex:\t" << id_tex << std::endl;
    name_uniform = name_u;

    // number_tex++;
    glGenTextures(1, &mHandle);
    bind();
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_LINEAR 0x2601
#define GL_REPEAT 0x2901
#define GL_NEAREST 0x2600

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    unBind();
    std::cout << "+++ ctor Texture" << std::endl;
}

Texture::~Texture()
{
    glDeleteTextures(1, &mHandle);
    std::cout << "--- destor Texture" << std::endl;
}

void Texture::bind() const
{
    // int texture_unit = static_cast<int>(mHandle);
    // std::cout << "texture_unit: \t" << texture_unit << std::endl;
    // glActiveTexture(GL_TEXTURE0 + texture_unit - 1);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mHandle);
}

void Texture::unBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setImage(Image& image)
{
    bind();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image.getWidth(),
                 image.getHeight(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image.getData().data());
    unBind();
}

void Texture::loadImage(std::string_view path)
{
    bind();
    stbi_set_flip_vertically_on_load(true);
    int            width      = 0;
    int            height     = 0;
    int            components = 0;
    unsigned char* decoded_img =
        stbi_load(path.data(), &width, &height, &components, 4);

    // if there's an error, display it
    if (decoded_img == nullptr)
    {
        std::cerr << "error: can't load file: " << path << std::endl;
        throw std::runtime_error("can't load texture");
    }

    std::cout << "image size: " << sizeof(decoded_img) * 4 * width * height << " byte"
              << "\twidth: " << width << "\theight: " << height << std::endl;

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 decoded_img);
    stbi_image_free(decoded_img);
    unBind();
}

} // namespace Animate