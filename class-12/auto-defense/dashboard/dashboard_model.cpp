#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "model.h"
#include "environment.h"
#include "projectile.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("projectiles", &Model::projectiles)
        .def_readwrite("environment", &Model::environment)
        .def_readwrite("time", &Model::time)
        .def_readwrite("simulation_complete", &Model::simulation_complete);

    pybind11::class_<Projectile>(m, "Projectile")
        .def_readwrite("position", &Projectile::position)
        .def_readwrite("velocity", &Projectile::velocity);
}