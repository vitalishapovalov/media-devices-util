#pragma once

#include <Mmdeviceapi.h>
#include <string>

class DefaultAudioDeviceWin {
    private:
        bool is_initialized = false;
        IMMDevice* mm_device = NULL;
        IMMDeviceEnumerator* mm_device_enumerator = NULL;
        IPropertyStore* props_store = NULL;
    public:
        std::string friendly_name;
        DefaultAudioDeviceWin();
        ~DefaultAudioDeviceWin();
};
