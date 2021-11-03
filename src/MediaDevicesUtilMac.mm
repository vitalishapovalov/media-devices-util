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
                InstanceMethod("getAudioDevices", &MediaDevicesUtilMac::get_audio_devices),
                InstanceMethod("getDefaultVideoDevice", &MediaDevicesUtilMac::get_default_video_device),
                InstanceMethod("getDefaultAudioDevice", &MediaDevicesUtilMac::get_default_audio_device)
            });
        }

    protected:
        Napi::Value get_default_video_device(const Napi::CallbackInfo& info) {
            Device defaultVideoDevice = get_default_device(AVMediaTypeVideo);
            return ConverterUtil::device_to_napi_object(defaultVideoDevice, info.Env());
        }

        Napi::Value get_default_audio_device(const Napi::CallbackInfo& info) {
            Device defaultAudioDevice = get_default_device(AVMediaTypeAudio);
            return ConverterUtil::device_to_napi_object(defaultAudioDevice, info.Env());
        }

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

    private:
        Device get_default_device(const AVMediaType media_type) {
            NSArray *devices = [AVCaptureDevice devicesWithMediaType:media_type];
            AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:media_type];
            std::string label = [[device localizedName] UTF8String];
            int index = [devices indexOfObject:device];
            return Device(index, label);
        }

};

NODE_API_ADDON(MediaDevicesUtilMac);
