#include <vector>
#include <string>
#include <napi.h>
#import <AVFoundation/AVFoundation.h>

#include "Device.h"
#include "ConverterUtil.h"

class MediaDevicesUtilMac : public Napi::Addon<MediaDevicesUtilMac> {
    public:
        MediaDevicesUtilMac(Napi::Env env, Napi::Object exports) {
            DefineAddon(exports, {
                InstanceMethod("getVideoDevices", &MediaDevicesUtilMac::get_video_devices),
                InstanceMethod("getAudioDevices", &MediaDevicesUtilMac::get_audio_devices)
            });
        }

    protected:
        Napi::Value get_video_devices(const Napi::CallbackInfo& info) {
            uint32_t num_screens = 0;
            std::vector<Device> available_devices;
            NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
            NSArray *devices_muxed = [AVCaptureDevice devicesWithMediaType:AVMediaTypeMuxed];

            // cameras, etc.
            int index = 0;
            for (AVCaptureDevice *device in devices) {
                std::string label = [[device localizedName] UTF8String];
                index = [devices indexOfObject:device];
                available_devices.push_back(Device(index, label));
            }
            for (AVCaptureDevice *device in devices_muxed) {
                std::string label = [[device localizedName] UTF8String];
                index = [devices count] + [devices_muxed indexOfObject:device];
                available_devices.push_back(Device(index, label));
            }

            // screens
            CGGetActiveDisplayList(0, NULL, &num_screens);
            if (num_screens > 0) {
                const int total_video_devices_count = [devices count] + [devices_muxed count];
                CGDirectDisplayID screens[num_screens];
                CGGetActiveDisplayList(num_screens, screens, &num_screens);
                for (int i = 0; i < (int) num_screens; i++) {
                    available_devices.push_back(Device(
                        total_video_devices_count + i,
                        "Capture screen " + std::to_string(i)
                    ));
                }
            }

            return ConverterUtil::devices_vector_to_napi_arr(available_devices, info.Env());
        }

        Napi::Value get_audio_devices(const Napi::CallbackInfo& info) {
            std::vector<Device> available_devices;
            NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeAudio];

            for (AVCaptureDevice *device in devices) {
                std::string label = [[device localizedName] UTF8String];
                int index = [devices indexOfObject:device];
                available_devices.push_back(Device(index, label));
            }

            return ConverterUtil::devices_vector_to_napi_arr(available_devices, info.Env());
        }
};

NODE_API_ADDON(MediaDevicesUtilMac);
