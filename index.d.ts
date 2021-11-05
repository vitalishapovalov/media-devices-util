declare type TDevice = {
  label: string;
  id?: string;
  alternativeName?: string;
}

declare type TMediaDevicesUtil = {
  getDefaultVideoDevice(): TDevice;
  getDefaultAudioDevice(): TDevice;
  getVideoDevices(): TDevice[];
  getAudioDevices(): TDevice[];
}

declare const mediaDevicesUtil: TMediaDevicesUtil;

export = mediaDevicesUtil;
