#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <algorithm>
#include "RTree.h"
#include "Node.h"

RTree::RTree() : max_children(8), root(new Node(true)) {}

void RTree::add_point(std::tuple<double,double,int> val) {
    Geometry::Point point;
    point.x = std::get<0>(val);
    point.y = std::get<1>(val);
    point.id = std::get<2>(val);

    insert(point);
}

Node* RTree::choose_leaf(Node* node, const Geometry::Point& point) {
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

        double enlarge_rect_area = enlarged_rect.area();
        double current_rect_area = child->rect.area();

        if (child->points.size() == 0) enlarge_rect_area = current_rect_area = 0;

        double enlargement = enlarge_rect_area - current_rect_area;
        if (enlargement < min_enlargement) {
            min_enlargement = enlargement;
            selected_child = child;
        }
    }

    if (selected_child != nullptr) {
        update_rectangle(selected_child->rect, point);
        selected_child->point_size++;
    }
    return choose_leaf(selected_child, point);
}

Node* RTree::split_node(Node* node) {
    if (node->is_leaf) {
        int mask = 0, full_mask = 1 << node->points.size();

        int best_mask = 0;
        double best_area = std::numeric_limits<double>::max();
        for (mask = 0; mask <= full_mask; ++mask) {
            std::vector<Geometry::Point> left, right;
            for (size_t i = 0; i < node->points.size(); ++i) {
                if (mask & (1 << i)) { 
                    left.push_back(node->points[i]);
                } else {
                    right.push_back(node->points[i]);
                }
            }

            Rectangle left_rect, right_rect;
            for (auto item: left) Geometry::update_rectangle(left_rect, item);
            for (auto item: right) Geometry::update_rectangle(right_rect, item);

            if (std::max(left.size(), right.size()) <= max_children &&
                left_rect.area() + right_rect.area() < best_area
            ) {
                best_mask = mask;
                best_area = left_rect.area() + right_rect.area();
            }
        }

        Node* new_node = new Node(node->is_leaf);
        Node* ori_node = new Node(node->is_leaf);

        for (size_t i = 0; i < node->points.size(); ++i) {
            if (best_mask & (1 << i)) new_node.points.push_back(node->points[i]);
            else ori_node.points.push_back(node->points[i]);
        }

        new_node->update_point_size();
        ori_node->update_point_size();

        node = ori_node;

        return new_node;
    } else {
        int mask = 0, full_mask = 1 << node->children.size();

        int best_mask = 0;
        double best_area = std::numeric_limits<double>::max();
        for (mask = 0; mask <= full_mask; ++mask) {
            std::vector<Node*> left, right;
            for (size_t i = 0; i < node->children.size(); ++i) {
                if (mask & (1 << i)) { 
                    left.push_back(node->children[i]);
                } else {
                    right.push_back(node->children[i]);
                }
            }

            Rectangle left_rect, right_rect;
            for (auto item: left) Geometry::update_rectangle(left_rect, item);
            for (auto item: right) Geometry::update_rectangle(right_rect, item);

            if (std::max(left.size(), right.size()) <= max_children &&
                left_rect.area() + right_rect.area() < best_area
            ) {
                best_mask = mask;
                best_area = left_rect.area() + right_rect.area();
            }
        }

        Node* new_node = new Node(node->is_leaf);
        Node* ori_node = new Node(node->is_leaf);

        for (size_t i = 0; i < node->children.size(); ++i) {
            if (best_mask & (1 << i)) new_node.children.push_back(node->children[i]);
            else ori_node.children.push_back(node->children[i]);
        }

        node = ori_node;

        return new_node;
    }
}

void RTree::adjust_tree(Node* ori_node, Node* new_node) {
    if (ori_node == root) {
        root = new Node();
        root->children.push_back(ori_node);
        root->children.push_back(new_node);
        Geometry::update_rectangle(root->rect, ori_node->rect);
        Geometry::update_rectangle(root->rect, new_node->rect);
        return;
    } else {
        Node* parent = find_parent(ori_node);

        parent->children.push_back(new_node);

        if (parent->children.size() > max_children) {
            Node* split = split_node(parent);
            adjust_tree(parent, split);
        }
    }
}

void RTree::find_parent(Node* current, Node* child) {
    if (std::find(current->children.begin(), current->children.end(), child) != current->children.end()) {
        return current;
    }

    if (current->is_leaf || !Geometry::is_overlap(current->rect, child->rect)) return nullptr;

    for (Node* item: current->children) {
        Node* result = find_parent(item, child);
        if (result != nullptr) return result;
    }

    return nullptr;
}

void RTree::clear() {
    delete root;
    root = new Node(true);
}

void RTree::insert(const Point& point) {
    Node* target = choose_leaf(root, point);

    update_rectangle(target->rect, point);
    target->update_point_size();

    if (target->points > max_children) {
        Node* split = split_node(target);
        adjust_tree(target, split);
    }
}

void RTree::remove_point(const std::tuple<double, double, int>& val) {
    Geometry::Point point;
    point.x = std::get<0>(val);
    point.y = std::get<1>(val);
    point.id = std::get<2>(val);
    remove_point_recursive(root, point);
}

bool RTree::remove_point_recursive(Node* node, const Geometry::Point& point) {
    if (node->is_leaf) {
        auto it = std::find(node->points.begin(), node->points.end(), point);
        if (it != node->points.end()) {
            node->points.erase(it);
            node->point_size--;
            return true;
        }
    } else {
        for (auto* child : node->children) {
            if (is_inside(child->rect, point)) {
                if (remove_point_recursive(child, point)) {
                    node->point_size--;
                    // If the child node became empty, remove it from the parent node
                    if (child->points.empty()) {
                        auto child_it = std::find(node->children.begin(), node->children.end(), child);
                        node->children.erase(child_it);
                    }

                    // Update the bounding rectangle of the parent node
                    if (!node->children.empty()) {
                        node->rect.x_min = node->children[0]->rect.x_min;
                        node->rect.y_min = node->children[0]->rect.y_min;
                        node->rect.x_max = node->children[0]->rect.x_max;
                        node->rect.y_max = node->children[0]->rect.y_max;

                        for (size_t i = 1; i < node->children.size(); ++i) {
                            Geometry::update_rectangle(node->rect, node->children[i]->rect);
                        }
                    }

                    return true;
                }
            }
        }
    }

    return false;
}
