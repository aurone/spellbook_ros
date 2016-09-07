#ifndef spellbook_std_msgs_h
#define spellbook_std_msgs_h

#include <string>

#include <std_msgs/Header.h>
#include <std_msgs/ColorRGBA.h>

namespace std_msgs {

ColorRGBA CreateColorRGBA(float r, float g, float b, float a);

const ColorRGBA BlackColorRGBA(float a = 1.0f);
const ColorRGBA RedColorRGBA(float a = 1.0f);
const ColorRGBA GreenColorRGBA(float a = 1.0f);
const ColorRGBA BlueColorRGBA(float a = 1.0f);
const ColorRGBA YellowColorRGBA(float a = 1.0f);
const ColorRGBA CyanColorRGBA(float a = 1.0f);
const ColorRGBA MagentaColorRGBA(float a = 1.0f);
const ColorRGBA WhiteColorRGBA(float a = 1.0f);

Header CreateHeader(
    uint32_t seq,
    const ros::Time& stamp,
    const std::string& frame_id);

std::string to_string(const ColorRGBA& color);
std::string to_string(const Header& header);

} // namespace std_msgs

#include "detail/std_msgs.h"

#endif
