#pragma once

#include <string>
#include <Mmdeviceapi.h>

class DefaultAudioDeviceWin {
    private:
        bool is_initialized;
        IMMDevice* mm_device = NULL;
        IMMDeviceEnumerator* mm_device_enumerator = NULL;
        IPropertyStore* props_store = NULL;
    public:
        std::string friendly_name;
        DefaultAudioDeviceWin();
        ~DefaultAudioDeviceWin();
};
