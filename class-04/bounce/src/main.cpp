#include <iostream>
#include <vector>

using namespace std;

auto main() -> int
{
    auto delta_time = double{ 0.1 };

    auto xs = vector<double>{ 0,  1,  2,  3,   4 };
    auto ys = vector<double>{ 60, 70, 80, 90, 100 };
    auto vs = vector<double>{ 0, 0, 0, 0, 0 };
    auto a = double{ -9.81 };
    auto r = double{ 0.8 };

    for (auto time = double{}; time < 100.0; time += delta_time)
    {
        for (auto i = size_t{}; i < xs.size(); ++i)
        {
            auto x = xs[i];
            auto y = ys[i];
            auto v = vs[i];

            auto dy = v * delta_time;
            if (y + dy <= 0.0)
            {
                auto time_to_impact = y / v;
                auto time_after_bounce = delta_time - time_to_impact;
                auto velocity_at_impact = v + a * time_to_impact;
                v = -velocity_at_impact * r;
                y = v * time_after_bounce;
                v += a * time_after_bounce;
            }
            else
            {
                y += v * delta_time;
                v += a * delta_time;
            }

            ys[i] = y;
            vs[i] = v;

            cout << time << " " << i << " " << x << " " << y << endl;
        }
    }
}


