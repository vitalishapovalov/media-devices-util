#pragma once

#include <vector>
#include <string>
#include <napi.h>

#include "Device.h"

namespace ConverterUtil {

    Napi::Object device_to_napi_object(Device device, Napi::Env env);

    Napi::Array devices_vector_to_napi_arr(std::vector<Device> devices_vector, Napi::Env env);

    std::string wchar_to_string(const wchar_t *s);

}
