#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include <napi.h>

#include "Device.h"
#include "ConverterUtil.h"

Napi::Object ConverterUtil::device_to_napi_object(Device device, Napi::Env env) {
    Napi::Object wrapped_device = Napi::Object::New(env);
    wrapped_device.Set("id", device.id);
    wrapped_device.Set("label", device.label);
    return wrapped_device;
}

Napi::Array ConverterUtil::devices_vector_to_napi_arr(std::vector<Device> devices_vector, Napi::Env env) {
    Napi::Array devices_napi_arr = Napi::Array::New(env, devices_vector.size());
    int current_index = 0;
    for (Device device : devices_vector) {
        devices_napi_arr[current_index] = device_to_napi_object(device, env);
        current_index++;
    }
    return devices_napi_arr;
}

std::string ConverterUtil::wchar_to_string(const wchar_t *s) {
    std::ostringstream string_stream;
    const std::locale& loc = std::locale();
    char dfault = '?';
    while (*s != L'\0') {
        string_stream << std::use_facet<std::ctype<wchar_t>>(loc).narrow(*s++, dfault);
    }
    return string_stream.str();
}
