# media-devices-util

<div style="display: flex;">
  <a href="https://badge.fury.io/js/media-devices-util">
    <img src="https://img.shields.io/npm/v/media-devices-util?color=%2351c82c">
  </a>
  <img src="https://img.shields.io/badge/os-Windows%20%7C%20MacOS-success">
  <a href="https://github.com/vitalishapovalov/media-devices-util/blob/master/LICENSE">
    <img src="https://img.shields.io/npm/l/media-devices-util?color=%2351c82c">
  </a>
  <a href="https://github.com/vitalishapovalov/media-devices-util">
    <img src="https://github.com/vitalishapovalov/media-devices-util/actions/workflows/nodejs.yml/badge.svg">
  </a>
</div>
<br />

What is this package for? To get info about available media devices, so it can be passed then to other programs (e.g. use with `ffmpeg`, to provide input).

## How to use it

### Install

```shell
npm i -S media-devices-util
```

### Use

```javascript
const mediaDevicesUtil = require("media-devices-util");

const videoDevices = mediaDevicesUtil.getVideoDevices();
const audioDevices = mediaDevicesUtil.getAudioDevices();

console.log({ audioDevices, videoDevices });

// output on mac:
// {
//     audioDevices: [
//         {
//             id: 'AppleHDAEngineInput:1F,3,0,1,0:1',
//             label: 'Built-in Microphone'
//         }
//     ],
//     videoDevices: [
//         {
//             id: '0x1420000005ac8600',
//             label: 'FaceTime HD Camera (Built-in)'
//         },
//         {
//             id: '69733382',
//             label: 'Capture screen 0'
//         }
//     ]
// }
//
// output on windows:
// {
//     audioDevices: [
//         {
//             id: '0',
//             label: 'Microphone (High Definition Audio Device)',
//             alternativeName: '@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\\wave_{B073E9D3-C0A7-4CB0-84E0-F829B281F95F}'
//         }
//     ],
//     videoDevices: [
//         {
//             id: '\\\\?\\usb#vid_203a&pid_fff9&mi_00#6&fafa70b&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global',
//             label: 'FaceTime HD Camera (Built-in)'',
//             alternativeName: '@device_pnp_\\\\?\\usb#vid_203a&pid_fff9&mi_00#6&fafa70b&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global'
//         }
//     ]
// }
```

### API

`media-devices-util` object has the following API:

```typescript
type TDevice = {
  // For mac:
  // audio/video devices are using the 'uniqueID' property - https://developer.apple.com/documentation/avfoundation/avcapturedevice/1390477-uniqueid
  // screen devices are using the 'CGDirectDisplayID' property - https://developer.apple.com/documentation/coregraphics/cgdirectdisplayid
  // For win:
  // audio devices are using the 'WaveInID' property - https://docs.microsoft.com/en-us/windows/win32/directshow/selecting-a-capture-device
  // video devices are using the 'DevicePath' property - https://docs.microsoft.com/en-us/windows/win32/directshow/selecting-a-capture-device
  id: string;

  // For mac:
  // audio/video devices are using the 'localizedName' property - https://developer.apple.com/documentation/avfoundation/avcapturedevice/1388222-localizedname
  // screen devices are using the 'Capture screen ' prefix plus device id in 'CGGetActiveDisplayList' - https://developer.apple.com/documentation/coregraphics/1454603-cggetactivedisplaylist
  // For win:
  // using the 'FriendlyName' property - https://docs.microsoft.com/en-us/windows/win32/directshow/selecting-a-capture-device
  label: string;

  // For win:
  // using the 'GetDisplayName' method's return value - https://docs.microsoft.com/en-us/windows/win32/api/objidl/nf-objidl-imoniker-getdisplayname
  alternativeName?: string;
}

type TMediaDevicesUtil = {
  getDefaultVideoDevice(): TDevice;
  getDefaultAudioDevice(): TDevice;
  getVideoDevices(): TDevice[];
  getAudioDevices(): TDevice[];
}
```

#### getDefaultAudioDevice

Return type: `TDevice`

Retrieve the default audio input device (microphone).

For `win`, using the [`GetDefaultAudioEndpoint`](https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nf-mmdeviceapi-immdeviceenumerator-getdefaultaudioendpoint) method.

