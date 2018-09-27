#include <spellbook/dubins/dubins.h>

// standard includes
#include <limits>

// project includes
#include <smpl/angles.h>
#include <spellbook/geometry/vector/vec2.h>
#include <spellbook/math/math.h>

#define DUBINS_DEBUG 0
#if DUBINS_DEBUG
#define VERBOSE(stuff) stuff
#else
#define VERBOSE(stuff)
#endif

namespace au {

const char* to_cstring(AngleDir dir)
{
    switch (dir) {
    case AngleDir::CW:
        return "CW";
    case AngleDir::CCW:
        return "CCW";
    }
}

std::ostream& operator<<(std::ostream& o, AngleDir dir)
{
    o << to_cstring(dir);
    return o;
}

namespace {

struct Circle2d
{
    vec2d center;
    double radius;
};

struct DirectionalCircle2d
{
    Circle2d circle;
    int direction; // -1 for left, 1 for right

    bool left() const { return direction == -1; }
    bool right() const { return direction == 1; }
};

struct Segment2d
{
    vec2d a;
    vec2d b;
};

std::ostream& operator<<(std::ostream& o, const Circle2d& c) {
    o << "{ c: (" << c.center.x << ", " << c.center.y << "), r: " << c.radius << " }";
    return o;
}

std::ostream& operator<<(std::ostream& o, const DirectionalCircle2d& c)
{
    o << "{ ";
    o << c.circle;
    o << ", Direction: " << (c.direction == -1 ? "Left" : "Right");
    o << " }";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Segment2d& s)
{
    o << "{ a: " << s.a << ", b: " << s.b << " }";
    return o;
}

double interp_angle(double aa, double ab, double t, AngleDir dir)
{
    const double sadist = smpl::shortest_angle_diff(ab, aa);

    double af;
    if (sadist < 0 && dir == AngleDir::CW || sadist > 0 && dir == AngleDir::CCW) {
        af = aa + t * sadist;
    }
    else {
        auto signd = [](double d) {
            if (d > 0) { return 1.0; }
            else if (d < 0.0) { return -1.0; }
            else { return 0.0; }
        };
        af = aa + t * -signd(sadist) * (2.0 * M_PI - fabs(sadist));
    }

    return smpl::normalize_angle_positive(af);
}

void compute_turning_circles(
    const pose2d& pose,
    double turning_radius_m,
    DirectionalCircle2d& left_turn,
    DirectionalCircle2d& right_turn);

void compute_inner_tangent(
    const DirectionalCircle2d& dc1,
    const DirectionalCircle2d& dc2,
    Segment2d& tangent);

void compute_outer_tangent(
    const DirectionalCircle2d& dc1,
    const DirectionalCircle2d& dc2,
    Segment2d& tangent);

std::vector<pose2d> generate_turn_path(
    const DirectionalCircle2d& circle,
    double start_angle,
    double end_angle,
    double interp_res);

std::vector<pose2d> generate_straight_path(
    const vec2d& start,
    const vec2d& end,
    double interp_res);

double compute_arc_length(
    const DirectionalCircle2d& circle,
    double start_angle,
    double end_angle);

vec2d rotate(const vec2d& v, double theta)
{
    double newvx = v.x * cos(theta) - v.y * sin(theta);
    double newvy = v.x * sin(theta) + v.y * cos(theta);
    return vec2d(newvx, newvy);
}

void compute_turning_circles(
    const pose2d& pose,
    double turning_radius_m,
    DirectionalCircle2d& left_turn,
    DirectionalCircle2d& right_turn)
{
    vec2d heading(cos(pose.yaw), sin(pose.yaw));
    heading.normalize();
    heading = rotate(heading, M_PI / 2.0);

    left_turn.circle.center = pose.pos + turning_radius_m * heading;
    left_turn.circle.radius = turning_radius_m;
    left_turn.direction = -1;

    heading = rotate(heading, M_PI);
    right_turn.circle.center = pose.pos + turning_radius_m * heading;
    right_turn.circle.radius = turning_radius_m;
    right_turn.direction = 1;
}

void compute_inner_tangent(
    const DirectionalCircle2d& dc1,
    const DirectionalCircle2d& dc2,
    Segment2d& tangent)
{
    const Circle2d& c1 = dc1.circle;
    const Circle2d& c2 = dc2.circle;

    const vec2d& p1 = c1.center;
    const vec2d& p2 = c2.center;
    double r1 = c1.radius;
    double r2 = c2.radius;

    vec2d v1(p2 - p1);
    double D = v1.norm();

    // create new circle at the midpoint between two input circles whose
    // circumference touches the centers of both input circle_centers
    vec2d p3(0.5 * (p1 + p2));
    double r3 = 0.5 * D;

    if (D < r1 + r2) {
        tangent.a = vec2d(
                std::numeric_limits<double>::quiet_NaN(),
                std::numeric_limits<double>::quiet_NaN());
        tangent.b = vec2d(
                std::numeric_limits<double>::quiet_NaN(),
                std::numeric_limits<double>::quiet_NaN());
        return;
    }

    const vec2d& p4 = p1;
    double r4 = r1 + r2;

    // Find the angle v1 would need to be rotated to point towards the upper tangent point
    double gamma = acos(r4 / (2*r3)); // r3*r3 = r3*r3 + r4*r4 - 2*r3*r4*cos(gamma) from law of cosines

    vec2d v2(v1.normalized());
    v2 = rotate(v2, gamma);

    Segment2d upper_inner_tangent, lower_inner_tangent;

    // Find tangent point on the first circle
    vec2d pt = p1 + r4 * v2;
    upper_inner_tangent.a = p1 + r1 * v2;

    // Find the tangent point on the second circle
    vec2d v4 = p2 - pt;
    upper_inner_tangent.b = upper_inner_tangent.a + v4;

    // Repeat the last couple steps to find the lower inner tangent
    v2 = v1.normalized();
    v2 = rotate(v2, -gamma);
    vec2d lpt = p1 + r4 * v2;
    lower_inner_tangent.a = p1 + r1 * v2;
    v4 = p2 - lpt;
    lower_inner_tangent.b = lower_inner_tangent.a + v4;

    if (dc1.right()) {
        tangent = upper_inner_tangent;
    }
    else {
        tangent = lower_inner_tangent;
//        printf("tangent: %s = p1 + r4 * v2 = %s + %0.3f * %s\n", to_string(tangent).c_str(), to_string(p1).c_str(), r4, to_string(v2).c_str());
//        printf("c1: %s, c2: %s\n", to_string(c1).c_str(), to_string(c2).c_str());
//        printf("r4: %0.3f, r3: %0.3f, gamma: %0.3f, v1: %s, D: %0.3f\n", r4, r3, gamma, to_string(v1).c_str(), D);
    }
}

void compute_outer_tangent(
    const DirectionalCircle2d& dc1,
    const DirectionalCircle2d& dc2,
    Segment2d& tangent)
{
    const Circle2d& c1 = dc1.circle;
    const Circle2d& c2 = dc2.circle;

    const vec2d& p1 = c1.radius > c2.radius ? c1.center : c2.center;
    const vec2d& p2 = c1.radius > c2.radius ? c2.center : c1.center;
    double r1 = c1.radius > c2.radius ? c1.radius : c2.radius;
    double r2 = c1.radius > c2.radius ? c2.radius : c1.radius;

    vec2d v1(p2 - p1);

    double D = v1.norm();

    vec2d p3(0.5 * (p1 + p1));
    double r3 = 0.5 * D;

    const vec2d& p4 = p1;
    double r4 = r1 - r2;

    double gamma = acos(r4 / (2*r3));

    vec2d v2(v1.normalized());
    v2 = rotate(v2, gamma);

    Segment2d upper_outer_tangent, lower_outer_tangent;

    vec2d pt = p1 + r4 * v2;
    upper_outer_tangent.a = p1 + r1 * v2;

    vec2d v4 = p2 - pt;
    upper_outer_tangent.b = upper_outer_tangent.a + v4;

    v2 = v1.normalized();
    v2 = rotate(v2, -gamma);
    vec2d lpt = p1 + r4 * v2;
    lower_outer_tangent.a = p1 + r1 * v2;
    v4 = p2 - lpt;
    lower_outer_tangent.b = lower_outer_tangent.a + v4;

    if (dc1.right()) {
        tangent = c1.radius > c2.radius ? upper_outer_tangent : lower_outer_tangent;
    }
    else {
        tangent = c1.radius > c2.radius ? lower_outer_tangent : upper_outer_tangent;
    }

    if (c1.radius <= c2.radius) {
        std::swap(tangent.a, tangent.b);
    }
}

std::vector<pose2d> generate_turn_path(
    const DirectionalCircle2d& circle,
    double start_angle,
    double end_angle,
    double interp_res)
{
    std::vector<pose2d> path;
    double angle = start_angle;
    path.push_back({
            circle.circle.center.x + circle.circle.radius * cos(start_angle),
            circle.circle.center.y + circle.circle.radius * sin(start_angle),
            circle.right() ? start_angle - M_PI / 2.0 : start_angle + M_PI / 2.0
    });

    double arc_length = compute_arc_length(circle, start_angle, end_angle);

    int num_interm_points = (int)floor(arc_length / interp_res);

    path.reserve(num_interm_points + 2);

    const double angle_res = interp_res / circle.circle.radius;
    if (circle.right()) {
        // clockwise
        for (int i = 0; i < num_interm_points; ++i) {
            angle -= angle_res;
            path.push_back({
                    circle.circle.center.x + circle.circle.radius * cos(angle),
                    circle.circle.center.y + circle.circle.radius * sin(angle),
                    angle - M_PI / 2.0
            });
        }
    }
    else {
        // counter-clockwise
        for (int i = 0; i < num_interm_points; ++i) {
            angle += angle_res;
            path.push_back({
                    circle.circle.center.x + circle.circle.radius * cos(angle),
                    circle.circle.center.y + circle.circle.radius * sin(angle),
                    angle + M_PI / 2.0
            });
        }
    }

    path.push_back({
            circle.circle.center.x + circle.circle.radius * cos(end_angle),
            circle.circle.center.y + circle.circle.radius * sin(end_angle),
            circle.right() ? end_angle - M_PI / 2.0 : end_angle + M_PI / 2.0
    });

    return path;
}

std::vector<pose2d> generate_straight_path(
    const vec2d& start,
    const vec2d& end,
    double interp_res)
{
    vec2d v1 = end - start;
    double angle = atan2(v1.y, v1.x);
    return { { vec2d(start.x, start.y), angle }, { vec2d(end.x, end.y), angle } };
}

double compute_arc_length(
    const DirectionalCircle2d& circle,
    double start_angle,
    double end_angle)
{
    vec2d start(circle.circle.center.x + circle.circle.radius * cos(start_angle), circle.circle.center.y + circle.circle.radius * sin(start_angle));
    vec2d end(circle.circle.center.x + circle.circle.radius * cos(end_angle), circle.circle.center.y + circle.circle.radius * sin(end_angle));

    vec2d v1 = start - circle.circle.center;
    vec2d v2 = end - circle.circle.center;
    double theta = atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
    if (theta < 0 && circle.left()) {
        theta += 2 * M_PI;
    }
    else if (theta > 0 && circle.right()) {
        theta -= 2 * M_PI;
    }
    return fabs(theta * circle.circle.radius);
}

dubins_path construct_rr_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    dubins_path rr_path;
    rr_path.start = start;
    rr_path.goal = goal;
    rr_path.radius = radius;
    rr_path.dir1 = AngleDir::CW;
    rr_path.dir2 = AngleDir::CW;

