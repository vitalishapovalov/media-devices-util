const test = require("tap").test;

const mediaDevicesUtil = require("..");

const isCI = process.env.CI === "true";
const isWin = process.platform === "win32";
const isMac = process.platform === "darwin";

const verifyDeviceObject = (t, device) => {
    // device itself should always be an object
    t.type(device, "object");

    // CI will have no devices at all, so we're expecting empty results
    const deviceFieldType = isCI ? "undefined" : "string";
    t.type(device.label, deviceFieldType);
    // windows doesn't have an ID, only alternative name
    if (isWin) {
        t.type(device.alternativeName, deviceFieldType);
    } else {
        t.type(device.id, deviceFieldType);
    }
};

if (isMac) {
    test("getDefaultVideoDevice - should return 'Device' object", (t) => {
        verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice());
        t.end();
    });
}

if (isWin) {
    test("getDefaultVideoDevice - should return null", (t) => {
        t.type(mediaDevicesUtil.getDefaultVideoDevice(), "null");
        t.end();
    });
}

test("getDefaultAudioDevice - should return 'Device' object", (t) => {
    verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice());
    t.end();
});

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
