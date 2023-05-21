#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <algorithm>
#include "RTree.h"
#include "Node.h"

RTree::RTree() : max_children(8), root(new Node(true)) {}

void RTree::add_point(std::tuple<double,double,int> val) {
    Point point;
    point.x = std::get<0>(val);
    point.y = std::get<1>(val);
    point.id = std::get<2>(val);

    insert(root, point);
}

Node* RTree::choose_leaf(Node* node, const Point& point) {
    if (node->is_leaf) {
        return node;
    }

    double min_enlargement = std::numeric_limits<double>::max();
    Node* selected_child = nullptr;
    for (auto* child : node->children) {
        Rectangle enlarged_rect;
        enlarged_rect.x_min = std::min(child->rect.x_min, point.x);
        enlarged_rect.y_min = std::min(child->rect.y_min, point.y);
        enlarged_rect.x_max = std::max(child->rect.x_max, point.x);
        enlarged_rect.y_max = std::max(child->rect.y_max, point.y);

        double enlarge_rect_area = (enlarged_rect.x_max - enlarged_rect.x_min) * (enlarged_rect.y_max - enlarged_rect.y_min);
        double current_rect_area = (child->rect.x_max - child->rect.x_min) * (child->rect.y_max - child->rect.y_min);

        if (child->points.size() == 0) enlarge_rect_area = current_rect_area = 0;

        double enlargement = enlarge_rect_area - current_rect_area;
        if (enlargement < min_enlargement) {
            min_enlargement = enlargement;
            selected_child = child;
        }
    }

    if (selected_child != nullptr) update_rectangle(selected_child->rect, point);
    return choose_leaf(selected_child, point);
}

void RTree::clear() {
    delete root;
    root = new Node(true);
}

void RTree::insert(Node* node, const Point& point) {
    Node* target = choose_leaf(node, point);
    target->points.push_back(point);
    update_rectangle(target->rect, point);

    if (target->points > max_children) {
        Node* split = split_node(target);
        adjust_tree(target, split);
    }
}