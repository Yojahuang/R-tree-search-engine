#ifndef RTREE_H
#define RTREE_H

#include<vector>

template<typename T>
class RTree {
public:
    RTree();

    void load_points(std::vector<std::tuple<double, double, T>>);
    void add_point(std::tuple<double, double, T>);
    std::tuple<double, double, T> k_nearest_neighbors(int);
    void remove_point(std::tuple<double,double,T>);
    void clear();
    std::vector<std::tuple<int,int,T>> range_query(std::tuple<double,double>);
private:
};

#endif