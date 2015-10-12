#include <spellbook/costmap_extruder/CostmapExtruder.h>

// #include <leatherman/print.h>
#include <octomap_msgs/conversions.h>
#include <octomap_ros/conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <spellbook/stringifier/stringifier.h>
#include <spellbook/msg_utils/msg_utils.h>

CostmapExtruder::CostmapExtruder(
    std::int8_t obs_threshold,
    bool unknown_obstacles)
:
    unknown_obstacles_(unknown_obstacles),
    obs_threshold_(obs_threshold)
{
}

bool CostmapExtruder::extrude(
    const nav_msgs::OccupancyGrid& grid,
    double extrusion,
    octomap_msgs::Octomap& map)
{
    moveit_msgs::CollisionObject collision_map =
            extrude_to_collision_map(grid, extrusion);

    // convert the collision map to a point cloud
    ROS_INFO("Creating Octomap from Collision Object");
    ROS_INFO("  %zd solid primitives (assumed boxes)", collision_map.primitives.size());

    const shape_msgs::SolidPrimitive& first_box =
            collision_map.primitives.front();
    assert(!first_box.dimensions.empty());
    const double res = first_box.dimensions[0];
    ROS_INFO("  Resolution: %0.3f", res);

    octomap::Pointcloud octomap_cloud;
    convert(collision_map, octomap_cloud);

    octomap::OcTree octree(res);
    octree.insertScan(octomap_cloud, octomap::point3d(0.0, 0.0, 0.0));

    log_octomap(octree);

    octomap_msgs::Octomap octomap;

    if (!octomap_msgs::fullMapToMsg(octree, octomap)) {
        ROS_ERROR("Failed to convert OcTree to octomap_msgs/Octomap");
        return false;
    }

    octomap.header = collision_map.header;
    map = octomap;
    return true;
}

bool CostmapExtruder::extrude(
    const nav_msgs::OccupancyGrid& grid,
    double extrusion,
    pcl::PointCloud<pcl::PointXYZI>& cloud)
{
    // create 3-d collision map via extrusion
    moveit_msgs::CollisionObject collision_map =
            extrude_to_collision_map(grid, extrusion);

    // convert the collision map to a point cloud
    ROS_INFO("Creating Octomap from Collision Map");
    ROS_INFO("  %zd solid primitives (assumed boxes)", collision_map.primitives.size());

    const shape_msgs::SolidPrimitive& first_box =
            collision_map.primitives.front();
    assert(!first_box.dimensions.empty());
    const double res = first_box.dimensions[0]; // note: cubic cell assumption
    ROS_INFO("  Resolution: %0.3f", res);

    // inherit frame_id and stamp...and seq
    cloud.header = pcl_conversions::toPCL(grid.header); 

    cloud.reserve(collision_map.primitives.size());

    for (const geometry_msgs::Pose& pose : collision_map.primitive_poses) {
        pcl::PointXYZI point;
        point.x = pose.position.x;
        point.y = pose.position.y;
        point.z = pose.position.z;
        point.intensity = 100;
        cloud.push_back(point);
    }

    return true;
}

moveit_msgs::CollisionObject
CostmapExtruder::extrude_to_collision_map(
    const nav_msgs::OccupancyGrid& grid,
    double extrusion) const
{
    ROS_INFO("Extruding Occupancy Grid");

    moveit_msgs::CollisionObject cmap;

    cmap.header.seq = 0;
    cmap.header.stamp = ros::Time(0);
    cmap.header.frame_id = grid.header.frame_id;

    const std::uint32_t width = grid.info.width;
    const std::uint32_t height = grid.info.height;
    const float res = grid.info.resolution;

    int num_occupied_cells = 0;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // only add occuped cells to the collision map
            bool obstacle =
                (grid.data[y * width + x] >= obs_threshold_) ||
                (unknown_obstacles_ && grid.data[y * width + x] < 0);

            if (!obstacle) {
                continue;
            }

            ++num_occupied_cells;

            double map_x = grid.info.origin.position.x + x * res + 0.5 * res;
            double map_y = grid.info.origin.position.y + y * res + 0.5 * res;
            for (double z = 0.0; z <= extrusion; z += res) {
                const double& map_z = z + 0.5 * res;

                // construct
                geometry_msgs::Point32 box_extents(geometry_msgs::CreatePoint32(res, res, res));
                shape_msgs::SolidPrimitive sp;
                sp.type = shape_msgs::SolidPrimitive::BOX;
                sp.dimensions.resize(3);
                sp.dimensions[shape_msgs::SolidPrimitive::BOX_X] = box_extents.x;
                sp.dimensions[shape_msgs::SolidPrimitive::BOX_Y] = box_extents.y;
                sp.dimensions[shape_msgs::SolidPrimitive::BOX_Z] = box_extents.z;
                cmap.primitives.push_back(sp);

                geometry_msgs::Point box_pos(geometry_msgs::CreatePoint(map_x, map_y, map_z));
                geometry_msgs::Quaternion box_rot(geometry_msgs::IdentityQuaternion());
                geometry_msgs::Pose box_pose(geometry_msgs::CreatePose(box_pos, box_rot));
                cmap.primitive_poses.push_back(box_pose);
            }
        }
    }

    ROS_INFO("Extruded %d occupied cells", num_occupied_cells);

    moveit_msgs::OrientedBoundingBox bbx = get_bbx(cmap);
    ROS_INFO("  Collision Map Bounding Box: %s", to_string(bbx).c_str());

    return cmap;
}

