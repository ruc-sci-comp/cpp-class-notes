#include <fstream>
#include <string>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

// ok to use this namespace
using namespace nlohmann;

auto main(int argc, char** argv) -> int
{
    auto file_path = std::string{ "data/config.json" };
    if (argc > 1)
    {
        file_path = argv[1];
    }

    auto configuration_stream = std::ifstream{ file_path };
    auto configuration = json::parse(configuration_stream);
    auto length = double{ configuration["length"] };
    fmt::print("A square with side length {}m has an area of {}m^2\n", length, length * length);
}
