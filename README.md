# MPPTCharger
Maximum Power Point Tracking Charge Controller for ESP8266 and Raspberry Pi

## Setup build system
```bash
sudo apt install python3-pip
sudo pip3 install platformio
```

## Building for ESP8266
```bash
cd source
pio run -t upload
```

## Building for Raspberry Pi
Only supported to build natively on Raspberry Pi
```bash
cd rpi
pio run
.pio/build/raspberrypi_3b/program

```

### Enable I2C driver
```
$ vi /boot/config.txt
dtparam=i2c_arm=on
```

