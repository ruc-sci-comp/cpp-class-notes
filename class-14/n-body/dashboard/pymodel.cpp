#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "model.h"
#include "entity.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("entities", &Model::entities)
        .def_readwrite("time", &Model::time);

    pybind11::class_<Entity>(m, "Entity")
        .def_readwrite("position", &Entity::position)
        .def_readwrite("mass", &Entity::mass);
}