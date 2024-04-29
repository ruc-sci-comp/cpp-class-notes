#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

#include "model.h"
#include "entity.h"

PYBIND11_MODULE(model, m) {
    pybind11::class_<Model>(m, "Model")
        .def(pybind11::init<>())
        .def("initialize", pybind11::overload_cast<std::filesystem::path>(&Model::initialize))
        .def("initialize", pybind11::overload_cast<const nlohmann::json&>(&Model::initialize))
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def_readwrite("entities", &Model::entities)
        .def_readwrite("time", &Model::time);

    pybind11::class_<Entity>(m, "Entity")
        .def_readwrite("position", &Entity::position)
        .def_readwrite("velocity", &Entity::velocity)
        .def_readwrite("mass", &Entity::mass);
}