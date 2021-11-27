#include <vector>
#include <napi.h>

#include "Device.h"
#include "ConverterUtil.h"

Napi::Object ConverterUtil::device_to_napi_object(const Device& device, const Napi::Env& env) {
    Napi::Object wrapped_device = Napi::Object::New(env);
    if (!device.id.empty()) {
        wrapped_device.Set("id", device.id);
    }
    if (!device.label.empty()) {
        wrapped_device.Set("label", device.label);
    }
    if (!device.alternative_name.empty()) {
        wrapped_device.Set("alternativeName", device.alternative_name);
    }
    return wrapped_device;
}

Napi::Array ConverterUtil::devices_vector_to_napi_arr(const std::vector<Device>& devices_vector, const Napi::Env& env) {
    Napi::Array devices_napi_arr = Napi::Array::New(env, devices_vector.size());
    int current_index = 0;
    for (const auto& device : devices_vector) {
        devices_napi_arr[current_index] = device_to_napi_object(device, env);
        current_index++;
    }
    return devices_napi_arr;
}
