#include <iostream>
#include <random>

using namespace std;

auto main() -> int
{
    auto time_delta = double{ 0.1 };

    // entity spawns at (0.0, 0.0)
    auto x = double{ 0.0 };
    auto y = double{ 0.0 };

    auto dis = uniform_int_distribution<int>{ 0, 3 };
    auto eng = mt19937{ 1337 };

    for (auto time = double{ 0.0 }; time < 100.0; time += time_delta)
    {
        auto direction = dis(eng);
        if (direction == 0)
        {
            y += 1.0;
        }
        else if (direction == 1)
        {
            x += 1.0;
        }
        else if (direction == 2)
        {
            y -= 1.0;
        }
        else if (direction == 3)
        {
            x -= 1.0;
        }

        cout << time << " " << x << " " << y << endl;
    }
}

