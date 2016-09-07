#ifndef spellbook_geometry_msgs_h
#define spellbook_geometry_msgs_h

#include <string>

#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>

namespace geometry_msgs {

Vector3 CreateVector3(double x, double y, double z);
const Vector3 ZeroVector3();

Point CreatePoint(double x, double y, double z);
const Point ZeroPoint();

Point32 CreatePoint32(float x, float y, float z);
const Point32 ZeroPoint32();

Quaternion CreateQuaternion(double w, double x, double y, double z);
const Quaternion IdentityQuaternion();

Pose CreatePose(const Point& position, const Quaternion& orientation);
const Pose IdentityPose();

std::string to_string(const Quaternion& quat);
std::string to_string(const Point& point);
std::string to_string(const Point32& point);
std::string to_string(const Pose& pose);
std::string to_string(const PoseStamped& pose);

} // namespace geometry_msgs

#include "detail/geometry_msgs.h"

#endif
