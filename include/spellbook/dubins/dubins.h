#ifndef au_dubins_h
#define au_dubins_h

// standard includes
#include <ostream>
#include <vector>

// project includes
#include <spellbook/poses/pose2.h>

namespace au {

enum class AngleDir { CW, CCW };
const char* to_cstring(AngleDir dir);
std::ostream& operator<<(std::ostream& o, AngleDir dir);

struct dubins_path
{
    pose2d      start;  ///< The start (x, y, yaw) pose
    pose2d      goal;   ///< The goal (x, y, yaw) pose
    double      radius; ///< The radius, in meters, of both turns
    double      arc1;   ///< Length, in radians, of the first turn
    double      arc2;   ///< Length, in radians, of the second turn
    AngleDir    dir1;   ///< The direction of the first turn
    AngleDir    dir2;   ///< The direction of the second turn

    dubins_path();
    dubins_path(
        const pose2d& start, const pose2d& goal, double radius_m,
        double arc1, double arc2,
        AngleDir dir1, AngleDir dir2);

    dubins_path(const std::vector<pose2d>& path) { throw "not implemented"; }

    vec2d pivot1() const;
    vec2d pivot2() const;

    pose2d straight_start() const;
    pose2d straight_end() const;

    double t0() const { return 0.0; }
    double t1() const;
    double t2() const;
    double t3() const { return 1.0; }

    double length() const;

    pose2d at(double t) const;
};

double length(const dubins_path& p);

std::ostream& operator<<(std::ostream& o, const dubins_path& p);

typedef std::vector<dubins_path> dubins_paths;

dubins_paths generate_dubins_paths(
    const pose2d& start,
    const pose2d& goal,
    double turning_radius_m);

} // namespace au

#endif
