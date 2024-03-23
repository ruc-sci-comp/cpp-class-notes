#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "model.h"
#include "entity.h"

PYBIND11_MODULE(model, m) {
    py::class_<Model>(m, "Model")
        .def(py::init<std::filesystem::path>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("entities", &Model::entities);

    py::class_<Entity>(m, "Entity")
        .def_readwrite("position", &Entity::position);
}