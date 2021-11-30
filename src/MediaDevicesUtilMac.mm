#import <AppKit/AppKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#include <napi.h>
#include <string>
#include <vector>

#include "Authorization.h"
#include "Device.h"
#include "StringUtil.h"

const std::string CAPTURE_SCREEN_PREFIX("Capture screen ");

Device map_avdevice_to_device(const AVCaptureDevice* avdevice) {
    std::string unique_id([[avdevice uniqueID] UTF8String]);
    std::string localized_name([[avdevice localizedName] UTF8String]);
    return Device(unique_id, localized_name);
}

Napi::Object get_default_device(const AVMediaType media_type, const Napi::Env& env) {
    const AVCaptureDevice* device = [AVCaptureDevice defaultDeviceWithMediaType:media_type];
    if (!device) {
        return Napi::Object::New(env);
    }
    return map_avdevice_to_device(device).to_napi_object(env);
}

Authorization get_authorization(const std::string& media_category) {
    Authorization authorization;
    if (!Authorization::category_exists(media_category)) {
        return authorization;
    }

    const AVMediaType media_type = Authorization::CAMERA_CATEGORY == media_category
        ? AVMediaTypeVideo
        : AVMediaTypeAudio;
    const AVAuthorizationStatus auth_status = [AVCaptureDevice authorizationStatusForMediaType:media_type];
    if (AVAuthorizationStatusAuthorized == auth_status) {
        authorization.set_authorized(true);
    } else if (AVAuthorizationStatusDenied == auth_status || AVAuthorizationStatusRestricted == auth_status) {
        authorization.set_authorized(false);
    }
    return authorization;
}

Napi::Value get_default_video_device(const Napi::CallbackInfo& info) {
    return get_default_device(AVMediaTypeVideo, info.Env());
}

Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
    return get_default_device(AVMediaTypeAudio, info.Env());
}

Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
    uint32_t screens_count = 0;
    CGGetActiveDisplayList(0, NULL, &screens_count);

    NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    NSArray* devices_muxed = [AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed];
    std::vector<Device> available_devices;

    available_devices.reserve([devices count] + [devices_muxed count] + screens_count);

    for (const AVCaptureDevice* device in devices) {
        available_devices.push_back(map_avdevice_to_device(device));
    }
    for (const AVCaptureDevice* device in devices_muxed) {
        available_devices.push_back(map_avdevice_to_device(device));
    }

    if (screens_count > 0) {
        CGDirectDisplayID screens[screens_count];
        CGGetActiveDisplayList(screens_count, screens, &screens_count);
        for (int i = 0; i < (int) screens_count; i++) {
            available_devices.emplace_back(std::to_string(screens[i]), CAPTURE_SCREEN_PREFIX + std::to_string(i));
        }
    }

    return Device::devices_vector_to_napi_arr(available_devices, info.Env());
}

Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
    NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeAudio];

    std::vector<Device> available_devices;
    available_devices.reserve([devices count]);
    for (const AVCaptureDevice* device in devices) {
        available_devices.push_back(map_avdevice_to_device(device));
    }

    return Device::devices_vector_to_napi_arr(available_devices, info.Env());
}

Napi::Value get_screen_authorization_status(const Napi::CallbackInfo& info) {
    Authorization authorization;

    if (@available(macOS 11.0, *)) {
        authorization.set_authorized(CGPreflightScreenCaptureAccess());
    } else if (@available(macOS 10.15, *)) {
        NSRunningApplication* current_app = NSRunningApplication.currentApplication;
        NSNumber* current_process_id = [NSNumber numberWithInteger:current_app.processIdentifier];
        CFArrayRef window_list = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);
        int windows_count = CFArrayGetCount(window_list);
        for (int index = 0; index < windows_count; index++) {
            // get information for each window
            NSDictionary* window_info = (NSDictionary*)CFArrayGetValueAtIndex(window_list, index);
            NSString* window_name = window_info[(id)kCGWindowName];
            NSNumber* process_id = window_info[(id)kCGWindowOwnerPID];
            // don't check windows owned by the current process
            if (![process_id isEqual:current_process_id]) {
                // get process information for each window
                pid_t pid = process_id.intValue;
                NSRunningApplication* running_app = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
                if (running_app) {
                    NSString* executable = running_app.executableURL.lastPathComponent;
                    if (window_name && ![executable isEqual:@"Dock"]) {
                        authorization.set_authorized(true);
                        break;
                    }
                }
            }
        }
        CFRelease(window_list);
    } else {
        authorization.set_authorized(true);
    }

    return authorization.to_napi_string(info.Env());
}

