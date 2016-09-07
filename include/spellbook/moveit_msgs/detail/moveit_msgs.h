#ifndef spellbook_detail_moveit_msgs_h
#define spellbook_detail_moveit_msgs_h

#include "../moveit_msgs.h"

#include <spellbook/geometry_msgs/geometry_msgs.h>

namespace moveit_msgs {

inline
std::string to_string(const moveit_msgs::OrientedBoundingBox& bbx)
{
    std::stringstream ss;
    ss << "{ pose: " << to_string(bbx.pose) << ", extents: " << to_string(bbx.extents) << " }";
    return ss.str();
}

} // namespace moveit_msgs

#endif
