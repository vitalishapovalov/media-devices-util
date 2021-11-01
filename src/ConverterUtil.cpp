#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <napi.h>

#include "Device.h"
#include "ConverterUtil.h"

Napi::Array ConverterUtil::devices_vector_to_napi_arr(std::vector<Device> devices_vector, Napi::Env env) {
    Napi::Array devices_napi_arr = Napi::Array::New(env, devices_vector.size());
    int current_index = 0;
    for (Device device : devices_vector) {
        Napi::Object wrapped_device = Napi::Object::New(env);
        wrapped_device.Set("id", device.id);
        wrapped_device.Set("label", device.label);
        devices_napi_arr[current_index] = wrapped_device;
        current_index++;
    }
    return devices_napi_arr;
}

std::string ConverterUtil::wchar_to_string(const wchar_t *s) {
    std::ostringstream stm;
    const std::locale& loc = std::locale();
    char dfault = '?';
    while (*s != L'\0') {
        stm << std::use_facet<std::ctype<wchar_t>>(loc).narrow(*s++, dfault);
    }
    return stm.str();
}
