#include <string>

#include "Device.h"

Device::Device() {
}

Device::Device(
    const std::string& _id,
    const std::string& _label,
    const std::string& _alternative_name
): id(_id), label(_label), alternative_name(_alternative_name) {
}
