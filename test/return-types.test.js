const test = require("tap").test;

const mediaDevicesUtil = require("..");

const isWin = process.platform === "win32";
const isMac = process.platform === "darwin";

const verifyDeviceObject = (t, device) => {
    t.type(device, "object");
    t.type(device.label, "string");
    if (isWin) {
        t.type(device.alternativeName, "string");
    } else {
        t.type(device.id, "string");
    }
};

test("getDefaultAudioDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice());
    t.end();
});

if (isMac) {
    test("getDefaultVideoDevice - should return 'Device' object", (t) => {
        verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice());
        t.end();
    });
}

test("getVideoDevices - should return an Array of 'Device' objects", (t) => {
    const videoDevices = mediaDevicesUtil.getVideoDevices();
    t.assert(Array.isArray(mediaDevicesUtil.getVideoDevices()));
    for (const videoDevice of videoDevices) {
        verifyDeviceObject(t, videoDevice);
    }
    t.end();
});

test("getAudioDevices - should return an Array of 'Device' objects", (t) => {
    const audioDevices = mediaDevicesUtil.getAudioDevices();
    t.assert(Array.isArray(mediaDevicesUtil.getAudioDevices()));
    for (const audioDevice of audioDevices) {
        verifyDeviceObject(t, audioDevice);
    }
    t.end();
});
