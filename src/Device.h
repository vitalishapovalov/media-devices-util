#pragma once

#include <string>

class Device {
    public:
        std::string id;
        std::string label;
        std::string alternative_name;

        Device();
        explicit Device(
            const std::string& _id,
            const std::string& _label = std::string(),
            const std::string& _alternative_name = std::string()
        );
};
