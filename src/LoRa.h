#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include "SPI.h"
#include "board.h"
#include "Commissioning.h"
#include "Mcu.h"
#include "Commissioning.h"

class LoRaClass{
public:
  void DeviceStateInit();

  void DeviceStateJoinOTAA();

  void DeviceStateJoinABP(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey);

  bool DeviceGetLoRaCreds(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey);

  void DeviceStateSend(uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed);

  void DeviceSleep(uint8_t isLowPowerOn,uint8_t debuglevel);
};


extern LoRaClass LoRa;

#endif