    vec2d v1 = start.pos - start_circle.circle.center;
    double start_angle = atan2(v1.y, v1.x);

    vec2d v2 = tangent.a - start_circle.circle.center;
    double end_angle = atan2(v2.y, v2.x);

    rr_path.arc1 = compute_arc_length(
            start_circle, start_angle, end_angle) / radius;

    v1 = tangent.b - goal_circle.circle.center;
    start_angle = atan2(v1.y, v1.x);

    v2 = goal.pos - goal_circle.circle.center;
    end_angle = atan2(v2.y, v2.x);

    rr_path.arc2 = compute_arc_length(
            goal_circle, start_angle, end_angle) / radius;
    return rr_path;
}

dubins_path construct_ll_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    dubins_path ll_path;
    ll_path.start = start;
    ll_path.goal = goal;
    ll_path.radius = radius;
    ll_path.dir1 = AngleDir::CCW;
    ll_path.dir2 = AngleDir::CCW;

    vec2d v1 = start.pos - start_circle.circle.center;
    double start_angle = atan2(v1.y, v1.x);

    vec2d v2 = tangent.a - start_circle.circle.center;
    double end_angle = atan2(v2.y, v2.x);

    ll_path.arc1 = compute_arc_length(
            start_circle, start_angle, end_angle) / radius;

