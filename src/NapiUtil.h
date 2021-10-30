#pragma once
#include <vector>
#include <string>
#include <napi.h>

#include "Device.h"

namespace NapiUtil {
    Napi::Array map_devices_vector_to_napi_arr(std::vector<Device> devices_vector, Napi::Env env);
}
