import RTree
import pytest
import math

def test_k_nearest_neighbors():
    rTree = RTree.RTree()
    points = []
    for i in range(500):
        points.append((0.0, i, i))
        rTree.add_point((0.0, i, i))
    
    for i in range(100):
        val = rTree.k_nearest_neighbors((0.0, 0.0), i)
        
        expected_val = (0.0, i, i)
        for j in range(3):
            assert math.isclose(val[j], expected_val[j])

    rTree.clear()

def test_remove_point():
    rTree = RTree.RTree()
    points = []
    for i in range(500):
        points.append((0.0, i, i))
        rTree.add_point((0.0, i, i))
    
    for i in range(100):
        rTree.remove_point((0.0, i, i))

    for i in range(100):
        val = rTree.k_nearest_neighbors((0.0, 0.0), i)
        
        expected_val = (0.0, i+100, i+100)
        for j in range(3):
            assert math.isclose(val[j], expected_val[j])

    rTree.clear()