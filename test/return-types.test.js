const test = require("tap").test;

const mediaDevicesUtil = require("..");

const isWin = process.platform === "win32";

const verifyDeviceObject = (t, device, shouldHaveId, shouldHaveAlternativeName) => {
    t.type(device, "object");
    t.type(device.label, "string");
    if (shouldHaveId) {
        t.type(device.id, "string");
    }
    if (shouldHaveAlternativeName) {
        t.type(device.alternativeName, "string");
    }
};

test("getDefaultAudioDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice(), true);
    t.end();
});

if (!isWin) {
    test("getDefaultVideoDevice - should return 'Device' object", (t) => {
        verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice(), true);
        t.end();
    });
}

test("getVideoDevices - should return an Array of 'Device' objects", (t) => {
    const videoDevices = mediaDevicesUtil.getVideoDevices();
    t.assert(Array.isArray(mediaDevicesUtil.getVideoDevices()));
    for (const videoDevice of videoDevices) {
        verifyDeviceObject(t, videoDevice, !isWin, isWin);
    }
    t.end();
});

test("getAudioDevices - should return an Array of 'Device' objects", (t) => {
    const audioDevices = mediaDevicesUtil.getAudioDevices();
    t.assert(Array.isArray(mediaDevicesUtil.getAudioDevices()));
    for (const audioDevice of audioDevices) {
        verifyDeviceObject(t, audioDevice, !isWin, isWin);
    }
    t.end();
});
