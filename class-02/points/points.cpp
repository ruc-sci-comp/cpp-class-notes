#include <cmath>
#include <iostream>
#include <numbers>

using namespace std;
using namespace std::numbers;

auto main() -> int
{
    auto periods = int{};
    cin >> periods;

    auto delta = double{ 0.01 };
    auto t = double{ 0.0 };

    while (t < periods * pi)
    {
        cout << t * cos(t) << "," << t * sin(t) << endl;
        t += delta;
    }
}
