import RTree
import pytest
import math

def test_k_nearest_neighbors():
    rTree = RTree.RTree()
    rTree2 = RTree.RTree()
    points = []
    for i in range(500):
        points.append((0.0, i, i))
        rTree2.add_point((0.0, i, i))
    
    rTree.load_points(points)

    for i in range(100):
        val1, val2 = rTree.k_nearest_neighbors((0.0, 0.0), i), rTree2.k_nearest_neighbors((0.0, 0.0), i)
        
        expected_val = (0.0, i, i)
        for j in range(3):
            assert math.isclose(val1[j], expected_val[j])
            assert math.isclose(val2[j], expected_val[j])
            assert math.isclose(val1[j], val2[j])

    rTree.clear()
    rTree2.clear()

def test_remove_point():
    rTree = RTree.RTree()
    rTree2 = RTree.RTree()
    points = []
    for i in range(500):
        points.append((0.0, i, i))
        rTree2.add_point((0.0, i, i))
    
    rTree.load_points(points)

    for i in range(100):
        rTree.remove_point((0.0, i, i))
        rTree2.remove_point((0.0, i, i))

    for i in range(100):
        val1, val2 = rTree.k_nearest_neighbors((0.0, 0.0), i), rTree2.k_nearest_neighbors((0.0, 0.0), i)
        
        expected_val = (0.0, i+100, i+100)
        for j in range(3):
            assert math.isclose(val1[j], expected_val[j])
            assert math.isclose(val2[j], expected_val[j])
            assert math.isclose(val1[j], val2[j])

    rTree.clear()
    rTree2.clear()