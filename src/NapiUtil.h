#pragma once

#include <napi.h>
#include <vector>

#include "Device.h"

namespace NapiUtil {
    Napi::Object device_to_napi_object(const Device& device, const Napi::Env& env);
    Napi::Array devices_vector_to_napi_arr(const std::vector<Device>& devices_vector, const Napi::Env& env);
    Napi::Value dummy_napi_fn(const Napi::CallbackInfo& info);
}
