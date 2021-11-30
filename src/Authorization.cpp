#include <string>
#include <napi.h>

#include "Authorization.h"

const std::string Authorization::AUTHORIZED("AUTHORIZED");
const std::string Authorization::DENIED("DENIED");
const std::string Authorization::NOT_DETERMINED("NOT_DETERMINED");

Authorization::Authorization(): _authorized(false), _authorization_set(false) {
}

Authorization::Authorization(bool authorized): _authorized(authorized), _authorization_set(true) {
}

void Authorization::set_authorized(bool authorized) {
    _authorized = authorized;
    _authorization_set = true;
}

bool Authorization::is_authorized() {
    return to_string() == AUTHORIZED;
}

bool Authorization::is_not_determined() {
    return to_string() == NOT_DETERMINED;
}

bool Authorization::is_denied() {
    return to_string() == DENIED;
}

std::string Authorization::to_string() {
    if (!_authorization_set) {
        return NOT_DETERMINED;
    }
    return _authorized ? AUTHORIZED : DENIED;
}

Napi::String Authorization::to_napi_string(const Napi::Env& env) {
    return Napi::String::New(env, to_string());
}
