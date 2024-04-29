
#include <algorithm>
#include <fstream>
#include <random>

#include "model.h"


void Model::initialize(std::filesystem::path configuration_file)
{
    auto cfs = std::ifstream{ configuration_file };
    initialize(json::parse(cfs));
}

void Model::initialize(const nlohmann::json& configuration)
{
    time = 0.0;
    static_cast<void>(configuration);
    // initialize the simulation!
}

void Model::update(const double delta_time)
{    
    time += delta_time;
    // simulate all of the entities and their gravitational forces!
}

void Model::finalize()
{
    entities.clear();
}
