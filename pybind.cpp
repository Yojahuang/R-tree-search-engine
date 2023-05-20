#include "RTree.h"

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(RTree, m) {
  m.doc() = "R Tree search engine function implementation";      // module doc string
  py::class_<RTree>(m, "RTree")
    .def(py::init<>())
    .def("load_points", &RTree::load_points)
    .def("add_point", &RTree::add_point)
    .def("k_nearest_neighbors", &RTree::k_nearest_neighbors)
    .def("remove_point", &RTree::remove_point)
    .def("clear", &RTree::clear)
    .def("range_query", &RTree::range_query)
}