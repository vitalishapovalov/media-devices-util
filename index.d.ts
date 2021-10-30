declare type TDevice = {
  id: string;
  label: string;
}

declare type TMediaDevicesUtil = {
  getVideoDevices(): TDevice[];
  getAudioDevices(): TDevice[];
}

declare const mediaDevicesUtil: TMediaDevicesUtil;

export = mediaDevicesUtil;
