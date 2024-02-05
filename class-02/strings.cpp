#include <iostream>
#include <string>

using namespace std;

auto main() -> int
{
    cout << "What is your name?" << endl;

    auto name = string{};
    auto num = int{};

    cin >> num >> name;
    while (num--)
    {
        cout << "Hello " << name << endl;
    }
}
