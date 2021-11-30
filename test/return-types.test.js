const test = require("ava");

const mediaDevicesUtil = require("..");

const isWin = process.platform === "win32";
const isString = (val) => typeof val === "string";

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

test("getScreenAuthorizationStatus - should return string as auth status", async (t) => {
    t.assert(isString(mediaDevicesUtil.getScreenAuthorizationStatus()));
});

test("getMediaAuthorizationStatus - should return string as auth status (when media category is provided)", (t) => {
    const cameraPermission = mediaDevicesUtil.getMediaAuthorizationStatus("camera");
    t.assert(isString(cameraPermission));
    const microphonePermission = mediaDevicesUtil.getMediaAuthorizationStatus("microphone");
    t.assert(isString(microphonePermission));
});

test("getMediaAuthorizationStatus - should return object of strings as auth statuses (when no media category is provided)", (t) => {
    const permissions = mediaDevicesUtil.getMediaAuthorizationStatus();
    t.assert(typeof permissions === "object");
    t.assert(isString(permissions.camera));
    t.assert(isString(permissions.microphone));
});

test("requestScreenAuthorization - should return string as auth status", (t) => {
    t.assert(isString(mediaDevicesUtil.requestScreenAuthorization()));
});

test("requestMediaAuthorization - should return Promise with string as auth status", async (t) => {
    const cameraPermissionRequestPromise = mediaDevicesUtil.requestMediaAuthorization("camera");
    t.assert(cameraPermissionRequestPromise instanceof Promise);
    t.assert(isString(await cameraPermissionRequestPromise));
    const microphonePermissionRequestPromise = mediaDevicesUtil.requestMediaAuthorization("microphone");
    t.assert(microphonePermissionRequestPromise instanceof Promise);
    t.assert(isString(await microphonePermissionRequestPromise));
});
