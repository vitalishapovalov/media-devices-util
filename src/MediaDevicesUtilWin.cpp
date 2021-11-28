#include <Functiondiscoverykeys_devpkey.h>
#include <Mmdeviceapi.h>
#include <dshow.h>
#include <napi.h>
#include <string>
#include <vector>
#include <windows.h>

#include "DefaultAudioDeviceWin.h"
#include "Device.h"
#include "NapiUtil.h"
#include "StringUtil.h"

HRESULT enumerate_devices(REFGUID device_category, IEnumMoniker** device_enum_moniker) {
    ICreateDevEnum* dev_enum;
    HRESULT h_result = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dev_enum));
    if (SUCCEEDED(h_result)) {
        h_result = dev_enum->CreateClassEnumerator(device_category, device_enum_moniker, 0);
        if (h_result == S_FALSE) {
            h_result = VFW_E_NOT_FOUND;  // the device_category is empty, treat as an error
        }
        dev_enum->Release();
    }
    return h_result;
}

std::vector<Device> get_devices(REFGUID device_category) {
    std::vector<Device> available_devices;

    if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
        return available_devices;
    }

    IEnumMoniker* devices_enum_moniker = NULL;
    HRESULT enumeration_result = enumerate_devices(device_category, &devices_enum_moniker);
    if (!SUCCEEDED(enumeration_result)) {
        return available_devices;
    }

    IMoniker *device_moniker = NULL;
    while (devices_enum_moniker->Next(1, &device_moniker, NULL) == S_OK) {
        IPropertyBag* device_properties_bag;
        HRESULT h_result = device_moniker->BindToStorage(0, 0, IID_PPV_ARGS(&device_properties_bag));
        if (FAILED(h_result)) {
            device_moniker->Release();
            continue;
        }

        Device device_to_add;

        IBindCtx* bind_ctx = NULL;
        LPOLESTR alternative_name_olestr = NULL;
        if (
            SUCCEEDED(CreateBindCtx(0, &bind_ctx))
            && SUCCEEDED(device_moniker->GetDisplayName(bind_ctx, NULL, &alternative_name_olestr))
        ) {
            device_to_add.alternative_name = StringUtil::wchar_to_string(alternative_name_olestr);
            // replace ':' with '_' since we use : to delineate between sources (the same as FFMPEG does)
            std::replace(device_to_add.alternative_name.begin(), device_to_add.alternative_name.end(), ':', '_');
        }

        VARIANT label_variant;
        VariantInit(&label_variant);
        if (SUCCEEDED(device_properties_bag->Read(L"FriendlyName", &label_variant, 0))) {
            device_to_add.label = StringUtil::wchar_to_string(label_variant.bstrVal);
        }
        VariantClear(&label_variant);

        VARIANT id_variant;
        VariantInit(&id_variant);
        if (CLSID_AudioInputDeviceCategory == device_category) {
            if (SUCCEEDED(device_properties_bag->Read(L"WaveInID", &id_variant, 0))) {
                device_to_add.id = std::to_string(id_variant.lVal);
            }
        } else if (CLSID_VideoInputDeviceCategory== device_category) {
            if (SUCCEEDED(device_properties_bag->Read(L"DevicePath", &id_variant, 0))) {
                device_to_add.id = StringUtil::wchar_to_string(id_variant.bstrVal);
            }
        }
        VariantClear(&id_variant);

        available_devices.push_back(device_to_add);

        device_properties_bag->Release();
        device_moniker->Release();
    }

    devices_enum_moniker->Release();
    CoUninitialize();

    return available_devices;
}

Napi::Value get_default_video_device(const Napi::CallbackInfo& info) {
    std::vector<Device> video_devices = get_devices(CLSID_VideoInputDeviceCategory);
    if (video_devices.empty()) {
        return Napi::Object::New(info.Env());
    }
    return NapiUtil::device_to_napi_object(video_devices.front(), info.Env());
}

Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
    DefaultAudioDeviceWin default_audio_device_win;
    std::vector<Device> dshow_audio_devices = get_devices(CLSID_AudioInputDeviceCategory);
    for (const auto& audio_device : dshow_audio_devices) {
        if (audio_device.label == default_audio_device_win.friendly_name) {
            return NapiUtil::device_to_napi_object(audio_device, info.Env());
        }
    }
    return Napi::Object::New(info.Env());
}

// TODO add screens option?
Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
    std::vector<Device> video_devices = get_devices(CLSID_VideoInputDeviceCategory);
    return NapiUtil::devices_vector_to_napi_arr(video_devices, info.Env());
}

Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
    std::vector<Device> audio_devices = get_devices(CLSID_AudioInputDeviceCategory);
    return NapiUtil::devices_vector_to_napi_arr(audio_devices, info.Env());
}

// TODO implement
Napi::Value get_screen_authorization_status(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}

// TODO implement
Napi::Value get_media_authorization_status(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}

// TODO implement
Napi::Value request_screen_authorization(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}

// TODO implement
Napi::Value request_media_authorization(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
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

NODE_API_MODULE(MediaDevicesUtilWin, Init);
