#include "../include/Image.hpp"
#include "../include/picopng.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "../include/stb_image.h"
namespace Animate
{
    


Image::~Image()
{

    std::cout << "--- destor Image" << std::endl;
}

// Image Image::loadFromFile1(const std::string& path)
// {

//     stbi_set_flip_vertically_on_load(true);
//     int            width      = 0;
//     int            height     = 0;
//     int            components = 0;
//     unsigned char* decoded_img =
//         stbi_load(path.data(), &width, &height, &components, 4);

//     // if there's an error, display it
//     if (decoded_img == nullptr)
//     {
//         std::cerr << "error: can't load file: " << path << std::endl;
//         throw std::runtime_error("can't load texture");
//     }

//     std::cout << "image size: "
//               << "\twidth: " << width << "\theight: " << height << std::endl;
//     return Image(width, height, decoded_img);
// }

Image Image::loadFromFile(const std::string& path)
{
    std::vector<std::byte> png_file_in_memory;
    std::ifstream          ifs(path.data(), std::ios_base::binary);
    if (!ifs)
    {
        throw std::runtime_error("could not load image: " + path);
    }
    ifs.seekg(0, std::ios_base::end);
    size_t pos_in_file = static_cast<size_t>(ifs.tellg());
    png_file_in_memory.resize(pos_in_file);
    ifs.seekg(0, std::ios_base::beg);
    if (!ifs)
    {
        throw std::runtime_error("could not load image: " + path);
    }

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
             static_cast<std::streamsize>(png_file_in_memory.size()));
    if (!ifs.good())
    {
        throw std::runtime_error("could not load image: " + path);
    }

    std::vector<std::byte> image;
    unsigned long          w     = 0;
    unsigned long          h     = 0;
    int                    error = decodePNG(
        image, w, h, &png_file_in_memory[0], png_file_in_memory.size(), false);

    // if there's an error, display it
    if (error != 0)
    {
        throw std::runtime_error("could not load image: " + path + " " +
                                 std::to_string(error));
        std::cout << "could not load image: " << std::endl;
    }

    std::cout << "image size: " << image.size() << " byte"
              << "\nw: " << w << "\nh: " << h << std::endl;
    return Image(w, h, image);
}

Image::Image(uint                          in_width,
             uint                          in_height,
             const std::vector<std::byte>& in_data
             /*, Format in_format*/)
    : width(in_width)
    , height(in_height)
    , data(in_data)
/*, format(in_format)*/
{
    std::cout << "+++ ctor Image" << std::endl;
}
} // namespace Animate