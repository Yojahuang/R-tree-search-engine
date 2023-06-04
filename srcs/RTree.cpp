#include <vector>
#include <tuple>
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <queue>
#include "RTree.h"
#include "Node.h"
#include "Utils.hpp"

RTree::RTree() : max_children(8), root(new Node(true)) {}

void RTree::add_point(const std::tuple<double, double, int> &val)
{
    Geometry::Point point;
    point.x = std::get<0>(val);
    point.y = std::get<1>(val);
    point.id = std::get<2>(val);

    insert(point);
    root->update_point_size();
}

std::vector<std::tuple<double, double, int>> RTree::range_query(std::tuple<double, double> point, double distance)
{
    Geometry::Point search_point;
    search_point.x = std::get<0>(point);
    search_point.y = std::get<1>(point);

    std::vector<Geometry::Point> points;
    std::vector<std::tuple<double, double, int>> result;

    range_query_recursive(root, search_point, distance, points);

    for (auto pt : points)
    {
        result.push_back(std::make_tuple(pt.x, pt.y, pt.id));
    }

    return result;
}

void RTree::range_query_recursive(Node *node, const Geometry::Point &search_point, double distance, std::vector<Geometry::Point> &result)
{
    if (node->is_leaf)
    {
        for (const auto &leaf_point : node->points)
        {
            if (Geometry::dist(leaf_point, search_point) <= distance)
            {
                result.push_back(leaf_point);
            }
        }
    }
    else
    {
        for (auto *child : node->children)
        {
            if (Geometry::min_dist(child->rect, search_point) <= distance)
            {
                range_query_recursive(child, search_point, distance, result);
            }
        }
    }
}

Node *RTree::choose_leaf(Node *node, const Geometry::Point &point)
{
    if (node->is_leaf)
    {
        return node;
    }

    double min_enlargement = std::numeric_limits<double>::max();
    Node *selected_child = nullptr;
    for (auto *child : node->children)
    {
        Geometry::Rectangle enlarged_rect = child->rect;
        Geometry::update_rectangle(enlarged_rect, point);

        double enlarge_rect_area = enlarged_rect.area();
        double current_rect_area = child->rect.area();

        if (child->points.size() == 0)
            enlarge_rect_area = current_rect_area = 0;

        double enlargement = enlarge_rect_area - current_rect_area;
        if (enlargement < min_enlargement)
        {
            min_enlargement = enlargement;
            selected_child = child;
        }
    }

    if (selected_child != nullptr)
    {
        update_rectangle(selected_child->rect, point);
        selected_child->point_size++;
    }
    return choose_leaf(selected_child, point);
}

Node *RTree::split_node(Node *node)
{
    if (node->is_leaf)
    {
        int mask = 0, full_mask = 1 << node->points.size();

        int best_mask = 0;
        double best_area = std::numeric_limits<double>::max();
        for (mask = 0; mask <= full_mask; ++mask)
        {
            std::vector<Geometry::Point> left, right;
            for (size_t i = 0; i < node->points.size(); ++i)
            {
                if (mask & (1 << i))
                {
                    left.push_back(node->points[i]);
                }
                else
                {
                    right.push_back(node->points[i]);
                }
            }

            Geometry::Rectangle left_rect, right_rect;
            for (auto item : left)
                Geometry::update_rectangle(left_rect, item);
            for (auto item : right)
                Geometry::update_rectangle(right_rect, item);

            if (std::max(left.size(), right.size()) <= max_children &&
                left_rect.area() + right_rect.area() < best_area)
            {
                best_mask = mask;
                best_area = left_rect.area() + right_rect.area();
            }
        }

        Node *new_node = new Node(node->is_leaf);
        std::vector<Geometry::Point> points;

        for (size_t i = 0; i < node->points.size(); ++i)
        {
            if (best_mask & (1 << i))
            {
                new_node->points.push_back(node->points[i]);
                Geometry::update_rectangle(new_node->rect, node->points[i]);
            }
            else
            {
                points.push_back(node->points[i]);
            }
        }

        node->points.clear();
        node->rect.x_min = std::numeric_limits<double>::max();
        node->rect.x_max = std::numeric_limits<double>::lowest();
        node->rect.y_min = std::numeric_limits<double>::max();
        node->rect.y_max = std::numeric_limits<double>::lowest();

        for (auto pt : points)
        {
            node->points.push_back(pt);
            Geometry::update_rectangle(node->rect, pt);
        }

        new_node->update_point_size();
        node->update_point_size();
        return new_node;
    }
    else
    {
        int mask = 0, full_mask = 1 << node->children.size();

        int best_mask = 0;
        double best_area = std::numeric_limits<double>::max();
        for (mask = 0; mask <= full_mask; ++mask)
        {
            std::vector<Node *> left, right;
            for (size_t i = 0; i < node->children.size(); ++i)
            {
                if (mask & (1 << i))
                {
                    left.push_back(node->children[i]);
                }
                else
                {
                    right.push_back(node->children[i]);
                }
            }

            Geometry::Rectangle left_rect, right_rect;
            for (auto item : left)
                Geometry::update_rectangle(left_rect, item->rect);
            for (auto item : right)
                Geometry::update_rectangle(right_rect, item->rect);

            if (std::max(left.size(), right.size()) <= max_children &&
                left_rect.area() + right_rect.area() < best_area)
            {
                best_mask = mask;
                best_area = left_rect.area() + right_rect.area();
            }
        }

        Node *new_node = new Node(node->is_leaf);
        std::vector<Node *> childs;

        for (size_t i = 0; i < node->children.size(); ++i)
        {
            if (best_mask & (1 << i))
            {
                new_node->children.push_back(node->children[i]);
                Geometry::update_rectangle(new_node->rect, node->children[i]->rect);
            }
            else
            {
                childs.push_back(node->children[i]);
            }
        }

        node->children.clear();
        node->rect.x_min = std::numeric_limits<double>::max();
        node->rect.x_max = std::numeric_limits<double>::lowest();
        node->rect.y_min = std::numeric_limits<double>::max();
        node->rect.y_max = std::numeric_limits<double>::lowest();

        for (auto child : childs)
        {
            node->children.push_back(child);
            Geometry::update_rectangle(node->rect, child->rect);
        }

        node->update_point_size();

        return new_node;
    }
}

