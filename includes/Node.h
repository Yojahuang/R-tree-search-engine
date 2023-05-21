#ifndef NODE_H
#define NODE_H

#include <vector>
#include <algorithm>

class Point {
public:
    double x;
    double y;
    int id;

    Point(double _x, double _y, int _id): x(_x), y(_y), id(_id) {} 
};

class Rectangle {
public:
    double x_min;
    double y_min;
    double x_max;
    double y_max;
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

class Node {
public:
    Node(bool leaf = false) : is_leaf(leaf) {
        rect.x_min = std::numeric_limits<double>::max();
        rect.x_max = std::numeric_limits<double>::lowest();
        rect.y_min = std::numeric_limits<double>::max();
        rect.y_max = std::numeric_limits<double>::lowest();
    }
    ~Node();

    bool is_leaf;
    std::vector<Point> points;
    std::vector<Node*> children;
    Rectangle rect;
};

Node::~Node() {
    for (auto& item: children) {
        delete item;
    }
    children.clear();
    points.clear();
}

#endif