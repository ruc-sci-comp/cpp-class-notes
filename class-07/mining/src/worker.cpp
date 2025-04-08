#include "worker.h" 

auto Worker::work(Mineral& mineral) -> void {
    //once the entity is close enough and the thing has minerals, they call work()
    auto request_amount = int {Worker.capacity - Worker.units};

    if (mine.units > 0)
    {
        auto amount_available = int {mineral.mine(request_amount)}; //can exceed the amount available but this will be handled seperately
        Worker.units += amount_available;
    }

    return;
    //we reqest as much as we can for increment entity inventory
};