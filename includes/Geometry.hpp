namespace Geometry {
    class Point {
    public:
        double x, y;
        int id;
        Point(double, double, int);
        Point();
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

    bool is_inside(const Rectangle& rect, const Point& pt) const {
        bool x_inside = rect.x_min <= pt.x && pt.x <= rect.x_max;
        bool y_inside = rect.y_min <= pt.y && pt.y <= rect.y_max;
        return x_inside & y_inside;
    }

    bool is_overlap(const Rectangle& lhs, const Rectangle& rhs) const {
        return is_inside(Point(lhs.x_min, lhs.y_min, 0), rhs) |
                is_inside(Point(lhs.x_max, lhs.y_min, 0), rhs) |
                is_inside(Point(lhs.x_min, lhs.y_max, 0), rhs) |
                is_inside(Point(lhs.x_max, lhs.y_max, 0), rhs);
    }

    void update_rectangle(Rectangle& rect, const Point& pt) {
        rect.x_min = std::min(rect.x_min, pt.x);
        rect.y_min = std::min(rect.y_min, pt.y);
        rect.x_max = std::max(rect.x_max, pt.x);
        rect.y_max = std::max(rect.y_max, pt.y);
    }

    void update_rectangle(Rectangle& rect_dest, const Rectangle& rect_src) {
        rect_dest.x_min = std::min(rect_dest.x_min, rect_src.x_min);
        rect_dest.y_min = std::min(rect_dest.y_min, rect_src.y_min);
        rect_dest.x_max = std::max(rect_dest.x_max, rect_src.x_max);
        rect_dest.y_max = std::max(rect_dest.y_max, rect_src.y_max);
    }

    double dist(const Point& pt1, const Point& pt2) const {
        double dx = pt1.x - pt2.x;
        double dy = pt1.y - pt2.y;
        double distance = dx * dx + dy * dy;
        distance = sqrt(distance);
        return distance;
    }

    double min_dist(const Rectangle& rest, const Point& pt) const {
        if (is_inside(rest, pt)) {
            return 0;
        } else if (rest.x_min <= pt.x && pt.x <= rest.x_max) {
            return std::min(abs(pt.y - rest.y_min), abs(pt.y - rest.y_max));
        } else if (rest.y_min <= pt.y && pt.y <= rest.y_max) {
            return std::min(abs(pt.x - rest.x_min), abs(pt.x - rest.x_max));
        } else {
            return std::min({
                dist(pt, Point(rest.x_min, rest.y_min)),
                dist(pt, Point(rest.x_min, rest.y_max)),
                dist(pt, Point(rest.x_max, rest.y_min)),
                dist(pt, Point(rest.x_max, rest.y_max)),
            });
        }
    }

    double min_max_dist(const Rectangle& rest, const Point& pt) const {
        return std::min({
            std::max(dist(pt, Point(rest.x_min, rest.y_min)), dist(pt, Point(rest.x_min, rest.y_max))),
            std::max(dist(pt, Point(rest.x_max, rest.y_min)), dist(pt, Point(rest.x_max, rest.y_max))),
            std::max(dist(pt, Point(rest.x_min, rest.y_min)), dist(pt, Point(rest.x_max, rest.y_min))),
            std::max(dist(pt, Point(rest.x_min, rest.y_max)), dist(pt, Point(rest.x_max, rest.y_max))),
        })
    }
}