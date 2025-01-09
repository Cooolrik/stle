## bitmap_font.h

`bitmap_font.h` provides functionality for rendering a fixed-width bitmap font. It includes functions to get glyphs for characters and render text to an output pixel buffer. The font used is a fixed 9x16 font, and the code supports rendering with various flags for customization.

### Caveats

The font renderer an ultralight-weight font render, which only supports the [code page 437](https://en.wikipedia.org/wiki/Code_page_437) (aka DOS Latin US) character set.

### Example Usage

```cpp
#include "bitmap_font.h"
#include <iostream>
#include <vector>

int main() 
{
    // Define the pixel type and create an output buffer
    using pixel_t = uint8_t;
    const uint32_t width = 100;
    const uint32_t height = 50;
    std::vector<pixel_t> output(width * height, 0);

    // Render text to the output buffer
    const char *text = "Hello, World!";
    pixel_t pixel_on = 1;
    pixel_t pixel_off = 0;
    ctle::render_bitmap_font(
		output.data(), 
		width, 
		10, 10, 
		text, 
		pixel_on, pixel_off, 
		ctle::bitmap_font_flags::center_horizontally
		);

    // Print the output buffer (for demonstration purposes)
    for (uint32_t y = 0; y < height; ++y) 
	{
        for (uint32_t x = 0; x < width; ++x) 
		{
            std::cout << (output[y * width + x] ? '#' : ' ');
        }
        std::cout << '\n';
    }

    return 0;
}```