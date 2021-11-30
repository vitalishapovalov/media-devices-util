#include <codecvt>
#include <locale>
#include <napi.h>
#include <string>

#include "StringUtil.h"

std::string StringUtil::wchar_to_string(const wchar_t* wchar_value) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wchar_value);
}

std::string StringUtil::napi_value_to_string(const Napi::Value& napi_value) {
    return napi_value.As<Napi::String>().ToString().Utf8Value();
}
