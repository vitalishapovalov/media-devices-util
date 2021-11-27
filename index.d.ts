declare type TDevice = {
  id: string;
  label: string;
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
