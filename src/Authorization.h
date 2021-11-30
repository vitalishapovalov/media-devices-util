#pragma once

#include <string>
#include <napi.h>

class Authorization {
    private:
        bool _authorized;
        bool _authorization_set;
    public:
        static const std::string AUTHORIZED;
        static const std::string DENIED;
        static const std::string NOT_DETERMINED;
        static const std::string CAMERA_CATEGORY;
        static const std::string MICROPHONE_CATEGORY;
        static bool category_exists(const std::string& category);
        Authorization();
        explicit Authorization(bool authorized);
        void set_authorized(bool authorized);
        bool is_authorized();
        bool is_not_determined();
        bool is_denied();
        std::string to_string();
        Napi::String to_napi_string(const Napi::Env& env);
};
