#ifndef RTREE_H
#define RTREE_H

#include<vector>

class RTree {
public:
    RTree();

    void load_points(std::vector<std::tuple<double, double, int>>);
    void add_point(std::tuple<double, double, int>);
    std::tuple<double, double, int> k_nearest_neighbors(std::tuple<double, double>, int);
    void remove_point(std::tuple<double, double, int>);
    void clear();
    std::vector<std::tuple<double, double, int>> range_query(std::tuple<double,double>);
private:
};

#endif