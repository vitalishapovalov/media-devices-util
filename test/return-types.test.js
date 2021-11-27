const test = require("ava");

const mediaDevicesUtil = require("..");

const isCI = process.env.CI === "true";
const isWin = process.platform === "win32";

const verifyDeviceObject = (t, device) => {
    // device itself should always be an object
    t.is(typeof device, "object");

    // CI will have no devices at all, so we're expecting empty results
    const deviceFieldType = isCI ? "undefined" : "string";
    t.is(typeof device.id, deviceFieldType);
    t.is(typeof device.label, deviceFieldType);
    // windows doesn't have an ID, only alternative name
    if (isWin) {
        t.is(typeof device.alternativeName, deviceFieldType);
    }
};

if (isWin) {
    test("getDefaultAudioDevice - should return 'Device' object", (t) => {
        verifyDeviceObject(t, mediaDevicesUtil.getDefaultAudioDevice());
    });
    
    test("getDefaultVideoDevice - should return 'Device' object", (t) => {
        verifyDeviceObject(t, mediaDevicesUtil.getDefaultVideoDevice());
    });
    
    test("getVideoDevices - should return an Array of 'Device' objects", (t) => {
        const videoDevices = mediaDevicesUtil.getVideoDevices();
        t.assert(Array.isArray(mediaDevicesUtil.getVideoDevices()));
        for (const videoDevice of videoDevices) {
            verifyDeviceObject(t, videoDevice);
        }
    });
    
    test("getAudioDevices - should return an Array of 'Device' objects", (t) => {
        const audioDevices = mediaDevicesUtil.getAudioDevices();
        t.assert(Array.isArray(mediaDevicesUtil.getAudioDevices()));
        for (const audioDevice of audioDevices) {
            verifyDeviceObject(t, audioDevice);
        }
    });
} else {
    console.log(mediaDevicesUtil.getVideoDevices());
    console.log(mediaDevicesUtil.getAudioDevices());
    console.log(mediaDevicesUtil.getDefaultVideoDevice());
}