For `mac`, using the [`defaultDeviceWithMediaType:AVMediaTypeAudio`](https://developer.apple.com/documentation/avfoundation/avcapturedevice/1386589-defaultdevicewithmediatype) method.

#### getDefaultVideoDevice

Return type: `TDevice | null`

Retrieve the default video input device (camera).

For `win`, taking the first entry of the [`IEnumMoniker`](https://docs.microsoft.com/en-us/windows/win32/api/objidl/nn-objidl-ienummoniker) with the `CLSID_VideoInputDeviceCategory` parameter.

For `mac`, using the [`defaultDeviceWithMediaType:AVMediaTypeVideo`](https://developer.apple.com/documentation/avfoundation/avcapturedevice/1386589-defaultdevicewithmediatype) method for audio/video devices.

#### getVideoDevices

Return type: `TDevice[]`

Retrieve the default video input devices list (cameras, screens, etc.).

For `mac`, using the [`devicesWithMediaType:AVMediaTypeVideo`](https://developer.apple.com/documentation/avfoundation/avcapturedevice/1390520-deviceswithmediatype) method, and the [`CGGetActiveDisplayList`](https://developer.apple.com/documentation/coregraphics/1454603-cggetactivedisplaylist) method for screens.

For `win`, using the [`IEnumMoniker`](https://docs.microsoft.com/en-us/windows/win32/api/objidl/nn-objidl-ienummoniker) with the `CLSID_VideoInputDeviceCategory` parameter.

#### getAudioDevices

Return type: `TDevice[]`

Retrieve the default audio input devices list (microphones, virtual devices, etc.).

For `mac`, using the [`devicesWithMediaType:AVMediaTypeAudio`](https://developer.apple.com/documentation/avfoundation/avcapturedevice/1390520-deviceswithmediatype) method.

For `win`, using the [`IEnumMoniker`](https://docs.microsoft.com/en-us/windows/win32/api/objidl/nn-objidl-ienummoniker) with the `CLSID_AudioInputDeviceCategory` parameter.

## FFMPEG

When using an `ffmpeg` or other libs to capture audio/video devices, you first need to obtain a list of available devices and their labels (IDs).

And when running `ffmpeg` to just get the devices list - it takes too long, because a lot of overhead `ffmpeg` lib actions are taken.

Using `media-devices-util` is much, much faster.

### Usage example

Let's imagine, that we want to capture the default audio and default video device with `ffmpeg`:

```javascript
const spawn = require("child_process").spawn;
const mediaDevicesUtil = require("media-devices-util");

const defaultVideoDevice = mediaDevicesUtil.getDefaultVideoDevice();
const defaultAudioDevice = mediaDevicesUtil.getDefaultAudioDevice();

// for MacOS, you can use the 'label' prop, and the item's index (if used with getVideoDevices/getAudioDevices):
const ffmpegWinProcess = spawn("ffmpeg", ["-f", "avfoundation", "-i", `${defaultVideoDevice.label}:${defaultAudioDevice.label}`, "video.mkv"]);

// for Windows, you should use the 'label' or the 'alternativeName' prop:
const ffmpegMacProcess = spawn("ffmpeg", ["-f", "dshow", "-i", `video="${defaultVideoDevice.label}":audio="${defaultAudioDevice.alternativeName}"`, "video.mkv"]);
```

### Alternatives

You can use the `ffmpeg`'s `list_devices` option (for [mac](https://ffmpeg.org/ffmpeg-devices.html#:~:text=-list_devices%20%3CTRUE%7CFALSE,names%20and%20indices.) or [win](https://ffmpeg.org/ffmpeg-devices.html#:~:text=the%20captured%20audio.-,list_devices,set%20to%20true%2C%20print%20a%20list%20of%20devices%20and%20exit.,-list_options)), and [parse](https://github.com/syumai/ffmpeg-device-list-parser) it's output to get the list itself.

Also, you can use other `ffmpeg` command abstractions (like [ffmpeg](https://www.npmjs.com/package/ffmpeg) and [fluent-ffmpeg](https://www.npmjs.com/package/fluent-ffmpeg)).

In any case, the `list_devices` in `ffmpeg` lib is something like a crunch, and the lib itself doesn't provide a [programmatic way to enumerate devices](https://trac.ffmpeg.org/wiki/DirectShow#Howtoprogrammaticallyenumeratedevices).

Or use other [ffmpeg nodejs bindings lib](https://github.com/Streampunk/beamcoder).

## Next Steps

- Fix the double-backslash Napi issue
- Add device permissions checkers
- Add listeners for devices/permissions
