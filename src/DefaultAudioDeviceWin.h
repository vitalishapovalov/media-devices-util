#pragma once

#include <string>
#include <Mmdeviceapi.h>

class DefaultAudioDeviceWin {
    private:
        bool is_initialized;
        IMMDevice* mm_device;
        IMMDeviceEnumerator* mm_device_enumerator;
        IPropertyStore* props_store;
    public:
        std::string friendly_name;
        DefaultAudioDeviceWin();
        ~DefaultAudioDeviceWin();
};
