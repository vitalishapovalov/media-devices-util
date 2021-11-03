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
// FIXME get_default_audio_device change ID to use alternative name
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
            Device defaultVideoDevice;

            // TODO implement

            return ConverterUtil::device_to_napi_object(defaultVideoDevice, info.Env());
        }

        Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
            Device defaultAudioDevice;
            IMMDevice* device;
            IMMDeviceEnumerator* enumerator;
            IPropertyStore *pProps = NULL;
            LPWSTR id;
            PROPVARIANT label;
            PropVariantInit(&label);

            if (
                SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))
                && SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&enumerator))
                && SUCCEEDED(enumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &device))
                && SUCCEEDED(device->OpenPropertyStore(STGM_READ, &pProps))
            ) {
                if (SUCCEEDED(device->GetId(&id))) {
                    defaultAudioDevice.id = ConverterUtil::wchar_to_string(id);
                }

                if (SUCCEEDED(pProps->GetValue(PKEY_Device_FriendlyName, &label))) {
                    defaultAudioDevice.label = ConverterUtil::wchar_to_string(label.pwszVal);
                }

                PropVariantClear(&label);
                enumerator->Release();
                device->Release();
                CoUninitialize();
            }

            return ConverterUtil::device_to_napi_object(defaultAudioDevice, info.Env());
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

            IEnumMoniker *pEnum = NULL;
            HRESULT hr = enumerate_devices(device_category, &pEnum);
            if (!SUCCEEDED(hr)) {
                return available_devices;
            }

            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
                IPropertyBag *pPropBag;
                HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
                if (FAILED(hr)) {
                    pMoniker->Release();
                    continue;
                }

                Device deviceToAdd = Device();

                IBindCtx *bind_ctx = NULL;
                LPOLESTR olestr = NULL;
                if (
                    SUCCEEDED(CreateBindCtx(0, &bind_ctx))
                    && SUCCEEDED(pMoniker->GetDisplayName(bind_ctx, NULL, &olestr))
                ) {
                    deviceToAdd.id = ConverterUtil::wchar_to_string(olestr);
                    // replace ':' with '_' since we use : to delineate between sources (the same as FFMPEG does)
                    std::replace(deviceToAdd.id.begin(), deviceToAdd.id.end(), ':', '_');
                }

                VARIANT var;
                VariantInit(&var);
                hr = pPropBag->Read(L"FriendlyName", &var, 0);
                if (SUCCEEDED(hr)) {
                    deviceToAdd.label = ConverterUtil::wchar_to_string(var.bstrVal);
                    VariantClear(&var);
                }

                available_devices.push_back(deviceToAdd);

                pPropBag->Release();
                pMoniker->Release();
            }

            pEnum->Release();

            CoUninitialize();

            return available_devices;
        }

        HRESULT enumerate_devices(REFGUID category, IEnumMoniker **ppEnum) {
            // create the System Device Enumerator
            ICreateDevEnum *pDevEnum;
            HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
            if (SUCCEEDED(hr)) {
                // create an enumerator for the category
                hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
                if (hr == S_FALSE) {
                    hr = VFW_E_NOT_FOUND;  // the category is empty, treat as an error
                }
                pDevEnum->Release();
            }
            return hr;
        }

};

NODE_API_ADDON(MediaDevicesUtilWin);