    v1 = tangent.b - goal_circle.circle.center;
    start_angle = atan2(v1.y, v1.x);

    v2 = goal.pos - goal_circle.circle.center;
    end_angle = atan2(v2.y, v2.x);

    ll_path.arc2 = compute_arc_length(
            goal_circle, start_angle, end_angle) / radius;
    return ll_path;
}

dubins_path construct_lr_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    dubins_path lr_path;
    lr_path.start = start;
    lr_path.goal = goal;
    lr_path.radius = radius;
    lr_path.dir1 = AngleDir::CCW;
    lr_path.dir2 = AngleDir::CW;

    vec2d v1 = start.pos - start_circle.circle.center;
    double start_angle = atan2(v1.y, v1.x);

    vec2d v2 = tangent.a - start_circle.circle.center;
    double end_angle = atan2(v2.y, v2.x);

    lr_path.arc1 = compute_arc_length(
            start_circle, start_angle, end_angle) / radius;

    v1 = tangent.b - goal_circle.circle.center;
    start_angle = atan2(v1.y, v1.x);

    v2 = goal.pos - goal_circle.circle.center;
    end_angle = atan2(v2.y, v2.x);

    lr_path.arc2 = compute_arc_length(
            goal_circle, start_angle, end_angle) / radius;
    return lr_path;
}

