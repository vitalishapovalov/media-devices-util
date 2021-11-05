#include <vector>
#include <string>
#include <napi.h>
#include <windows.h>
#include <dshow.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>

#include "Device.h"
#include "ConverterUtil.h"

// FIXME double-backslash issue
class MediaDevicesUtilWin : public Napi::Addon<MediaDevicesUtilWin> {
    public:
        MediaDevicesUtilWin(Napi::Env env, Napi::Object exports) {
            DefineAddon(exports, {
                InstanceMethod("getVideoDevices", &MediaDevicesUtilWin::get_video_devices),
                InstanceMethod("getAudioDevices", &MediaDevicesUtilWin::get_audio_devices),
                InstanceMethod("getDefaultVideoDevice", &MediaDevicesUtilWin::get_default_video_device),
                InstanceMethod("getDefaultAudioDevice", &MediaDevicesUtilWin::get_default_audio_device)
            });
        }

    protected:
        Napi::Value get_default_video_device(const Napi::CallbackInfo& info) {
            Device default_video_device;

            // TODO implement

            return ConverterUtil::device_to_napi_object(default_video_device, info.Env());
        }

        Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
            Device default_audio_device;
            IMMDevice* mm_device;
            IMMDeviceEnumerator* mm_device_enumerator;
            IPropertyStore* props_store = NULL;

            if (
                SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))
                && SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&mm_device_enumerator))
                && SUCCEEDED(mm_device_enumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &mm_device))
                && SUCCEEDED(mm_device->OpenPropertyStore(STGM_READ, &props_store))
            ) {
                LPWSTR id;
                if (SUCCEEDED(mm_device->GetId(&id))) {
                    default_audio_device.id = ConverterUtil::wchar_to_string(id);
                }

                PROPVARIANT label;
                PropVariantInit(&label);
                if (SUCCEEDED(props_store->GetValue(PKEY_Device_FriendlyName, &label))) {
                    default_audio_device.label = ConverterUtil::wchar_to_string(label.pwszVal);
                    PropVariantClear(&label);
                }

                // TODO also add alternative_name here?

                mm_device_enumerator->Release();
                mm_device->Release();
                CoUninitialize();
            }

            return ConverterUtil::device_to_napi_object(default_audio_device, info.Env());
        }

        Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
            std::vector<Device> video_devices_vector = fill_devices_list(CLSID_VideoInputDeviceCategory);
            return ConverterUtil::devices_vector_to_napi_arr(video_devices_vector, info.Env());
        }

        Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
            std::vector<Device> audio_devices_vector = fill_devices_list(CLSID_AudioInputDeviceCategory);
            return ConverterUtil::devices_vector_to_napi_arr(audio_devices_vector, info.Env());
        }

    private:
        std::vector<Device> fill_devices_list(const GUID device_category) {
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
                LPOLESTR olestr = NULL;
                if (
                    SUCCEEDED(CreateBindCtx(0, &bind_ctx))
                    && SUCCEEDED(device_moniker->GetDisplayName(bind_ctx, NULL, &olestr))
                ) {
                    device_to_add.alternative_name = ConverterUtil::wchar_to_string(olestr);
                    // replace ':' with '_' since we use : to delineate between sources (the same as FFMPEG does)
                    std::replace(device_to_add.alternative_name.begin(), device_to_add.alternative_name.end(), ':', '_');
                }

                VARIANT label_variant;
                VariantInit(&label_variant);
                if (SUCCEEDED(device_properties_bag->Read(L"FriendlyName", &label_variant, 0))) {
                    device_to_add.label = ConverterUtil::wchar_to_string(label_variant.bstrVal);
                    VariantClear(&label_variant);
                }

                available_devices.push_back(device_to_add);

                device_properties_bag->Release();
                device_moniker->Release();
            }

            devices_enum_moniker->Release();
            CoUninitialize();

            return available_devices;
        }

        HRESULT enumerate_devices(REFGUID category, IEnumMoniker** device_enum_moniker) {
            // create the System Device Enumerator
            ICreateDevEnum* dev_enum;
            HRESULT h_result = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dev_enum));
            if (SUCCEEDED(h_result)) {
                // create an enumerator for the category
                h_result = dev_enum->CreateClassEnumerator(category, device_enum_moniker, 0);
                if (h_result == S_FALSE) {
                    h_result = VFW_E_NOT_FOUND;  // the category is empty, treat as an error
                }
                dev_enum->Release();
            }
            return h_result;
        }

};

NODE_API_ADDON(MediaDevicesUtilWin);
