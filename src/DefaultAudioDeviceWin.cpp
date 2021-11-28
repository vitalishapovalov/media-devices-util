#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

#include <Functiondiscoverykeys_devpkey.h>
#include <Mmdeviceapi.h>
#include <dshow.h>
#include <windows.h>

#include "DefaultAudioDeviceWin.h"
#include "StringUtil.h"

DefaultAudioDeviceWin::DefaultAudioDeviceWin() {
    is_initialized = SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED));
    if (!is_initialized) {
        return;
    }
    if (!SUCCEEDED(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**) &mm_device_enumerator))) {
        return;
    }
    if (!SUCCEEDED(mm_device_enumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &mm_device))) {
        return;
    }
    if (!SUCCEEDED(mm_device->OpenPropertyStore(STGM_READ, &props_store))) {
        return;
    }

    PROPVARIANT friendly_name_variant;
    PropVariantInit(&friendly_name_variant);
    if (SUCCEEDED(props_store->GetValue(PKEY_Device_FriendlyName, &friendly_name_variant))) {
        friendly_name = StringUtil::wchar_to_string(friendly_name_variant.pwszVal);
    }
    PropVariantClear(&friendly_name_variant);
}

DefaultAudioDeviceWin::~DefaultAudioDeviceWin() {
    SAFE_RELEASE(mm_device_enumerator);
    SAFE_RELEASE(mm_device);
    SAFE_RELEASE(props_store);
    if (is_initialized) {
        CoUninitialize();
    }
}
