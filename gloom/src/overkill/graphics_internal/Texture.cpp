#include "Texture.hpp"
namespace OK
{
TextureBase::TextureBase(
    uint16_t width, 
    uint16_t height, 
    uint16_t channels
) : width{width},
    height{height},
    channels{channels}
{}

Texture2DArray::Texture2DArray(
    uint16_t width, 
    uint16_t height, 
    uint16_t depth, 
    uint16_t channels
) :	Texture(width, height, channels), 
    depth{ depth }
{}
}