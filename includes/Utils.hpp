#ifndef UTILS_H
#define UTILS_H

#include "Geometry.h"
#include "Node.h"

namespace Utils
{
    class Wrap
    {
    public:
        bool is_point;
        Node *node;
        Geometry::Point pt, point;
        Wrap(bool is_point, Node *node, Geometry::Point pt, Geometry::Point point) : is_point(is_point), node(node), pt(pt), point(point) {}
    };

    class CMP
    {
    public:
        CMP() {}
        bool operator()(const Wrap &lhs, const Wrap &rhs) const
        {
            double lhs_distance = 0, rhs_distance = 0;

            if (lhs.is_point)
                lhs_distance = Geometry::dist(lhs.pt, lhs.point);
            else
                lhs_distance = Geometry::min_max_dist(lhs.node->rect, lhs.point);

            if (rhs.is_point)
                rhs_distance = Geometry::dist(rhs.pt, lhs.point);
            else
                rhs_distance = Geometry::min_max_dist(rhs.node->rect, lhs.point);

            return lhs_distance > rhs_distance;
        }
    };
}
#endif