#ifndef msg_utils_h
#define msg_utils_h

#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <Eigen/Dense>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/ColorRGBA.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <XmlRpc.h>

namespace msg_utils {

/// \brief Return whether a joint state contains a given set of joints
bool contains_joints(
    const sensor_msgs::JointState& joint_state,
    const std::vector<std::string>& joints);

/// \brief Return whether a joint state contains exactly a given set of joints
bool contains_only_joints(
    const sensor_msgs::JointState& joint_state,
    const std::vector<std::string>& joints);

/// \brief Reorder the joints in a joint state message.
/// \return false if the joint state contains joints other than those specified
///     in the joint order; true otherwise
bool reorder_joints(
    sensor_msgs::JointState& joint_state,
    const std::vector<std::string>& joint_order);

bool reorder_joints(
    trajectory_msgs::JointTrajectory& joint_trajectory,
    const std::vector<std::string>& joint_order);

int get_joint_index(
    const sensor_msgs::JointState& joint_state,
    const std::string& name);

visualization_msgs::Marker create_arrow_marker(const geometry_msgs::Vector3 &scale);
visualization_msgs::MarkerArray create_triad_marker_arr(const geometry_msgs::Vector3& scale);

Eigen::Affine3d interp(const Eigen::Affine3d& s, const Eigen::Affine3d& t, double alpha);
Eigen::Affine3d transform_diff(const Eigen::Affine3d& s, const Eigen::Affine3d& t);
void get_euler_ypr(const Eigen::Affine3d& transform, double& yaw, double& pitch, double& roll);

bool extract_xml_value(XmlRpc::XmlRpcValue& value, bool& bout);
bool extract_xml_value(XmlRpc::XmlRpcValue& value, int& iout);
bool extract_xml_value(XmlRpc::XmlRpcValue& value, double& dout);
bool extract_xml_value(XmlRpc::XmlRpcValue& value, std::string& sout);

template <typename T>
bool extract_xml_value(XmlRpc::XmlRpcValue& value, std::vector<T>& vout);

template <typename T>
bool extract_xml_value(XmlRpc::XmlRpcValue& value, std::map<std::string, T>& mout);

template <typename T>
bool download_param(const ros::NodeHandle& nh, const std::string& param_name, T& tout);

bool extract_xml_value(XmlRpc::XmlRpcValue& value, geometry_msgs::Point& p);
bool extract_xml_value(XmlRpc::XmlRpcValue& value, geometry_msgs::Quaternion& q);
bool extract_xml_value(XmlRpc::XmlRpcValue& value, geometry_msgs::Pose& p);

template <typename T, typename Op>
bool vector_pwiseop(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const Op& op);

template <typename T>
bool vector_sum(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv);

template <typename T, typename PlusOp>
bool vector_sum(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const PlusOp& op);

template <typename T>
bool vector_diff(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv);

template <typename T, typename MinusOp>
bool vector_diff(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const MinusOp& op);

template <typename T>
bool vector_mul(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv);

std::vector<double> to_degrees(const std::vector<double>& v);
std::vector<double> to_radians(const std::vector<double>& v);

void convert(const tf::Transform& from, Eigen::Affine3d& to);
void convert(const Eigen::Affine3d& from, tf::Transform& to);

////////////////////////////////////////////////////////////////////////////////
// Template Implementation
////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool extract_xml_value(XmlRpc::XmlRpcValue& value, std::vector<T>& vout)
{
    if (value.getType() != XmlRpc::XmlRpcValue::TypeArray) {
        ROS_WARN("Expected XML value of type array");
        return false;
    }

    // attempt to convert individual xmlrpc elements to values
    std::vector<T> t_array(value.size());
    for (std::size_t i = 0; i < t_array.size(); ++i) {
        if (!extract_xml_value(value[i], t_array[i])) {
            ROS_WARN("Failed to extract array element");
            return false;
        }
    }

    vout = std::move(t_array);
    return true;
}

template <typename T>
bool extract_xml_value(XmlRpc::XmlRpcValue& value, std::map<std::string, T>& mout)
{
    if (value.getType() != XmlRpc::XmlRpcValue::TypeStruct) {
        ROS_WARN("Expected XML value of type struct");
        return false;
    }

    // copy the map
    std::map<std::string, T> t_map;
    for (auto it = value.begin(); it != value.end(); ++it) {
        T t;
        if (!extract_xml_value(it->second, t)) {
            ROS_WARN("Failed to extract struct field");
            return false;
        }
        else {
            t_map[it->first] = t;
        }
    }

    mout = std::move(t_map);
    return true;
}

template <typename T>
bool download_param(const ros::NodeHandle& nh, const std::string& param_name, T& tout)
{
    ROS_DEBUG("Retrieving parameter %s", param_name.c_str());

    XmlRpc::XmlRpcValue value_array;
    if (!nh.getParam(param_name, value_array)) {
        ROS_WARN("Failed to retrieve param '%s' from the param server", param_name.c_str());
        return false;
    }

    return extract_xml_value(value_array, tout);
}

template <typename T, typename Op>
bool vector_pwiseop(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const Op& op)
{
    if (u.size() != v.size()) {
        return false;
    }

    uv.resize(u.size());
    for (std::size_t i = 0; i < u.size(); ++i) {
        uv[i] = op(u[i], v[i]);
    }

    return true;
}

template <typename T>
bool vector_sum(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv)
{
    return vector_pwiseop(u, v, uv, std::plus<T>());
}

template <typename T, typename PlusOp>
bool vector_sum(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const PlusOp& op)
{
    return vector_pwiseop(u, v, uv, op);
}

template <typename T>
bool vector_diff(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv)
{
    return vector_pwiseop(u, v, uv, std::minus<T>());
}

template <typename T, typename MinusOp>
bool vector_diff(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv, const MinusOp& op)
{
    return vector_pwiseop(u, v, uv, op);
}

template <typename T>
bool vector_mul(const std::vector<T>& u, const std::vector<T>& v, std::vector<T>& uv)
{
    return vector_pwiseop(u, v, uv, std::multiplies<T>());
}

} // namespace msg_utils

#endif
