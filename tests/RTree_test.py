import RTree
import pytest

def test_k_nearest_neighbors():
    rTree = RTree.RTree()
    points = []
    for i in range(500):
        points.append((0.0, i, i))
    
    rTree.load_points(points)

    for i in range(100):
        val = rTree.k_nearest_neighbors(i)
        assert val == (0.0, i, i)
