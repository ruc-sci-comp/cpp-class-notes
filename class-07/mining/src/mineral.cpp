#include "mineral.h"

auto Mineral::mine(int requested) -> int {
    auto temp_units = int{units};
    
    if (requested >= units){
        units = 0;
        return temp_units;
    }
    else{
        units = units - requested;
        return requested;
    }

}  

auto Mineral::is_available() -> bool {
    if (units > 0){
        return true;
    }

    return false;
}   