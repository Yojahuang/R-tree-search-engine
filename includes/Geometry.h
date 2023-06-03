#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>

namespace Geometry {
    class Point {
    public:
        double x, y;
        int id;
        
        Point(double _x, double _y, int _id) {
            x = _x;
            y = _y;
            id = _id;
        }

        Point(double _x, double _y) {
            x = _x;
            y = _y;
            id = -1;
        }
        Point();

        bool operator==(const Point& rhs) const {
            return x == rhs.x && y == rhs.y && id == rhs.id;
        }
    };

    class Rectangle {
    public:
        double x_min, x_max;
        double y_min, y_max;

        Rectangle() {
            x_min = std::numeric_limits<double>::max();
            x_max = std::numeric_limits<double>::lowest();
            y_min = std::numeric_limits<double>::max();
            y_max = std::numeric_limits<double>::lowest();
        }

        double area() {
            if (x_min == std::numeric_limits<double>::max()) return std::numeric_limits<double>::max();
            double dx = x_max - x_min;
            double dy = y_max - y_min;
            return dx * dy;
        }
    };

    bool is_inside(const Geometry::Rectangle& rect, const Geometry::Point& pt);
    bool is_overlap(const Geometry::Rectangle& lhs, const Geometry::Rectangle& rhs);

    void update_rectangle(Geometry::Rectangle& rect, const Geometry::Point& pt);

    void update_rectangle(Geometry::Rectangle& rect_dest, const Geometry::Rectangle& rect_src);

    double dist(const Geometry::Point& pt1, const Geometry::Point& pt2);

    double min_dist(const Geometry::Rectangle& rest, const Geometry::Point& pt);

    double min_max_dist(const Geometry::Rectangle& rest, const Geometry::Point& pt);
}
#endif