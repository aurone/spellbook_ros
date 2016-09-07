#ifndef spellbook_detail_geometry_msgs_h
#define spellbook_detail_geometry_msgs_h

#include "../geometry_msgs.h"

#include <spellbook/std_msgs/std_msgs.h>

namespace geometry_msgs {

inline
Vector3 CreateVector3(double x, double y, double z)
{
    Vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

inline
const Vector3 ZeroVector3()
{
    return CreateVector3(0.0, 0.0, 0.0);
}

inline
Point CreatePoint(double x, double y, double z)
{
    Point p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

inline
const Point ZeroPoint()
{
    return CreatePoint(0.0, 0.0, 0.0);
}

inline
Point32 CreatePoint32(float x, float y, float z)
{
    Point32 p;
    p.x = x;
    p.y = y;
    p.z = z;
    return p;
}

inline
const Point32 ZeroPoint32()
{
    return CreatePoint32(0.0f, 0.0f, 0.0f);
}

inline
Quaternion CreateQuaternion(double w, double x, double y, double z)
{
    Quaternion q;
    q.w = w;
    q.x = x;
    q.y = y;
    q.z = z;
    return q;
}

inline
const Quaternion IdentityQuaternion()
{
    return CreateQuaternion(1.0, 0.0, 0.0, 0.0);
}

inline
Pose CreatePose(const Point& position, const Quaternion& orientation)
{
    Pose p;
    p.position = position;
    p.orientation = orientation;
    return p;
}

inline
const Pose IdentityPose()
{
    return CreatePose(ZeroPoint(), IdentityQuaternion());
}

inline
std::string to_string(const Quaternion& quat)
{
    std::stringstream ss;
    ss << "(" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")";
    return ss.str();
}

inline
std::string to_string(const Point& point)
{
    std::stringstream ss;
    ss << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return ss.str();
}

inline
std::string to_string(const Point32& point)
{
    std::stringstream ss;
    ss << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return ss.str();
}

inline
std::string to_string(const Pose& pose)
{
    std::stringstream ss;
    ss << "{ position: " << to_string(pose.position) << ", " << "orientation: " << to_string(pose.orientation) << " }";
    return ss.str();
}

inline
std::string to_string(const PoseStamped& pose)
{
    std::stringstream ss;
    ss << "{ header: " << to_string(pose.header) << ", pose: " << to_string(pose.pose) << "}";
    return ss.str();
}

} // namespace geometry_msgs

#endif
