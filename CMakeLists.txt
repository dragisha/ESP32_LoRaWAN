cmake_minimum_required(VERSION 3.10)
project(LoRaWAN)

set(CMAKE_CXX_STANDARD 14)

set(ESP32_LIBS_HOME "/home/dragisha/arduino-1.8.5/hardware/heltec/esp32")

include_directories(
  ${ESP32_LIBS_HOME}/cores/esp32
  ${ESP32_LIBS_HOME}/tools/sdk/include/freertos
  ${ESP32_LIBS_HOME}/tools/sdk/include/newlib
  ${ESP32_LIBS_HOME}/tools/sdk/include/soc
  ${ESP32_LIBS_HOME}/tools/sdk/include/heap
  ${ESP32_LIBS_HOME}/tools/sdk/include/driver
  ${ESP32_LIBS_HOME}/tools/sdk/include/log
  ${ESP32_LIBS_HOME}/tools/sdk/include/config/
  ${ESP32_LIBS_HOME}/tools/sdk/include/esp32
  ${ESP32_LIBS_HOME}/tools/sdk/include/spi_flash/
  ${ESP32_LIBS_HOME}/libraries/SPI/src
  ${ESP32_LIBS_HOME}/libraries/ArduinoOTA/src
  ${ESP32_LIBS_HOME}/libraries/Update/src
  ${ESP32_LIBS_HOME}/libraries/WiFi/src
  ${ESP32_LIBS_HOME}/libraries/ESPmDNS/src
  ${ESP32_LIBS_HOME}/libraries/EEPROM
  ${ESP32_LIBS_HOME}/variants/wireless_stick_lite
  ${ESP32_LIBS_HOME}/variants/wifi_lora_32_V2
)

add_executable(LoRaWAN
        src/LoRa.cpp
        src/LoRa.h
        src/aes.c
        src/aes.h
        src/board.c
        src/board.h
        src/cmac.c
        src/cmac.h
        src/Commissioning.h
        src/delay.c
        src/delay.h
        src/fifo.c
        src/fifo.h
        src/LoRaMac-definitions.h
        src/LoRaMac.c
        src/LoRaMac.h
        src/LoRaMacCrypto.c
        src/LoRaMacCrypto.h
        src/LoRaMacTest.h
        src/Mcu.h
        src/pinName-board.h
        src/pinName-ioe.h
        src/radio.h
        src/rtc-board.h
        src/RTE_Components.h
        src/sx1276-board.c
        src/sx1276-board.h
        src/sx1276.c
        src/sx1276.h
        src/sx1276Regs-Fsk.h
        src/sx1276Regs-LoRa.h
        src/timer.h
        src/utilities.c
        src/utilities.h
)