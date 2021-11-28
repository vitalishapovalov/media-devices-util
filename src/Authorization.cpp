#include <string>
#include <napi.h>

#include "Authorization.h"

const std::string Authorization::AUTHORIZED = "AUTHORIZED";
const std::string Authorization::DENIED = "DENIED";
const std::string Authorization::NOT_DETERMINED = "NOT_DETERMINED";

Authorization::Authorization() {
    _is_authorization_set = false;
    _is_authorized = false;
}

Authorization::Authorization(bool initial_authorization): _is_authorized(initial_authorization) {
    _is_authorization_set = true;
}

void Authorization::set_authorized(bool new_authorization) {
    _is_authorized = new_authorization;
    _is_authorization_set = true;
}

bool Authorization::is_authorized() {
    return _is_authorized;
}

std::string Authorization::to_string() {
    if (!_is_authorization_set) {
        return NOT_DETERMINED;
    }
    return _is_authorized ? AUTHORIZED : DENIED;
}

Napi::String Authorization::to_napi_string(const Napi::Env& env) {
    return Napi::String::New(env, to_string());
}
