# media-devices-util

What is this package for?

When using an `ffmpeg` or other libs to capture audio/video devices, you first need to obtain a list of available devices and their labels (IDs).

And when running `ffmpeg` to just get the devices list - it takes too long, because a lot of overhead actions are taken. Using this lib is much faster.

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
//         { id: '0', label: 'ZoomAudioDevice' },
//         { id: '1', label: 'MacBook Pro Microphone' }
//     ],
//     videoDevices: [
//         { id: '0', label: 'FaceTime HD Camera (Built-in)' },
//         { id: '1', label: 'Capture screen 0' }
//     ]
// }
//
// output on windows:
// {
//     audioDevices: [
//         {
//             id: 'Microphone (High Definition Audio Device)',
//             label: '@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\\wave_{5CD1095F-7066-4597-B86B-D55F177403A3}'
//         }
//     ],
//     videoDevices: [
//         {
//             id: 'FaceTime HD Camera (Built-in)',
//             label: '@device_pnp_\\\\?\\usb#vid_203a&pid_fff9&mi_00#6&28ef9718&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global'
//         }
//     ]
// }
```

## Alternatives

You can use the `ffmpeg`'s `list_devices` option (for [mac](https://ffmpeg.org/ffmpeg-devices.html#:~:text=-list_devices%20%3CTRUE%7CFALSE,names%20and%20indices.) or [win](https://ffmpeg.org/ffmpeg-devices.html#:~:text=the%20captured%20audio.-,list_devices,set%20to%20true%2C%20print%20a%20list%20of%20devices%20and%20exit.,-list_options)), and [parse](https://github.com/syumai/ffmpeg-device-list-parser) it's output to get the list itself.

Also, you can use other `ffmpeg` command abstractions (like [ffmpeg](https://www.npmjs.com/package/ffmpeg) and [fluent-ffmpeg](https://www.npmjs.com/package/fluent-ffmpeg)).

In any case, the `list_devices` in `ffmpeg` lib is something like a crunch, and the lib itself doesn't provide a [programmatic way to enumerate devices](https://trac.ffmpeg.org/wiki/DirectShow#Howtoprogrammaticallyenumeratedevices).

Or use other [ffmpeg nodejs bindings lib](https://github.com/Streampunk/beamcoder).