dubins_path construct_rl_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    dubins_path rl_path;
    rl_path.start = start;
    rl_path.goal = goal;
    rl_path.radius = radius;
    rl_path.dir1 = AngleDir::CW;
    rl_path.dir2 = AngleDir::CCW;

    vec2d v1 = start.pos - start_circle.circle.center;
    double start_angle = atan2(v1.y, v1.x);

    vec2d v2 = tangent.a - start_circle.circle.center;
    double end_angle = atan2(v2.y, v2.x);

    rl_path.arc1 = compute_arc_length(
            start_circle, start_angle, end_angle) / radius;

    v1 = tangent.b - goal_circle.circle.center;
    start_angle = atan2(v1.y, v1.x);

    v2 = goal.pos - goal_circle.circle.center;
    end_angle = atan2(v2.y, v2.x);

    rl_path.arc2 = compute_arc_length(
            goal_circle, start_angle, end_angle) / radius;
    return rl_path;
}

dubins_path construct_rlr_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    return dubins_path();
//    // check for RLR trajectory
//    const Circle2d& c1 = start_circle.circle;
//    const Circle2d& c2 = goal_circle.circle;
//
//    vec2d v1 = c2.center - c1.center;
//    double D = v1.norm();
//    double gamma = acos(D / (4 * radius));
//    double theta = atan2(v1.y, v1.x) + gamma;
//    vec2d v2(1.0, 0.0);// = v1;
//    v2.normalize();
//
//    rotate(v2, theta); //gamma); //theta);
//
//    vec2d p3 = c1.center + 2 * radius * v2;
//    vec2d pt1 = p3 + (c1.center - p3).normalized() * radius;
//    vec2d pt2 = p3 + (c2.center - p3).normalized() * radius;
//
//    v1 = start.pos - c1.center;
//    double start_angle1 = atan2(v1.y, v1.x);
//    v1 = pt1 - c1.center;
//    double end_angle1 = atan2(v1.y, v1.x);
//    std::vector<pose2d> turn1 = generate_turn_path(
//            start_circle,
//            start_angle1,
//            end_angle1,
//            interp_res);
//
//    v1 = pt1 - p3;
//    double start_angle2 = atan2(v1.y, v1.x);
//    v1 = pt2 - p3;
//    double end_angle2 = atan2(v1.y, v1.x);
//    DirectionalCircle2d midturncircle = { { p3, radius }, -1 };
//    std::vector<pose2d> turn2 = generate_turn_path(
//            midturncircle,
//            start_angle2,
//            end_angle2,
//            interp_res);
//
//    v1 = pt2 - c2.center;
//    double start_angle3 = atan2(v1.y, v1.x);
//    v1 = goal.pos - c2.center;
//    double end_angle3 = atan2(v1.y, v1.x);
//    std::vector<pose2d> turn3 = generate_turn_path(
//            goal_circle,
//            start_angle3,
//            end_angle3,
//            interp_res);
//
//    turn1.insert(turn1.end(), turn2.begin(), turn2.end());
//    turn1.insert(turn1.end(), turn3.begin(), turn3.end());
//
//    paths.push_back(turn1);
}

