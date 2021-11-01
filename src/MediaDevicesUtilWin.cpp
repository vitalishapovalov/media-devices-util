#include <vector>
#include <string>
#include <napi.h>
#include <windows.h>
#include <dshow.h>

#include "Device.h"
#include "ConverterUtil.h"

class MediaDevicesUtilWin : public Napi::Addon<MediaDevicesUtilWin> {
    public:
        MediaDevicesUtilWin(Napi::Env env, Napi::Object exports) {
            DefineAddon(exports, {
                InstanceMethod("getVideoDevices", &MediaDevicesUtilWin::get_video_devices),
                InstanceMethod("getAudioDevices", &MediaDevicesUtilWin::get_audio_devices)
            });
        }

    protected:
        Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
            std::vector<Device> video_devices_vector = fill_devices_list(CLSID_VideoInputDeviceCategory);
            return ConverterUtil::devices_vector_to_napi_arr(video_devices_vector, info.Env());
        }

        Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
            std::vector<Device> audio_devices_vector = fill_devices_list(CLSID_AudioInputDeviceCategory);
            return ConverterUtil::devices_vector_to_napi_arr(audio_devices_vector, info.Env());
        }

        HRESULT enumerate_devices(REFGUID category, IEnumMoniker **ppEnum) {
            // create the System Device Enumerator
            ICreateDevEnum *pDevEnum;
            HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
            if (SUCCEEDED(hr)) {
                // create an enumerator for the category
                hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
                if (hr == S_FALSE) {
                    hr = VFW_E_NOT_FOUND;  // the category is empty, treat as an error, TODO should we?
                }
                pDevEnum->Release();
            }
            return hr;
        }

        std::vector<Device> fill_devices_list(const GUID device_category) {
            std::vector<Device> available_devices;

            if (!SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
                return available_devices;
            }

            IEnumMoniker *pEnum;
            HRESULT hr = enumerate_devices(device_category, &pEnum);
            if (!SUCCEEDED(hr)) {
                return available_devices;
            }

            int r;
            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
                IPropertyBag *pPropBag;
                HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
                if (FAILED(hr)) {
                    pMoniker->Release();
                    continue;
                }

                Device deviceToAdd = Device();

                // getting display name to be used as id (the same as FFMPEG does)
                IBindCtx *bind_ctx = NULL;
                LPOLESTR olestr = NULL;
                r = CreateBindCtx(0, &bind_ctx);
                if (r == S_OK) {
                    r = pMoniker->GetDisplayName(bind_ctx, NULL, &olestr);
                    if (r == S_OK) {
                        deviceToAdd.label = ConverterUtil::wchar_to_string(olestr);
                        // replace ':' with '_' since we use : to delineate between sources
                        std::replace(deviceToAdd.label.begin(), deviceToAdd.label.end(), ':', '_');
                    }
                }

                // get description or friendly name (the same as FFMPEG does)
                VARIANT var;
                VariantInit(&var);
                hr = pPropBag->Read(L"Description", &var, 0);
                if (FAILED(hr)) {
                    hr = pPropBag->Read(L"FriendlyName", &var, 0);
                }
                if (SUCCEEDED(hr)) {
                    deviceToAdd.id = ConverterUtil::wchar_to_string(var.bstrVal);
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
};

NODE_API_ADDON(MediaDevicesUtilWin);
