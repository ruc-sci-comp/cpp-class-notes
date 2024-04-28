#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include "mc_model.h"
#include "model.h"
#include "environment.h"
#include "projectile.h"
#include "radar_system.h"
#include "weapons_system.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<MonteCarloModel>(m, "MonteCarloModel")
        .def(pybind11::init<>())
        .def("execute_full_set", &MonteCarloModel::execute_full_set)
        .def("execute_model", &MonteCarloModel::execute_model)
        .def("clear", &MonteCarloModel::clear)
        .def_readwrite("mc_data", &MonteCarloModel::mc_data);

    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("projectiles", &Model::projectiles)
        .def_readwrite("environment", &Model::environment)
        .def_readwrite("radar_system", &Model::radar_system)
        .def_readwrite("weapons_system", &Model::weapons_system)
        .def_readwrite("time", &Model::time)
        .def_readwrite("simulation_complete", &Model::simulation_complete);

    pybind11::class_<Projectile>(m, "Projectile")
        .def_readwrite("position", &Projectile::position)
        .def_readwrite("velocity", &Projectile::velocity);

    pybind11::class_<RadarSystem>(m, "RadarSystem")
        .def_readwrite("position", &RadarSystem::position)
        .def_readwrite("detection_range", &RadarSystem::detection_range)
        .def_readwrite("tracking", &RadarSystem::tracking)
        .def_readwrite("track_object", &RadarSystem::track_object);

    pybind11::class_<WeaponsSystem>(m, "WeaponsSystem")
        .def_readwrite("position", &WeaponsSystem::position)
        .def_readwrite("engage_range", &WeaponsSystem::engage_range)
        .def_readwrite("interceptors", &WeaponsSystem::interceptors);
}