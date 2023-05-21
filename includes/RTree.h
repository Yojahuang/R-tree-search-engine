#ifndef RTREE_H
#define RTREE_H

#include<vector>
#include "Node.h"

class RTree {
public:
    RTree();

    void add_point(std::tuple<double, double, int>);
    std::tuple<double, double, int> k_nearest_neighbors(std::tuple<double, double>, int);
    void remove_point(std::tuple<double, double, int>);
    void clear();
    std::vector<std::tuple<double, double, int>> range_query(std::tuple<double,double>);
private:
    int max_children;
    Node* root;

    void insert(Node* node, const Point& point);
    Node* choose_leaf(Node* node, const Point& point);
    Node* split_node(Node* node);
    void adjust_tree(Node* node, Node* split_node);
    Node* find_parent(Node* current, Node* child);
    void k_nearest_neighbors_recursive(Node* node, const Point& point, int k, std::vector<Point>& result, double& max_distance);
    void range_query_recursive(Node* node, const Point& point, std::vector<Point>& result);
};

#endif