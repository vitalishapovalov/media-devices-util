const test = require("tap").test;

const mediaDevicesUtil = require("..");

test("should return an Array", (t) => {
    t.assert(Array.isArray(mediaDevicesUtil.getVideoDevices()));
    t.assert(Array.isArray(mediaDevicesUtil.getAudioDevices()));
    t.end();
});

test("returned array should consist of 'Device' objects", (t) => {
    const videoDevices = mediaDevicesUtil.getVideoDevices();
    const audioDevices = mediaDevicesUtil.getAudioDevices();
    const traverseAssertDevices = (devices) => {
        for (const device of devices) {
            t.type(device, "object");
            t.type(device.id, "string");
            t.type(device.label, "string");
        }
    };
    traverseAssertDevices(videoDevices);
    traverseAssertDevices(audioDevices);
    t.end();
});