void RTree::adjust_tree(Node *ori_node, Node *new_node)
{
    if (ori_node == root)
    {
        root = new Node(false);
        root->children.push_back(ori_node);
        root->children.push_back(new_node);
        Geometry::update_rectangle(root->rect, ori_node->rect);
        Geometry::update_rectangle(root->rect, new_node->rect);
        return;
    }
    else
    {
        Node *parent = find_parent(root, ori_node);

        Geometry::update_rectangle(parent->rect, new_node->rect);
        parent->children.push_back(new_node);

        if (parent->children.size() > max_children)
        {
            Node *split = split_node(parent);
            adjust_tree(parent, split);
        }
    }
}

Node *RTree::find_parent(Node *current, Node *child)
{
    if (std::find(current->children.begin(), current->children.end(), child) != current->children.end())
    {
        return current;
    }

    if (current->is_leaf || !Geometry::is_overlap(current->rect, child->rect))
    {
        return nullptr;
    }

    for (Node *item : current->children)
    {
        Node *result = find_parent(item, child);
        if (result != nullptr)
            return result;
    }

    return nullptr;
}

void RTree::clear()
{
    delete root;
    root = new Node(true);
}

void RTree::insert(const Geometry::Point &point)
{
    Geometry::update_rectangle(root->rect, point);
    Node *target = choose_leaf(root, point);

    update_rectangle(target->rect, point);
    target->update_point_size();
    target->points.push_back(point);

    if (target->points.size() > max_children)
    {
        Node *split = split_node(target);
        adjust_tree(target, split);
    }
}

void RTree::remove_point(const std::tuple<double, double, int> &val)
{
    Geometry::Point point;
    point.x = std::get<0>(val);
    point.y = std::get<1>(val);
    point.id = std::get<2>(val);
    remove_point_recursive(root, point);
}

bool RTree::remove_point_recursive(Node *node, const Geometry::Point &point)
{
    if (node->is_leaf)
    {
        auto it = std::find(node->points.begin(), node->points.end(), point);
        if (it != node->points.end())
        {
            node->points.erase(it);
            node->point_size--;
            return true;
        }
    }
    else
    {
        for (auto *child : node->children)
        {
            if (is_inside(child->rect, point))
            {
                if (remove_point_recursive(child, point))
                {
                    node->point_size--;
                    // If the child node became empty, remove it from the parent node
                    if (child->points.empty())
                    {
                        auto child_it = std::find(node->children.begin(), node->children.end(), child);
                        node->children.erase(child_it);
                    }

                    node->rect.x_min = std::numeric_limits<double>::max();
                    node->rect.x_max = std::numeric_limits<double>::lowest();
                    node->rect.y_min = std::numeric_limits<double>::max();
                    node->rect.y_max = std::numeric_limits<double>::lowest();

                    // Update the bounding rectangle of the parent node
                    for (size_t i = 0; i < node->children.size(); ++i)
                    {
                        Geometry::update_rectangle(node->rect, node->children[i]->rect);
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

std::tuple<double, double, int> RTree::k_nearest_neighbors(const std::tuple<double, double> &point, int k)
{
    Geometry::Point special_pt(std::get<0>(point), std::get<1>(point));

    std::priority_queue<Utils::Wrap, std::vector<Utils::Wrap>, Utils::CMP> pq;

    pq.push(Utils::Wrap(false, root, Geometry::Point(), special_pt));

    int cnt = k;
    while (cnt > 0 && !pq.empty())
    {
        Utils::Wrap top = pq.top();
        pq.pop();
        Geometry::Point pt = top.pt;
        if (top.is_point)
        {
            cnt--;
            if (cnt == 0)
            {
                return std::make_tuple(pt.x, pt.y, pt.id);
            }
        }
        else
        {
            // open the box and put it back
            Node *node = top.node;

            if (node->is_leaf)
            {
                for (auto pt : node->points)
                {
                    Utils::Wrap content(true, new Node(), pt, special_pt);
                    pq.push(content);
                }
            }
            else
            {
                for (auto child : node->children)
                {
                    Utils::Wrap content(false, child, Geometry::Point(), special_pt);
                    pq.push(content);
                }
            }
        }
    }

    return std::make_tuple(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), -1);
}
