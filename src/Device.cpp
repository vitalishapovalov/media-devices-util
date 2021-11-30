#include <string>
#include <napi.h>

#include "Device.h"

Napi::Array Device::devices_vector_to_napi_arr(
    const std::vector<Device>& devices_vector,
    const Napi::Env& env
) {
    Napi::Array devices_napi_arr = Napi::Array::New(env, devices_vector.size());
    int current_index = 0;
    for (const auto& device : devices_vector) {
        devices_napi_arr[current_index] = device.to_napi_object(env);
        current_index++;
    }
    return devices_napi_arr;
}

Device::Device() {
}

Device::Device(
    const std::string& _id,
    const std::string& _label,
    const std::string& _alternative_name
): id(_id), label(_label), alternative_name(_alternative_name) {
}

Napi::Object Device::to_napi_object(const Napi::Env& env) const {
    Napi::Object device = Napi::Object::New(env);
    if (!id.empty()) {
        device.Set("id", id);
    }
    if (!label.empty()) {
        device.Set("label", label);
    }
    if (!alternative_name.empty()) {
        device.Set("alternativeName", alternative_name);
    }
    return device;
}
