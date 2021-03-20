#ifndef LORA_H
#define LORA_H
#ifdef __cplusplus
extern "C"{
#endif

#include "LoRaMac-definitions.h"
#include "LoRaMac.h"
#include "board.h"

#include "SPI.h"

/*!
* User application data buffer size
*/
#define LORAWAN_APP_DATA_MAX_SIZE                           256

/*!
 * Indicates if the end-device is to be connected to a private or public network
 */
#define LORAWAN_PUBLIC_NETWORK                      true

/*!
 * Current network ID
 */
#define LORAWAN_NETWORK_ID                          ( uint32_t )0


/*
 * Needs to be global and exposed because of compliance code and is used from OTAA code in LoRa.cpp as well as from compliance part.
 */
extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];

extern uint8_t isJoined;
extern uint8_t isAckReceived;

/*!
* Defines the application data transmission duty cycle
*/
extern uint32_t TxDutyCycleTime;

/*!
* Timer to handle the application data transmission duty cycle
*/
extern TimerEvent_t TxNextPacketTimer;

enum eDeviceState
{
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP
};


extern LoRaMacPrimitives_t applicationLevel;

/*!
* Specifies the state of the application LED
*/
extern bool AppLedStateOn;

extern enum eDeviceState DeviceState;
void OnTxNextPacketTimerEvent( void );

#ifdef __cplusplus
} // extern "C"
#endif

class LoRaClass {
public:
  void DeviceStateInit(uint32_t myRandSeed, DeviceClass_t deviceClass);

  void DeviceStateJoinOTAA();

  void DeviceStateJoinABP(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey, uint32_t downlinkCounter);

  bool DeviceGetLoRaCreds(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey);

  void DeviceStateSend(uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed, int8_t datarate = -1);

  void DeviceSleep(uint8_t isLowPowerOn,uint8_t debuglevel);
};


extern LoRaClass LoRa;

#endif
