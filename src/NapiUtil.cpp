#include <napi.h>
#include <vector>

#include "Device.h"
#include "NapiUtil.h"

Napi::Object NapiUtil::device_to_napi_object(const Device& device, const Napi::Env& env) {
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

Napi::Array NapiUtil::devices_vector_to_napi_arr(const std::vector<Device>& devices_vector, const Napi::Env& env) {
    Napi::Array devices_napi_arr = Napi::Array::New(env, devices_vector.size());
    int current_index = 0;
    for (const auto& device : devices_vector) {
        devices_napi_arr[current_index] = device_to_napi_object(device, env);
        current_index++;
    }
    return devices_napi_arr;
}

Napi::Value NapiUtil::dummy_napi_fn(const Napi::CallbackInfo& info) {
  return info.Env().Undefined();
}

bool NapiUtil::is_first_arg_string(const Napi::CallbackInfo& info) {
    return info.Length() > 0 && info[0].IsString();
}

bool NapiUtil::is_first_arg_bool(const Napi::CallbackInfo& info) {
    return info.Length() > 0 && info[0].IsBoolean();
}

std::string NapiUtil::get_first_arg_string(const Napi::CallbackInfo& info) {
    return info[0].As<Napi::String>().ToString().Utf8Value();
}

bool NapiUtil::get_first_arg_bool(const Napi::CallbackInfo& info) {
    return info[0].ToBoolean().Value();
}