moveit_msgs::OrientedBoundingBox
CostmapExtruder::get_bbx(
    const moveit_msgs::CollisionObject& collision_map) const
{
    moveit_msgs::OrientedBoundingBox bbx;

    if (collision_map.primitives.empty()) {
        bbx.extents = geometry_msgs::CreatePoint32(0.0, 0.0, 0.0);
        bbx.pose = geometry_msgs::IdentityPose();
        return bbx;
    }

    assert(!collision_map.primitive_poses.empty());

    const geometry_msgs::Pose& first_pose =
            collision_map.primitive_poses.front();

    double min_x = first_pose.position.x;
    double min_y = first_pose.position.y;
    double min_z = first_pose.position.z;

    double max_x = first_pose.position.x;
    double max_y = first_pose.position.y;
    double max_z = first_pose.position.z;

    for (const geometry_msgs::Pose& pose : collision_map.primitive_poses) {
        if (pose.position.x < min_x) {
            min_x = pose.position.x;
        }
        if (pose.position.y < min_y) {
            min_y = pose.position.y;
        }
        if (pose.position.z < min_z) {
            min_z = pose.position.z;
        }

        if (pose.position.x > max_x) {
            max_x = pose.position.x;
        }
        if (pose.position.y > max_y) {
            max_y = pose.position.y;
        }
        if (pose.position.z > max_z) {
            max_z = pose.position.z;
        }
    }

    bbx.extents.x = max_x - min_x;
    bbx.extents.y = max_y - min_y;
    bbx.extents.z = max_z - min_z;
    bbx.pose.position.x = 0.5 * (max_x + min_x);
    bbx.pose.position.y = 0.5 * (max_y + min_y);
    bbx.pose.position.z = 0.5 * (max_z + min_z);
    bbx.pose.orientation = geometry_msgs::IdentityQuaternion();

    return bbx;
}

void CostmapExtruder::log_octomap(const octomap::OcTree& octree) const
{
    size_t num_nodes = octree.calcNumNodes();
    ROS_INFO("  Num Nodes: %zd", num_nodes);
    ROS_INFO("  Memory Usage: %zd bytes", octree.memoryUsage());
    ROS_INFO("  Num Leaf Nodes: %zd", octree.getNumLeafNodes());

    // TODO: where has this function gone?
//    unsigned num_thresholded, num_other;
//    octree.calcNumThresholdedNodes(num_thresholded, num_other);
//    ROS_INFO("  Num Thresholded Nodes: %u", num_thresholded);
//    ROS_INFO("  Num Other Nodes: %u", num_other);

    const octomap::point3d octomap_min = octree.getBBXMin();
    const octomap::point3d octomap_max = octree.getBBXMax();
    const octomap::point3d octomap_center = octree.getBBXCenter();
    double clamping_thresh_min = octree.getClampingThresMin();
    double clamping_thresh_max = octree.getClampingThresMax();

    ROS_INFO("  Bounding Box Set: %s", octree.bbxSet() ? "TRUE" : "FALSE");
    ROS_INFO("  Bounding Box Min: (%0.3f, %0.3f, %0.3f)", octomap_min.x(), octomap_min.y(), octomap_min.z());
    ROS_INFO("  Bounding Box Max: (%0.3f, %0.3f, %0.3f)", octomap_max.x(), octomap_max.y(), octomap_max.z());
    ROS_INFO("  Bounding Box Center: (%0.3f, %0.3f, %0.3f)", octomap_center.x(), octomap_center.y(), octomap_center.z());
    ROS_INFO("  Clamping Threshold Min: %0.3f", clamping_thresh_min);
    ROS_INFO("  Clamping Threshold Max: %0.3f", clamping_thresh_max);

    double metric_min_x, metric_min_y, metric_min_z;
    double metric_max_x, metric_max_y, metric_max_z;
    double metric_size_x, metric_size_y, metric_size_z;
    octree.getMetricMin(metric_min_x, metric_min_y, metric_min_z);
    octree.getMetricMax(metric_max_x, metric_max_y, metric_max_z);

    ROS_INFO("  Metric Min: (%0.3f, %0.3f, %0.3f)", metric_min_x, metric_min_y, metric_min_z);
    ROS_INFO("  Metric Max: (%0.3f, %0.3f, %0.3f)", metric_max_x, metric_max_y, metric_max_z);

    octree.getMetricSize(metric_size_x, metric_size_y, metric_size_z);
    ROS_INFO("  Metric Size: (%0.3f, %0.3f, %0.3f)", metric_size_x, metric_size_y, metric_size_z);

    ROS_INFO("  Node Size (max depth): %0.6f", octree.getNodeSize(octree.getTreeDepth()));
    ROS_INFO("  Occupancy Threshold: %0.3f", octree.getOccupancyThres());
    ROS_INFO("  Probability Hit: %0.3f", octree.getProbHit());
    ROS_INFO("  Probability Miss: %0.3f", octree.getProbMiss());
    ROS_INFO("  Resolution: %0.3f", octree.getResolution());
    ROS_INFO("  Depth: %u", octree.getTreeDepth());
    ROS_INFO("  Tree Type: %s", octree.getTreeType().c_str());
}

void CostmapExtruder::convert(
    const moveit_msgs::CollisionObject& cmap,
    octomap::Pointcloud& octomap_cloud) const
{
    octomap_cloud.clear();
    octomap_cloud.reserve(cmap.primitives.size());
    assert(cmap.primitives.size() == cmap.primitive_poses.size());
    for (size_t i = 0; i < cmap.primitives.size(); ++i)
    {
        const geometry_msgs::Pose& pose = cmap.primitive_poses[i];
        octomap_cloud.push_back(
                pose.position.x, pose.position.y, pose.position.z);
    }
}
