#pragma once

#include <vector>
#include <napi.h>

#include "Device.h"

namespace ConverterUtil {

    Napi::Object device_to_napi_object(const Device& device, const Napi::Env& env);

    Napi::Array devices_vector_to_napi_arr(const std::vector<Device>& devices_vector, const Napi::Env& env);

}
