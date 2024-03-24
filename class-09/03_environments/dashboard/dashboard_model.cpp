#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "model.h"
#include "entity.h"

PYBIND11_MODULE(model, m) {
    py::class_<Model>(m, "Model")
        .def(py::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("entities", &Model::entities)
        .def_readwrite("environment", &Model::environment)
        .def_readwrite("time", &Model::time);

    py::class_<Entity>(m, "Entity")
        .def_readwrite("position", &Entity::position);
}