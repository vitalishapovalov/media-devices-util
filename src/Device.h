#pragma once
#include <string>

struct Device {
    std::string id;
    std::string label;
    Device();
    Device(int i, std::string l);
    Device(std::string i, std::string l);
};
