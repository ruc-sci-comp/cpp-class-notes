#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "model.h"
#include "ant.h"
#include "anthill.h"
#include "environment.h"
#include "food.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("anthill", &Model::anthill)
        .def_readwrite("ants", &Model::ants)
        .def_readwrite("environment", &Model::environment)
        .def_readwrite("food", &Model::food)
        .def_readwrite("time", &Model::time);

    pybind11::class_<Ant>(m, "Ant")
        .def_readwrite("position", &Ant::position);

    pybind11::class_<AntHill>(m, "AntHill")
        .def_readwrite("position", &AntHill::position);

    pybind11::class_<Food>(m, "Food")
        .def_readwrite("quantity", &Food::quantity)
        .def_readwrite("position", &Food::position);

    pybind11::class_<Environment>(m, "Environment")
        .def_readwrite("x_limit", &Environment::x_limit)
        .def_readwrite("x_limit", &Environment::x_limit)
        .def_readwrite("pheremones", &Environment::pheremones);
}