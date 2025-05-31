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