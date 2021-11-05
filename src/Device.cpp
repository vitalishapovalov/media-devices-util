#include <string>

#include "Device.h"

Device::Device() {
}

Device::Device(int i, std::string l, std::string a) {
    id = std::to_string(i);
    label = l;
    alternative_name = a;
}