Napi::Value get_media_authorization_status(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() > 0 && info[0].IsString()) {
        std::string media_category = StringUtil::napi_value_to_string(info[0]);
        return get_authorization(media_category).to_napi_string(env);
    }

    Napi::Object authorization_obj = Napi::Object::New(env);
    authorization_obj.Set(Authorization::CAMERA_CATEGORY,
        get_authorization(Authorization::CAMERA_CATEGORY).to_napi_string(env));
    authorization_obj.Set(Authorization::MICROPHONE_CATEGORY,
        get_authorization(Authorization::MICROPHONE_CATEGORY).to_napi_string(env));
    return authorization_obj;
}

Napi::Value request_screen_authorization(const Napi::CallbackInfo& info) {
    Authorization authorization;

    if (@available(macOS 11.0, *)) {
        authorization.set_authorized(CGRequestScreenCaptureAccess());
    } else if (@available(macOS 10.15, *)) {
        CGDisplayStreamRef stream_ref = CGDisplayStreamCreate(CGMainDisplayID(), 1, 1, kCVPixelFormatType_32BGRA, NULL,
            ^(CGDisplayStreamFrameStatus status, uint64_t display_time, IOSurfaceRef surface_ref, CGDisplayStreamUpdateRef stream_update_ref){});
        if (stream_ref) {
          CFRelease(stream_ref);
          authorization.set_authorized(true);
        }
    } else {
        authorization.set_authorized(true);
    }

    if (!authorization.is_authorized()) {
        NSWorkspace* workspace = [[NSWorkspace alloc] init];
        NSString* screen_permissions_url =
            @"x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture";
        [workspace openURL:[NSURL URLWithString:screen_permissions_url]];
    }

    return authorization.to_napi_string(info.Env());
}

Napi::Value request_media_authorization(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    if (info.Length() < 0 || !info[0].IsString()) {
        deferred.Resolve(env.Undefined());
        return deferred.Promise();
    }

    Authorization authorization;
    Napi::ThreadSafeFunction thread_safe_fn = Napi::ThreadSafeFunction::New(env,
        Napi::Function::New(env, [](const Napi::CallbackInfo& info){}), "callback", 0, 1);
    if (@available(macOS 10.14, *)) {
        std::string media_category = StringUtil::napi_value_to_string(info[0]);
        if (!Authorization::category_exists(media_category)) {
            deferred.Resolve(env.Undefined());
            return deferred.Promise();
        }

        const AVMediaType media_type = Authorization::CAMERA_CATEGORY == media_category
            ? AVMediaTypeVideo
            : AVMediaTypeAudio;
        Authorization current_authorization = get_authorization(media_category);
        if (current_authorization.is_not_determined()) {
            __block Napi::ThreadSafeFunction tsf = thread_safe_fn;
            [AVCaptureDevice requestAccessForMediaType:media_type completionHandler: ^(BOOL granted) {
                auto callback = [=](Napi::Env env, Napi::Function js_cb, const std::string& authorization_string) {
                    deferred.Resolve(Napi::String::New(env, authorization_string));
                };
                tsf.BlockingCall(granted ? "AUTHORIZED" : "DENIED", callback);
                tsf.Release();
            }];
        } else if (current_authorization.is_denied()) {
            NSWorkspace* workspace = [[NSWorkspace alloc] init];
            NSString* media_permissions_url =
                [NSString stringWithFormat:@"x-apple.systempreferences:com.apple.preference.security?%@",
                          Authorization::CAMERA_CATEGORY == media_category ? @"Privacy_Camera" : @"Privacy_Microphone"];
            [workspace openURL:[NSURL URLWithString:media_permissions_url]];
            thread_safe_fn.Release();
            authorization.set_authorized(false);
            deferred.Resolve(authorization.to_napi_string(env));
        } else {
            thread_safe_fn.Release();
            deferred.Resolve(current_authorization.to_napi_string(env));
        }
    } else {
        thread_safe_fn.Release();
        authorization.set_authorized(true);
        deferred.Resolve(authorization.to_napi_string(env));
    }

    return deferred.Promise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getDefaultVideoDevice"), Napi::Function::New(env, get_default_video_device));
  exports.Set(Napi::String::New(env, "getDefaultAudioDevice"), Napi::Function::New(env, get_default_audio_device));
  exports.Set(Napi::String::New(env, "getVideoDevices"), Napi::Function::New(env, get_video_devices));
  exports.Set(Napi::String::New(env, "getAudioDevices"), Napi::Function::New(env, get_audio_devices));
  exports.Set(Napi::String::New(env, "getScreenAuthorizationStatus"), Napi::Function::New(env, get_screen_authorization_status));
  exports.Set(Napi::String::New(env, "getMediaAuthorizationStatus"), Napi::Function::New(env, get_media_authorization_status));
  exports.Set(Napi::String::New(env, "requestScreenAuthorization"), Napi::Function::New(env, request_screen_authorization));
  exports.Set(Napi::String::New(env, "requestMediaAuthorization"), Napi::Function::New(env, request_media_authorization));
  return exports;
}

NODE_API_MODULE(MediaDevicesUtilMac, Init);
