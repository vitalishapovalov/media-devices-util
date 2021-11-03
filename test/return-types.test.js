const test = require("tap").test;

const mediaDevicesUtil = require("..");

const verifyDeviceObject = (t, device) => {
    t.type(device, "object");
    t.type(device.id, "string");
    t.type(device.label, "string");
};

test("getDefaultAudioDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice());
    t.end();
});

test("getDefaultVideoDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice());
    t.end();
});

test("getVideoDevices - should return an Array", (t) => {
    t.assert(Array.isArray(mediaDevicesUtil.getVideoDevices()));
    t.end();
});

test("getAudioDevices - should return an Array", (t) => {
    t.assert(Array.isArray(mediaDevicesUtil.getAudioDevices()));
    t.end();
});

test("getVideoDevices - returned array should consist of 'Device' objects", (t) => {
    const videoDevices = mediaDevicesUtil.getVideoDevices();
    for (const videoDevice of videoDevices) {
        verifyDeviceObject(t, videoDevice);
    }
    t.end();
});

test("getAudioDevices - returned array should consist of 'Device' objects", (t) => {
    const audioDevices = mediaDevicesUtil.getAudioDevices();
    for (const audioDevice of audioDevices) {
        verifyDeviceObject(t, audioDevice);
    }
    t.end();
});
