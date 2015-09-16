#include <cstdio>
#include <map>
#include <string>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

enum MainResult
{
    SUCCESS = 0,
    ERROR = 1
};

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <bag file>\n", argv[0]);
        return ERROR;
    }

    ros::init(argc, argv, "rosbag_freeze");

    ros::NodeHandle nh;

    std::string rosbag_fname(argv[1]);
    rosbag::Bag bag;

    try {
        bag.open(rosbag_fname, rosbag::bagmode::Read);
    }
    catch (const rosbag::BagException& ex) {
        fprintf(stderr, "Failed to open bag file for reading (%s)", ex.what());
        return ERROR;
    }

    rosbag::View view(bag); // view into entire bag

    // create a new latching publisher for each topic in the bag file
    std::map<std::string, ros::Publisher> publishers;
    for (const rosbag::ConnectionInfo* conn : view.getConnections()) {
        ros::AdvertiseOptions ops(conn->topic, 1, conn->md5sum, conn->datatype, conn->msg_def);
        ops.latch = true;
        publishers[conn->topic] = nh.advertise(ops);
    }

    std::map<std::string, rosbag::MessageInstance const> last_msgs;
    for (rosbag::MessageInstance const m : view) {
        last_msgs.insert(std::make_pair(m.getTopic(), m));
        // last_msgs[m.getTopic()] = m;
    }

    for (const auto& entry : last_msgs) {
        const std::string& topic = entry.first;
        const rosbag::MessageInstance& msg = entry.second;
        publishers[topic].publish(msg);
    }

    ros::spin();
    return SUCCESS;
}