dubins_path construct_lrl_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    const DirectionalCircle2d& start_circle,
    const DirectionalCircle2d& goal_circle,
    const Segment2d& tangent)
{
    return dubins_path();
//    // check for LRL trajectory
//    const Circle2d& c1 = start_circle.circle;
//    const Circle2d& c2 = goal_circle.circle;
//
//    vec2d v1 = c2.center - c1.center;
//    double D = v1.norm();
//    double gamma = acos(D / (4 * radius));
//    double theta = atan2(v1.y, v1.x) + gamma;
//    vec2d v2 = v1;
//    v2.normalize();
//
//    rotate(v2, -gamma); //theta);
//
//    vec2d p3 = c1.center + 2 * radius * v2;
//    vec2d pt1 = p3 + (c1.center - p3).normalized() * radius;
//    vec2d pt2 = p3 + (c2.center - p3).normalized() * radius;
//
//    v1 = start.pos - c1.center;
//    double start_angle1 = atan2(v1.y, v1.x);
//    v1 = pt1 - c1.center;
//    double end_angle1 = atan2(v1.y, v1.x);
//    std::vector<pose2d> turn1 = generate_turn_path(
//            start_circle,
//            start_angle1,
//            end_angle1,
//            interp_res);
//
//    v1 = pt1 - p3;
//    double start_angle2 = atan2(v1.y, v1.x);
//    v1 = pt2 - p3;
//    double end_angle2 = atan2(v1.y, v1.x);
//    DirectionalCircle2d midturncircle = { { p3, radius }, 1 };
//    std::vector<pose2d> turn2 = generate_turn_path(
//            midturncircle,
//            start_angle2,
//            end_angle2,
//            interp_res);
//
//    v1 = pt2 - c2.center;
//    double start_angle3 = atan2(v1.y, v1.x);
//    v1 = goal.pos - c2.center;
//    double end_angle3 = atan2(v1.y, v1.x);
//    std::vector<pose2d> turn3 = generate_turn_path(
//            goal_circle,
//            start_angle3,
//            end_angle3,
//            interp_res);
//
//    turn1.insert(turn1.end(), turn2.begin(), turn2.end());
//    turn1.insert(turn1.end(), turn3.begin(), turn3.end());
//
//    paths.push_back(turn1);
}

vec2d rotate90cw(const vec2d& v)
{
    return vec2d(v.y, -v.x);
}

vec2d rotate90ccw(const vec2d& v)
{
    return vec2d(-v.y, v.x);
}

vec2d calc_heading_vector(double th)
{
    return vec2d(cos(th), sin(th));
}

double calc_heading(const vec2d& v)
{
    return atan2(v.y, v.x);
}

double dist(const vec2d& u, const vec2d& v)
{
    return length(u - v);
}

double dist_sqrd(const vec2d& u, const vec2d& v)
{
    return length_sqrd(u - v);
}

} // empty namespace

dubins_path::dubins_path()
{
}

dubins_path::dubins_path(
    const pose2d& start,
    const pose2d& goal,
    double radius,
    double arc1,
    double arc2,
    AngleDir dir1,
    AngleDir dir2)
:
    start(start),
    goal(goal),
    radius(radius),
    arc1(arc1),
    arc2(arc2),
    dir1(dir1),
    dir2(dir2)
{
}

vec2d dubins_path::pivot1() const
{
    vec2d hv = calc_heading_vector(start.yaw);
    if (dir1 == AngleDir::CW) {
        hv = rotate90cw(hv);
    }
    else if (dir1 == AngleDir::CCW) {
        hv = rotate90ccw(hv);
    }

    return start.pos + hv * radius;
}

vec2d dubins_path::pivot2() const
{
    vec2d hv = calc_heading_vector(goal.yaw);
    if (dir2 == AngleDir::CW) {
        hv = rotate90cw(hv);
    }
    else if (dir2 == AngleDir::CCW) {
        hv = rotate90ccw(hv);
    }
    return goal.pos + hv * radius;
}

pose2d dubins_path::straight_start() const
{
    vec2d piv1 = pivot1();
    vec2d h2s = start.pos - piv1;
    const double dir = (dir1 == AngleDir::CCW ? 1.0 : -1.0);
    return pose2d(piv1 + rotate(h2s, dir * arc1), start.yaw + dir * arc1);
}

pose2d dubins_path::straight_end() const
{
    vec2d piv2 = pivot2();
    vec2d h2g = goal.pos - piv2;
    const double dir = (dir2 == AngleDir::CCW ? 1.0 : -1.0);
    return pose2d(piv2 + rotate(h2g, -dir * arc2), goal.yaw - dir * arc2);
}

