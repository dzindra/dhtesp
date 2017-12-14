# dhtesp
Simple sketch for DHT22 readouts by ESP8266

## Usage

Connect to `dht-sensor` Wifi and navigate to http://192.168.4.1 or http://192.168.4.1/temp.json

## Compiling & uploading

* Download and install [Arduino IDE](https://www.arduino.cc/en/Main/Software) (not the online version)
* Install [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
* Download and install [CH340 USB driver](https://github.com/dzindra/ch34x-osx-installer/releases)
* Download this repo and open it in Arduino IDE
* Connect the board to the computer
* Select `Tools > Boards > Wemos D1 R2 & Mini`
* Select `Tools > Upload speed > 230400`
* Select `Tools > Port > /dev/cu.wchusb....`
* Hit `Upload` in Arduino IDE
