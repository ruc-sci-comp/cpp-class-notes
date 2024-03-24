#include <fstream>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

// ok to use this namespace
using namespace nlohmann;

auto main() -> int
{
    // first we need to open a file. to do so we need to use a file stream,
    // which we can get access to by including fstream. we can create an
    // ifstream (input file stream) and give it a file name/path
    auto configuration_stream = std::ifstream{ "data/config.json" };

    // using nlohmann json to read the file
    auto configuration = json::parse(configuration_stream);

    // access the parameter "length" from the configuration
    // we must tell the JSON what variable type we are reading!
    auto length = double{ configuration["length"] };

    // use length like any normal variable!
    fmt::print("A square with side length {}m has an area of {}m^2\n", length, length * length);
}
