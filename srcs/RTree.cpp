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

std::vector<std::tuple<double, double, int>> range_query(std::tuple<double, double> point, double distance) {
    Geometry::Point search_point;
    search_point.x = std::get<0>(point);
    search_point.y = std::get<1>(point);

    std::vector<Geometry::Point> points;
    std::vector<std::tuple<double, double, int>> result;

    range_query_recursive(root, search_point, distance, points);

    for (auto pt: points) {
        result.push_back(std::make_tuple(pt.x, pt.y, pt.id));
    }

    return result;


void RTree::range_query_recursive(Node* node, const Geometry::Point& search_point, double distance, std::vector<Geometry::Point>& result) {
    if (node->is_leaf) {
        for (const auto& leaf_point : node->points) {
            if (Geometry::dist(leat_point, search_point) <= distance) {
                result.push_back(leaf_point);
            }
        }
    } else {
        for (auto* child : node->children) {
            if (Geometry::min_dist(child->rect, search_point) <= distance) {
                range_query_recursive(child, search_point, distance, result);
            }
        }
    }
}}

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

                    node->rect.x_min = std::numeric_limits<double>::max();
                    node->rect.x_max = std::numeric_limits<double>::lowest();
                    node->rect.y_min = std::numeric_limits<double>::max();
                    node->rect.y_max = std::numeric_limits<double>::lowest();

                    // Update the bounding rectangle of the parent node
                    for (size_t i = 0; i < node->children.sizE(); ++i) {
                        Geometry::update_rectangle(node->rect, node->children[i]->rect);
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

std::tuple<double, double, int> RTree::k_nearest_neighbors(const std::tuple<double, double>& point, int k) {
    class Wrap {
    public:
        bool is_point;
        Node* node;
        Geometry::Point pt;
        Wrap(bool is_point, Node* node, Geometry::Point pt): is_point(is_point), node(node), pt(pt) {
        }
    };
    
    auto compare = []( Wrap lhs, Wrap rhs ) {
        double lhs_distance = 0, rhs_distance = 0;

        if (lhs->is_point)
            lhs_distance = Geometry::dist(lhs.pt, point);
        else 
            lhs_distance = Geometry::min_max_dist(lhs.node->rect, point);

        if (rhs->is_point)
            rhs_distance = Geometry::dist(rhs.pt, point);
        else
            rhs_distance = Geometry::min_max_dist(rhs.node->rect, point);
        

        return lhs_distance < rhs_distance;
    };

    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq;

    pq.push(Wrap(false, root, Geometry::Point()));

    int cnt = k;
    while (cnt > 0 && !pq.empty()) {
        Wrap top = pq.top(); pq.pop();
        if (top.is_point) {
            cnt--;
            if (cnt == 0) {
                return std::make_tuple(pt.x, pt.y, pt.id);
            } 
        } else {
            // open the box and put it back
            Node* node = top.node;

            if (node->is_leaf) {
                for (auto pt: node->points) {
                    pq.push(Wrap(true, Node(), pt));
                }
            } else {
                for (auto child: node->children) {
                    pq.push(Wrap(false, child, Geometry::Point()));
                }
            }
        }
    }

    return std::make_tuple(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), -1);
}
