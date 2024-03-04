#include <fmt/format.h>

#include "mineral.h"
#include "worker.h"

auto main() -> int
{
    auto mineral = Mineral{ 10.0, 10.0 };
    auto worker = Worker{ .capacity = 15 };

    fmt::print("{} {} {} {} {}\n", 0.0, "mineral", mineral.position_x, mineral.position_y, mineral.units);
    fmt::print("{} {} {} {} {}\n", 0.0, "worker", worker.position_x, worker.position_y, worker.units);

    for (auto time = double{ 1.0 }; time < 1000.0; time += 1.0)
    {
        worker.update(mineral);
        fmt::print("{} {} {:.4f} {:.4f} {}\n", time, "mineral", mineral.position_x, mineral.position_y, mineral.units);
        fmt::print("{} {} {:.4f} {:.4f} {}\n", time, "worker", worker.position_x, worker.position_y, worker.units);
    }
}
