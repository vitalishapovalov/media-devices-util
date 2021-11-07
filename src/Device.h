#pragma once

#include <string>

struct Device {
    std::string id;
    std::string label;
    std::string alternative_name;

    Device();
    Device(int i, std::string l = "", std::string a = "");
};
