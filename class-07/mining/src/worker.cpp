auto work(Mineral &mineral) -> void {
    //once the entity is close enough and the thing has minerals, they call work()
    int request_amount = worker.capacity - worker.units;

    if (mine.units > 0) 
    {
        int amount_available = mineral.mine(request_amount); //can exceed the amount available but this will be handled seperately
        worker.units += amount_available;
    }

    return;
    //we reqest as much as we can for increment entity inventory
};