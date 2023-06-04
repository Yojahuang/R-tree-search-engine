import RTree
import pytest
import math

def test_range_query_with_remove_point():
    rTree = RTree.RTree()
    expected_val = []
    for i in range(10):
       rTree.add_point((0.0, i, i))

    for i in range(5):
        rTree.remove_point((0.0, i, i))

    for i in range(5, 10, 1):
        expected_val.append((0.0, i, i))

    points = rTree.range_query((0,0), 10)

    assert len(points) == 5

    for val in expected_val:
        find = False
        for i in range(len(points)):
            if val == points[i]:
                find = True
        assert find == True
    
    rTree.clear()

def test_range_query():
    rTree = RTree.RTree()
    expected_val = []
    for i in range(10):
        rTree.add_point((0.0, i, i))
        expected_val.append((0.0, i, i))

    points = rTree.range_query((0,0), 10)

    assert len(points) == 10

    for val in expected_val:
        find = False
        for i in range(len(points)):
            if val == points[i]:
                find = True
        assert find == True
    
    rTree.clear()

def test_k_nearest_neighbors():
    rTree = RTree.RTree()
    for i in range(50):
        rTree.add_point((0.0, i, i))
    
    for i in range(50):
        val = rTree.k_nearest_neighbors((0.0, 0.0), i+1)
        
        expected_val = (0.0, i, i)
        for j in range(3):
            assert math.isclose(val[j], expected_val[j])

    rTree.clear()
