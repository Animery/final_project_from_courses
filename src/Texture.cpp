#include "../include/Texture.hpp"
#include "glad/glad.h"
#include <iostream>

Texture::Texture(std::string& name_u)
{
    static unsigned number_tex = 0;
    id_tex                     = number_tex;
    std::cout << "id_tex:\t" << id_tex << std::endl;

    number_tex++;
    glGenTextures(1, &mHandle);
    bind();
    name_uniform = name_u;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void Texture::setImage(const Image& image)
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
