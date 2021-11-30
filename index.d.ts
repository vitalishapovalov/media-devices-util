declare type TDevice = {
  id: string;
  label: string;
  alternativeName?: string;
}

declare type TAuthorization = "AUTHORIZED" | "DENIED" | "NOT_DETERMINED";

declare type TMediaType = "camera" | "microphone";

declare type TMediaDevicesUtil = {
  getDefaultVideoDevice(): TDevice;
  getDefaultAudioDevice(): TDevice;
  getVideoDevices(): TDevice[];
  getAudioDevices(): TDevice[];
  getScreenAuthorizationStatus(): TAuthorization;
  getMediaAuthorizationStatus(mediaType: TMediaType): TAuthorization;
  getMediaAuthorizationStatus(): {
    camera: TAuthorization;
    microphone: TAuthorization;
  };
  requestScreenAuthorization(): TAuthorization;
  requestMediaAuthorization(mediaType: TMediaType): Promise<TAuthorization>;
}

declare const mediaDevicesUtil: TMediaDevicesUtil;

export = mediaDevicesUtil;
