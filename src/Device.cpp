#include <string>

#include "Device.h"

Device::Device() { }

Device::Device(int i, std::string l) {
    id = std::to_string(i);
    label = l;
}

Device::Device(std::string i, std::string l) {
    id = i;
    label = l;
}
