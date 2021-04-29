# MPPTCharger
Maximum Power Point Tracking Charge Controller for ESP8266 and Raspberry Pi

## Setup build system
```bash
sudo apt install python3-pip
sudo pip3 install platformio
```

## Building for ESP8266
```bash
pio run -t upload
```

## Building for Raspberry Pi
Only supported to build natively on Raspberry Pi
```bash
pio run -e raspberrypi_3b
.pio/build/raspberrypi_3b/program

```

