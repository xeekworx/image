![Xeekworx](http://xeekworx.com/images/github/xeekworx_logo.png)
xeekworx::image
===========
**xeekworx::image is a single header/source class that you can easily include with your C++ project that abstracts software image manipulation. The STB library is required, but it's not a compiled library.**

Note: C++11 is required. If you're using Visual Studio 2015, make sure you're on at least Update 2.<br/><br/>
First, you must include *"image.h"*. The class itself is within the xeekworx namespace, so keep that in mind as you're reading the documentation.

CONSTRUCTORS
------------
You can construct an image object one of three ways:

 1. **image(const byte * pixels, const uint32_t width, const uint32_t height, const uint32_t bits)**
- Create an image object with a byte pixel buffer. Bits can be 24 or 32, but 32 is default. If a 24-bit buffer is given, it will be converted to 32-bits for this object.
 2. **image(const uint32_t width, const uint32_t height)**
- Create a blank image object with the given width and height. The pixels are initialized to 0,0,0,0.
 3. **image(const std::string& file)**
- Create an image object by loading an image file. This can be JPEG, PNG, BMP, PSD, TGA, GIF, HDR, PIC, PPM, and PGM.

LOADING & SAVING MEMBER FUNCTIONS:
------------
**void load(const std::string& file)**<br/>
DESCRIPTION: Load an image from a file. The support formats are JPEG, PNG, BMP, PSD, TGA, GIF, HDR, PIC, PPM, and PGM.<br/>
PARAMETERS:

- *const std::string& file* - The path to the image file to load. The type of image will be determined by the header information or file extension.

**void save(const std::string& file, const file_format format = file_format::png) const**<br/>
DESCRIPTION: Save an image to a file. The supported formats are PNG, BMP, and TGA.<br/>
PARAMETERS:

- *const std::string& file* - The path to the image file to save.
- *const file_format format* - one of the file_format constants. This can be file_format::PNG, file_format::BMP or file_format::TGA.

INFORMATIONAL MEMBER FUNCTIONS:
------------
**const xeekworx::byte * pixels() const**<br/>
DESCRIPTION: Returns byte buffer representing the pixels of the image. "byte" is an alias for uint8_t or unsigned char in most<br/> implementations.

**const uint32_t& width() const**<br/>
DESCRIPTION: Returns the pixel width of the image.<br/>

**const uint32_t& height() const**<br/>
DESCRIPTION: Returns the pixel height of the image.<br/>

**const uint32_t bits() const**<br/>
DESCRIPTION: Returns the bit depth of the image. This will only ever be 32 since creating an image object with a 24-bit buffer will cause it to convert it to 32-bit.<br/>

**const uint32_t channels() const**<br/>
DESCRIPTION: Returns the number of color channels in this image. This will only ever be 4 since the image is always 32-bit.<br/>

**const uint32_t pitch() const**<br/>
DESCRIPTION: Returns the byte pitch of the image. This is calculated by multiplying the width and number of color channels.<br/>

**uint32_t length() const**<br/>
DESCRIPTION: Returns the total byte length of the image pixel buffer.<br/>

**bool empty() const**<br/>
DESCRIPTION: Returns true if the image is empty (no pixels, width, or height).<br/>

MANIPULATION MEMBER FUNCTIONS:
------------
**void reset()**<br/>
DESCRIPTION: The image is reset to empty. The pixel buffer will be cleared and set to a length of 0.<br/>

**void flip(const uint32_t type)**<br/>
DESCRIPTION: Flips the image vertically and/or horizontally (mirror).<br/>
PARAMETERS:

- *const uint32_t type* - One or more of the flip_type flags. This can be flip_type::vertical and/or flip_type::horizontal.

**void clear(const uint32_t color)**<br/>
DESCRIPTION: Clears the image buffer to a specific color, transparent black being the default.<br/>
PARAMETERS:

- *const uint32_t color* - A hexadecimal color to clear the image. eg. 0xFFFFFFFF (opaque white).

**void resize(const uint32_t new_width, const uint32_t new_height)**<br/>
DESCRIPTION: Modify the canvas size of the image. This does not stretch or scale.<br/>
PARAMETERS:

- *const uint32_t width* - The new width of the image.
- *const uint32_t height* - The new height of the image.

OPERATORS:
------------
**byte& operator\[\](const int i)**<br/>
DESCRIPTION: Returns a reference to the pixel at index i.<br/>
PARAMETERS:

- *const int i* - The index of the pixel to return.

**const byte& operator\[\](const int i) const**<br/>
DESCRIPTION: Returns a const reference to the pixel at index i.<br/>
PARAMETERS:
*const int i* - The index of the pixel to return.
