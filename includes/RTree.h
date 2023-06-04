#ifndef RTREE_H
#define RTREE_H

#include <vector>
#include "Node.h"

class RTree
{
public:
    RTree();

    void add_point(const std::tuple<double, double, int> &);
    std::tuple<double, double, int> k_nearest_neighbors(const std::tuple<double, double> &, int);
    void remove_point(const std::tuple<double, double, int> &);
    void clear();
    std::vector<std::tuple<double, double, int>> range_query(std::tuple<double, double>, double);

private:
    size_t max_children;
    Node *root;

    void bfs();
    void insert(const Geometry::Point &point);
    Node *choose_leaf(Node *node, const Geometry::Point &point);
    Node *split_node(Node *node);
    void adjust_tree(Node *node, Node *split_node);
    Node *find_parent(Node *current, Node *child);
    void range_query_recursive(Node *node, const Geometry::Point &search_point, double distance, std::vector<Geometry::Point> &result);
    bool remove_point_recursive(Node *node, const Geometry::Point &point);
};
#endif
