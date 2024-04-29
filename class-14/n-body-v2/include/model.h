#pragma once

#include <barrier>
#include <filesystem>
#include <vector>
#include <thread>

#include <nlohmann/json.hpp>

#include "entity.h"

using json = nlohmann::json;

struct Model
{
    void initialize(std::filesystem::path configuration_file);

    void initialize(const nlohmann::json& configuration);

    void update(const double delta_time);

    void finalize();

    std::vector<Entity> entities{};
    std::vector<std::jthread> threads{};
    std::barrier<> sync_point{1};
    std::size_t number_of_threads {};
    std::size_t slice_size {};

    double time{};
};