#include <string>
#include <napi.h>

class Authorization {
    private:
        bool _is_authorization_set;
        bool _is_authorized;
    public:
        static const std::string AUTHORIZED;
        static const std::string DENIED;
        static const std::string NOT_DETERMINED;
        Authorization();
        explicit Authorization(bool initial_authorization);
        void set_authorized(bool new_authorization);
        bool is_authorized();
        std::string to_string();
        Napi::String to_napi_string(const Napi::Env& env);
};
