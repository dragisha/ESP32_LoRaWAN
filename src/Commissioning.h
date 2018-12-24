/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2015 Semtech

Description: End device commissioning parameters

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/

#ifndef __LORA_COMMISSIONING_H__
#define __LORA_COMMISSIONING_H__
#include "Arduino.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "LoRaMac-definitions.h"
#include "board.h"
#include "LoRaMac.h"

extern void lora_printf(const char *format, ...);
extern uint8_t isJioned;
extern uint8_t isAckReceived;
/*!
 * When set to 1 the application uses the Over-the-Air activation procedure
 * When set to 0 the application uses the Personalization activation procedure
 */

/*!
 * Indicates if the end-device is to be connected to a private or public network
 */
#define LORAWAN_PUBLIC_NETWORK                      true

/*!
 * Current network ID
 */
#define LORAWAN_NETWORK_ID                          ( uint32_t )0x00666888

/*!
 * Defines the application data transmission duty cycle. 60s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            60000

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

/*!
 * Default datarate
 */
#define LORAWAN_DEFAULT_DATARATE                    DR_0

/*!
 * LoRaWAN confirmed messages
 * dd: Used in ComplianceTest workflows, so I leave it here but ignore in normal workflows.
 */
#define LORAWAN_CONFIRMED_MSG_ON                    true

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_ON                              1

#if defined( USE_BAND_868 )

#include "LoRaMacTest.h"

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        true

#define USE_SEMTECH_DEFAULT_CHANNEL_LINEUP          1

#if ( USE_SEMTECH_DEFAULT_CHANNEL_LINEUP == 1 )

#define LC4                { 867100000, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }
#define LC5                { 867300000, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }
#define LC6                { 867500000, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }
#define LC7                { 867700000, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }
#define LC8                { 867900000, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 }
#define LC9                { 868800000, { ( ( DR_7 << 4 ) | DR_7 ) }, 2 }
#define LC10               { 868300000, { ( ( DR_6 << 4 ) | DR_6 ) }, 1 }

#endif

#endif

/*!
* User application data buffer size
*/
#define LORAWAN_APP_DATA_MAX_SIZE                           64
/*!
 * User application data buffer size
 */



enum eDeviceState
{
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP
};

struct ComplianceTest_s
{
    bool Running;
    uint8_t State;
    bool IsTxConfirmed;
    uint8_t AppPort;
    uint8_t AppDataSize;
    uint8_t *AppDataBuffer;
    uint16_t DownLinkCounter;
    bool LinkCheck;
    uint8_t DemodMargin;
    uint8_t NbGateways;
};


/*
 * Needs to be global and exposed because of compliance code and is used from OTAA code in LoRa.cpp as well as from compliance part.
 */
extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];

/*!
* Defines the application data transmission duty cycle
*/
extern uint32_t TxDutyCycleTime;

/*!
* Timer to handle the application data transmission duty cycle
*/
extern TimerEvent_t TxNextPacketTimer;

/*!
* Specifies the state of the application LED
*/
extern bool AppLedStateOn;

/*!
* Indicates if a new packet can be sent
*/
extern bool NextTx;

extern LoRaMacPrimitives_t LoRaMacPrimitives;
extern LoRaMacPrimitives_t applicationLevel;
extern LoRaMacCallback_t LoRaMacCallbacks;

extern MibRequestConfirm_t mibReq;
extern enum eDeviceState DeviceState;
extern struct ComplianceTest_s ComplianceTest;
void MlmeConfirm( MlmeConfirm_t *mlmeConfirm );
void McpsIndication( McpsIndication_t *mcpsIndication );
void McpsConfirm( McpsConfirm_t *mcpsConfirm );
void OnTxNextPacketTimerEvent( void );
bool SendFrame( void );
bool SendAppFrame( uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed);
void PrepareTxFrame( uint8_t port );


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __LORA_COMMISSIONING_H__
