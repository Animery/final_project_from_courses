#pragma once

#include "Image.hpp"
#include <string>

typedef unsigned int GLuint;

class Texture
{
private:
    GLuint      mHandle;
    std::string name_uniform = "";
    unsigned    id_tex;

public:
    Texture(std::string&);
    ~Texture();
    Texture(const Texture&) = delete;

    void bind() const;
    void unBind() const;
    void setImage(const Image& image);

    std::string getName() const { return name_uniform; }
    GLuint      getTexID() { return mHandle; }
};