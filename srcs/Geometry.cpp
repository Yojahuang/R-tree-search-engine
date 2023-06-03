#include "Geometry.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>


bool Geometry::is_inside(const Geometry::Rectangle& rect, const Geometry::Point& pt) {
    bool x_inside = rect.x_min <= pt.x && pt.x <= rect.x_max;
    bool y_inside = rect.y_min <= pt.y && pt.y <= rect.y_max;
    return x_inside & y_inside;
}

bool Geometry::is_overlap(const Geometry::Rectangle& lhs, const Geometry::Rectangle& rhs) {
    return Geometry::is_inside(rhs, Point(lhs.x_min, lhs.y_min, 0)) |
            Geometry::is_inside(rhs, Point(lhs.x_max, lhs.y_min, 0)) |
            Geometry::is_inside(rhs, Point(lhs.x_min, lhs.y_max, 0)) |
            Geometry::is_inside(rhs, Point(lhs.x_max, lhs.y_max, 0));
}

void Geometry::update_rectangle(Geometry::Rectangle& rect, const Geometry::Point& pt) {
    rect.x_min = std::min(rect.x_min, pt.x);
    rect.y_min = std::min(rect.y_min, pt.y);
    rect.x_max = std::max(rect.x_max, pt.x);
    rect.y_max = std::max(rect.y_max, pt.y);
}

void Geometry::update_rectangle(Geometry::Rectangle& rect_dest, const Geometry::Rectangle& rect_src) {
    rect_dest.x_min = std::min(rect_dest.x_min, rect_src.x_min);
    rect_dest.y_min = std::min(rect_dest.y_min, rect_src.y_min);
    rect_dest.x_max = std::max(rect_dest.x_max, rect_src.x_max);
    rect_dest.y_max = std::max(rect_dest.y_max, rect_src.y_max);
}

double Geometry::dist(const Geometry::Point& pt1, const Geometry::Point& pt2) {
    double dx = pt1.x - pt2.x;
    double dy = pt1.y - pt2.y;
    double distance = dx * dx + dy * dy;
    distance = sqrt(distance);
    return distance;
}

double Geometry::min_dist(const Geometry::Rectangle& rest, const Geometry::Point& pt) {
  if (is_inside(rest, pt)) {
      return 0;
  } else if (rest.x_min <= pt.x && pt.x <= rest.x_max) {
      return std::min(abs(pt.y - rest.y_min), abs(pt.y - rest.y_max));
  } else if (rest.y_min <= pt.y && pt.y <= rest.y_max) {
      return std::min(abs(pt.x - rest.x_min), abs(pt.x - rest.x_max));
  } else {
    std::vector<double> dists = {
        dist(pt, Point(rest.x_min, rest.y_min)),
        dist(pt, Point(rest.x_min, rest.y_max)),
        dist(pt, Point(rest.x_max, rest.y_min)),
        dist(pt, Point(rest.x_max, rest.y_max)),
    };
    return *std::min_element(dists.begin(), dists.end());
  }
}

double Geometry::min_max_dist(const Geometry::Rectangle& rest, const Geometry::Point& pt) {
    std::vector<double> dists = {
        std::max(dist(pt, Point(rest.x_min, rest.y_min)), dist(pt, Point(rest.x_min, rest.y_max))),
        std::max(dist(pt, Point(rest.x_max, rest.y_min)), dist(pt, Point(rest.x_max, rest.y_max))),
        std::max(dist(pt, Point(rest.x_min, rest.y_min)), dist(pt, Point(rest.x_max, rest.y_min))),
        std::max(dist(pt, Point(rest.x_min, rest.y_max)), dist(pt, Point(rest.x_max, rest.y_max))),
    };
    return *std::min_element(dists.begin(), dists.end());
}
