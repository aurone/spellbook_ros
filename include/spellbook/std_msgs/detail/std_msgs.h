#ifndef spellbook_detail_std_msgs_h
#define spellbook_detail_std_msgs_h

#include "../std_msgs.h"

namespace std_msgs {

inline
ColorRGBA CreateColorRGBA(float r, float g, float b, float a)
{
    ColorRGBA color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

inline
const ColorRGBA BlackColorRGBA(float a)
{
    return CreateColorRGBA(0.0f, 0.0f, 0.0f, a);
}

inline
const ColorRGBA RedColorRGBA(float a)
{
    return CreateColorRGBA(1.0f, 0.0f, 0.0f, a);
}

inline
const ColorRGBA GreenColorRGBA(float a)
{
    return CreateColorRGBA(0.0f, 1.0f, 0.0f, a);
}

inline
const ColorRGBA BlueColorRGBA(float a)
{
    return CreateColorRGBA(0.0f, 0.0f, 1.0f, a);
}

inline
const ColorRGBA YellowColorRGBA(float a)
{
    return CreateColorRGBA(1.0f, 1.0f, 0.0f, a);
}

inline
const ColorRGBA CyanColorRGBA(float a)
{
    return CreateColorRGBA(0.0f, 1.0f, 1.0f, a);
}

inline
const ColorRGBA MagentaColorRGBA(float a)
{
    return CreateColorRGBA(1.0f, 0.0f, 1.0f, a);
}

inline
const ColorRGBA WhiteColorRGBA(float a)
{
    return CreateColorRGBA(1.0f, 1.0f, 1.0f, a);
}

inline
Header CreateHeader(
    uint32_t seq,
    const ros::Time& stamp,
    const std::string& frame_id)
{
    Header header;
    header.seq = seq;
    header.stamp = stamp;
    header.frame_id = frame_id;
    return header;
}

} // namespace std_msgs

#endif
