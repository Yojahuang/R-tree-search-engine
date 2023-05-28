#ifndef NODE_H
#define NODE_H

#include <vector>
#include <algorithm>
#include "Geometry.hpp"

class Node {
public:
    Node(bool leaf = false) : is_leaf(leaf) {
        point_size = 0;
    }
    ~Node();

    bool is_leaf;
    int point_size;
    std::vector<Geometry::Point> points;
    std::vector<Node*> children;
    Geometry::Rectangle rect;

    void update_point_size() {
        point_size = points.size();
        for (Node* item: children) {
            point_size += item->point_size;
        }
    }
};

Node::~Node() {
    for (auto& item: children) {
        delete item;
    }
    children.clear();
    points.clear();
}

#endif