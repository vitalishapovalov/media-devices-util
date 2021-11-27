#pragma once

#include <string>

class Device {
    public:
        std::string id;
        std::string label;
        std::string alternative_name;

        Device();
        explicit Device(
            const std::string& d_id,
            const std::string& d_label = std::string(),
            const std::string& d_alternative_name = std::string()
        );
};
