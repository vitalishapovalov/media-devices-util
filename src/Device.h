#pragma once

#include <string>
#include <napi.h>

class Device {
    public:
        static Napi::Array devices_vector_to_napi_arr(
            const std::vector<Device>& devices_vector,
            const Napi::Env& env
        );
        std::string id;
        std::string label;
        std::string alternative_name;
        Device();
        explicit Device(
            const std::string& _id,
            const std::string& _label = std::string(),
            const std::string& _alternative_name = std::string()
        );
        Napi::Object to_napi_object(const Napi::Env& env) const;
};
