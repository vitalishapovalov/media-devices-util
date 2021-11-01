#pragma once
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <napi.h>

#include "Device.h"

namespace ConverterUtil {

    Napi::Array devices_vector_to_napi_arr(std::vector<Device> devices_vector, Napi::Env env);

    std::string wchar_to_string(const wchar_t *s);

}
