#include <iostream>

#include "log.hpp"

int main(int args, char** argv) {

    auto now = std::chrono::system_clock::now();

    {
        logging::UnitLog unit_log("Not device", now, "NOTICE");
    }
    {
        logging::UnitLog unit_log("Not page", now);
    }
    {
        std::string str = "Not permissions";
        logging::UnitLog unit_log(std::move(str), now, "WAIT");
    }
    {
        logging::UnitLog unit_log("Segmentation fault", now, "CRITICAL");
        logging::UnitLog new_unit_log(std::move(unit_log));
    }
    return 0;
}