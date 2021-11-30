#include <string>
#include <napi.h>

#include "Authorization.h"

const std::string Authorization::AUTHORIZED("AUTHORIZED");
const std::string Authorization::DENIED("DENIED");
const std::string Authorization::NOT_DETERMINED("NOT_DETERMINED");
const std::string Authorization::CAMERA_CATEGORY("camera");
const std::string Authorization::MICROPHONE_CATEGORY("microphone");

bool Authorization::category_exists(const std::string& category) {
    return Authorization::CAMERA_CATEGORY == category || Authorization::MICROPHONE_CATEGORY == category;
}

Authorization::Authorization(): _authorized(false), _authorization_set(false) {
}

Authorization::Authorization(bool authorized): _authorized(authorized), _authorization_set(true) {
}

void Authorization::set_authorized(bool authorized) {
    _authorized = authorized;
    _authorization_set = true;
}

bool Authorization::is_authorized() {
    return Authorization::AUTHORIZED == to_string();
}

bool Authorization::is_not_determined() {
    return Authorization::NOT_DETERMINED == to_string();
}

bool Authorization::is_denied() {
    return Authorization::DENIED == to_string();
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
