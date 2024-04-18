#include "mc_model.h"

auto MonteCarloModel::execute_full_set(const int mcs) -> void
{
    for (auto mc = int{}; mc < mcs; ++mc)
    {
        execute_model(mc);
    }
}

auto MonteCarloModel::execute_model(const int mc) -> void
{
    auto model = Model{};
    model.initialize("data/config.json");
    auto& data = mc_data[mc];
    while (!model.simulation_complete)
    {
        model.update(0.25);
        data.insert(
            data.end(),
            model.projectiles.begin(),
            model.projectiles.end()
        );
    }
    model.finalize();
}

auto MonteCarloModel::clear() -> void
{
    mc_data.clear();
}
