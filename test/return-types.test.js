const test = require("ava");

const mediaDevicesUtil = require("..");

const isWin = process.platform === "win32";

const verifyDeviceObject = (t, device) => {
    // device itself should always be an object
    t.is(typeof device, "object");

    // if we have zero keys - means that something went wrong and we expect no info at all
    const deviceFieldType = Object.keys(device).length ? "string" : "undefined";

    t.is(typeof device.id, deviceFieldType);
    t.is(typeof device.label, deviceFieldType);
    if (isWin) {
        t.is(typeof device.alternativeName, deviceFieldType);
    }
};

test("getDefaultAudioDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice());
});

test("getDefaultVideoDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice());
});

test("getVideoDevices - should return an Array of 'Device' objects", (t) => {
    const videoDevices = mediaDevicesUtil.getVideoDevices();
    t.assert(Array.isArray(videoDevices));
    for (const videoDevice of videoDevices) {
        verifyDeviceObject(t, videoDevice);
    }
});

test("getAudioDevices - should return an Array of 'Device' objects", (t) => {
    const audioDevices = mediaDevicesUtil.getAudioDevices();
    t.assert(Array.isArray(audioDevices));
    for (const audioDevice of audioDevices) {
        verifyDeviceObject(t, audioDevice);
    }
});
