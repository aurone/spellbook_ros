#ifndef spellbook_moveit_msgs_h
#define spellbook_moveit_msgs_h

#include <string>

#include <moveit_msgs/OrientedBoundingBox.h>

namespace moveit_msgs {

std::string to_string(const moveit_msgs::OrientedBoundingBox& bbx);

} // namespace moveit_msgs

#include "detail/moveit_msgs.h"

#endif
