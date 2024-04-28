#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "mc_model.h"
#include "projectile.h"

PYBIND11_MODULE(mc_model, m) {
    pybind11::class_<MonteCarloModel>(m, "MonteCarloModel")
        .def(pybind11::init<>())
        .def("execute_full_set", &MonteCarloModel::execute_full_set)
        .def("execute_model", &MonteCarloModel::execute_model)
        .def("clear", &MonteCarloModel::clear)
        .def_readwrite("mc_data", &MonteCarloModel::mc_data);

    pybind11::class_<Projectile>(m, "Projectile")
        .def_readwrite("position", &Projectile::position)
        .def_readwrite("velocity", &Projectile::velocity);
}