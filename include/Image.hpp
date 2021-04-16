#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace Animate
{
	

class Image
{
// public:
//     enum Format
//     {
//         RGB,
//         RGBA
//     };

private:
    uint            width;
    uint            height;
    std::vector<std::byte> data;
    // Format            format;

public:
    Image(uint                 in_width,
          uint                 in_height,
          const std::vector<std::byte>& in_data/*,
          Format                   in_format*/);
    ~Image();

    uint getWidth() const
	{
		return width;
	}

	uint getHeight() const
	{
		return height;
	}

	const std::vector<std::byte>& getData() const
	{
		return data;
	}

	// Format getFormat() const
	// {
	// 	return format;
	// }

    static Image loadFromFile(const std::string& path);
};
} // namespace Animate