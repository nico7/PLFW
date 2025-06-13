# FN530

This is the PL530 driver.

## NOTE!
- Uploading LittleFS is a race condition, therefore must be done as soon as the ESP32 comes out of reset

## TODO
- [ ] Make website nice
- [ ] Tune TEC algo
- [ ] Tune LASER algo

## Arduino Settings
- Flash Size: 8MB
- Partition Scheme: 8M with spiffs (3MB APP/1.5MB SPIFFS)

Not sure which of the following since I have them all installed ¯\_(ツ)_/¯
- Probably using ASYNC TCP v3.4.0 by ESP32Async from https://github.com/ESP32Async/AsyncTCP
- Probably using AsyncEspFsWebserver v2.0.2 by Tolentino Cotesta... from  https://github.com/cotestatnt/async-esp-fs-webserver
- Probably using ESP Async WebServer v3.7.7 by ESP32Async from https://github.com/ESP32Async/ESPAsyncWebServer
- Probably using WiFiManager v2.0.17 by tzapu from https://github.com/tzapu/WiFiManager
