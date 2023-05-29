#ifndef RTREE_H
#define RTREE_H

#include <vector>
#include "Node.h"

class RTree {
public:
    RTree();

    void add_point(std::tuple<double, double, int>);
    std::tuple<double, double, int> k_nearest_neighbors(std::tuple<double, double>, int);
    void remove_point(std::tuple<double, double, int>);
    void clear();
    std::vector<std::tuple<double, double, int>> range_query(std::tuple<double,double>, double);
private:
    int max_children;
    Node* root;

    void insert(const Point& point);
    Node* choose_leaf(Node* node, const Point& point);
    Node* split_node(Node* node);
    void adjust_tree(Node* node, Node* split_node);
    Node* find_parent(Node* current, Node* child);
    void range_query_recursive(Node* node, const Geometry::Point& point, std::vector<Geometry::Point>& result);
    bool remove_point_recursive(Node* node, const Geometry::Point& point)
};

#endif