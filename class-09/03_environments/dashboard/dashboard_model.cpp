#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "model.h"
#include "entity.h"
#include "environment.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("entities", &Model::entities)
        .def_readwrite("environment", &Model::environment)
        .def_readwrite("time", &Model::time);

    pybind11::class_<Entity>(m, "Entity")
        .def_readwrite("position", &Entity::position);

    pybind11::class_<Environment>(m, "Environment")
        .def_readwrite("x_limit", &Environment::x_limit)
        .def_readwrite("y_limit", &Environment::y_limit);
}