double dubins_path::t1() const
{
    double len = length();
    if (len == 0.0) {
        return 0.0;
    }
    double arc1len = arc1 * radius;
    return arc1len / len;
}

double dubins_path::t2() const
{
    double len = length();
    if (len == 0.0) {
        return 0.0;
    }
    double arc2len = arc2 * radius;
    return (len - arc2len) / len;
}

double dubins_path::length() const
{
    return arc1 * radius +
            dist(straight_start().pos, straight_end().pos) +
            arc2 * radius;
}

pose2d dubins_path::at(double t) const
{
    if (t <= 0.0) {
        return start;
    }
    if (t >= 1.0) {
        return goal;
    }

    double t_1 = t1();
    double t_2 = t2();
    vec2d piv1 = pivot1();
    vec2d piv2 = pivot2();

    if (t < t_1) {
        // on start turn
        double a = t / t_1;
        vec2d h2s = start.pos - piv1;
        const double dir = (dir1 == AngleDir::CCW ? 1.0 : -1.0);
        return pose2d(piv1 + rotate(h2s, dir * a * arc1),
                interp_angle(start.yaw, start.yaw + dir * arc1, a, dir1));
    }
    else if (t > t_2) {
        // on goal turn
        double a = (t - t_2) / (1 - t_2);
        vec2d h2g = goal.pos - piv2;
        const double dir = (dir2 == AngleDir::CCW ? 1.0 : -1.0);
        return pose2d(piv2 + rotate(h2g, -dir * (1.0 - a) * arc2),
                interp_angle(goal.yaw - dir * arc2, goal.yaw, a, dir2));
    }
    else {
        // on straight segment
        const double a = (t - t_1) / (t_2 - t_1);
        return pose2d(interp(straight_start(), straight_end(), a));
    }
}

std::vector<dubins_path> generate_dubins_paths(
    const pose2d& start,
    const pose2d& goal,
    double radius)
{
    // compute both pairs of turning circles
    DirectionalCircle2d start_circle_l, start_circle_r;
    DirectionalCircle2d goal_circle_l, goal_circle_r;
    compute_turning_circles(start, radius, start_circle_l, start_circle_r);
    compute_turning_circles(goal, radius, goal_circle_l, goal_circle_r);

    // TODO: variant that returns geometric debug information (turning circles)

    Segment2d ll_tangent;
    Segment2d rr_tangent;
    Segment2d lr_tangent;
    Segment2d rl_tangent;

    compute_outer_tangent(start_circle_l, goal_circle_l, ll_tangent);
    compute_outer_tangent(start_circle_r, goal_circle_r, rr_tangent);
    compute_inner_tangent(start_circle_l, goal_circle_r, lr_tangent);
    compute_inner_tangent(start_circle_r, goal_circle_l, rl_tangent);

    dubins_paths paths;
    paths.reserve(6);

    if (rr_tangent.a.x == rr_tangent.a.x) {
        paths.push_back(construct_rr_path(
                start, goal, radius,
                start_circle_r, goal_circle_r, rr_tangent));
    }

    if (ll_tangent.a.x == ll_tangent.a.x) {
        paths.push_back(construct_ll_path(
                start, goal, radius,
                start_circle_l, goal_circle_l, ll_tangent));
    }

    if (lr_tangent.a.x == lr_tangent.a.x) // else nan
    {
        paths.push_back(construct_lr_path(
                start, goal, radius,
                start_circle_l, goal_circle_r, lr_tangent));
    }

    if (rl_tangent.a.x == rl_tangent.a.x) { // else nan
        paths.push_back(construct_rl_path(
                start, goal, radius,
                start_circle_r, goal_circle_l, rl_tangent));
    }

//    if ((start_circle_l.circle.center - goal_circle_l.circle.center).norm() < 4 * radius) {
//        paths.push_back(construct_lrl_path())
//    }
//
//    if ((start_circle_r.circle.center - goal_circle_r.circle.center).norm() < 4 * radius) {
//        paths.push_back(construct_rlr_path())
//    }

    return paths;
}

double length(const dubins_path& p)
{
    return p.length();
}

std::ostream& operator<<(std::ostream& o, const dubins_path& p)
{
    o << "{ start: " << p.start << ", goal: " << p.goal << ", radius: " << p.radius << ", arc1: " << p.arc1 << ", arc2: " << p.arc2 << " }";
    return o;
}

} // namespace au
