#pragma once

#include <napi.h>
#include <string>

namespace StringUtil {
    std::string wchar_to_string(const wchar_t* wchar_value);
    std::string napi_value_to_string(const Napi::Value& napi_value);
}
