#include <vector>
#include <string>
#include <napi.h>
#import <AVFoundation/AVFoundation.h>

#include "Device.h"
#include "ConverterUtil.h"

class MediaDevicesUtilMac : public Napi::Addon<MediaDevicesUtilMac> {
    public:
        MediaDevicesUtilMac(const Napi::Env& env, const Napi::Object& exports) {
            DefineAddon(exports, {
                InstanceMethod("getVideoDevices", &MediaDevicesUtilMac::get_video_devices),
                InstanceMethod("getAudioDevices", &MediaDevicesUtilMac::get_audio_devices),
                InstanceMethod("getDefaultVideoDevice", &MediaDevicesUtilMac::get_default_video_device),
                InstanceMethod("getDefaultAudioDevice", &MediaDevicesUtilMac::get_default_audio_device)
            });
        }

    protected:
        Napi::Value get_default_video_device(const Napi::CallbackInfo& info) {
            const AVCaptureDevice* device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
            if (!device) {
                return Napi::Object::New(info.Env());
            }
            return ConverterUtil::device_to_napi_object(map_avdevice_to_device(device), info.Env());
        }

        Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
            const AVCaptureDevice* device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
            if (!device) {
                return Napi::Object::New(info.Env());
            }
            return ConverterUtil::device_to_napi_object(map_avdevice_to_device(device), info.Env());
        }

        Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
            uint32_t screens_count = 0;
            CGGetActiveDisplayList(0, NULL, &screens_count);
            NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
            NSArray *devices_muxed = [AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed];
            std::vector<Device> available_devices;

            available_devices.reserve([devices count] + [devices_muxed count] + screens_count);

            // cameras, etc.
            for (const AVCaptureDevice* device in devices) {
                available_devices.push_back(map_avdevice_to_device(device));
            }
            for (const AVCaptureDevice* device in devices_muxed) {
                available_devices.push_back(map_avdevice_to_device(device));
            }

            // screens
            if (screens_count > 0) {
                CGDirectDisplayID screens[screens_count];
                CGGetActiveDisplayList(screens_count, screens, &screens_count);
                for (int i = 0; i < (int) screens_count; i++) {
                    Device screen_device(std::to_string(screens[i]), "Capture screen " + std::to_string(i));
                    available_devices.push_back(screen_device);
                }
            }

            return ConverterUtil::devices_vector_to_napi_arr(available_devices, info.Env());
        }

        Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
            NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeAudio];
            std::vector<Device> available_devices;

            available_devices.reserve([devices count]);

            for (const AVCaptureDevice* device in devices) {
                available_devices.push_back(map_avdevice_to_device(device));
            }

            return ConverterUtil::devices_vector_to_napi_arr(available_devices, info.Env());
        }

    private:
        Device map_avdevice_to_device(const AVCaptureDevice* avdevice) {
            std::string uniquie_id([[avdevice uniqueID] UTF8String]);
            std::string localized_name([[avdevice localizedName] UTF8String]);
            return Device(uniquie_id, localized_name);
        }

};

NODE_API_ADDON(MediaDevicesUtilMac);
