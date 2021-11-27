#include <string>

#include "Device.h"

Device::Device() { }

Device::Device(
    const std::string& d_id,
    const std::string& d_label,
    const std::string& d_alternative_name
): id(d_id),
   label(d_label),
   alternative_name(d_alternative_name) { }
