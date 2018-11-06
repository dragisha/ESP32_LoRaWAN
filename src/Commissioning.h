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
#define OVER_THE_AIR_ACTIVATION                     1

/*!
 * Indicates if the end-device is to be connected to a private or public network
 */
#define LORAWAN_PUBLIC_NETWORK                      true

/*!
 * IEEE Organizationally Unique Identifier ( OUI ) (big endian)
 * \remark This is unique to a company or organization
 */
#define IEEE_OUI                                    0x22, 0x32, 0x33

/*!
 * Mote device IEEE EUI (big endian)
 *
 * \remark In this application the value is automatically generated by calling
 *         BoardGetUniqueId function
 */
#define LORAWAN_DEVICE_EUI                          { IEEE_OUI, 0x5b, 0x17, 0x8e, 0x0d, 0x84 }

/*!
 * Application IEEE EUI (big endian)
 */
#define LORAWAN_APPLICATION_EUI                     { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x9E, 0x63 }

/*!
 * AES encryption/decryption cipher application key
 */
#define LORAWAN_APPLICATION_KEY                     { 0x31, 0x35, 0x18, 0x44, 0xF1, 0x68, 0x4B, 0xFF, 0x0F, 0xFB, 0xE0, 0xAE, 0xE1, 0xED, 0x5D, 0xBA }

/*!
 * Current network ID
 */
#define LORAWAN_NETWORK_ID                          ( uint32_t )0x00666888

/*!
 * Device address on the network (big endian)
 *
 * \remark In this application the value is automatically generated using
 *         a pseudo random generator seeded with a value derived from
 *         BoardUniqueId value if LORAWAN_DEVICE_ADDRESS is set to 0
 */
#define LORAWAN_DEVICE_ADDRESS                      ( uint32_t )0x66666666

/*!
 * AES encryption/decryption cipher network session key
 */
#define LORAWAN_NWKSKEY                             { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88 }

/*!
 * AES encryption/decryption cipher application session key
 */
#define LORAWAN_APPSKEY                             { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88 }

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
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            2

/*!
* User application data buffer size
*/
#define LORAWAN_APP_DATA_MAX_SIZE                           64
/*!
 * User application data buffer size
 */
#if defined( USE_BAND_433 ) || defined( USE_BAND_470 ) || defined( USE_BAND_470PREQUEL ) || defined( USE_BAND_780 ) || defined( USE_BAND_868 )

#define LORAWAN_APP_DATA_SIZE                       16

#elif defined( USE_BAND_915 ) || defined( USE_BAND_915_HYBRID )

#define LORAWAN_APP_DATA_SIZE                       11

#endif



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


extern uint8_t DevEui[];
extern uint8_t AppEui[];
extern uint8_t AppKey[];

#if( OVER_THE_AIR_ACTIVATION == 0 )
extern uint8_t NwkSKey[] = LORAWAN_NWKSKEY;
extern uint8_t AppSKey[] = LORAWAN_APPSKEY;

/*!
* Device address
*/
extern uint32_t DevAddr = LORAWAN_DEVICE_ADDRESS;

#endif

/*!
* Application port
*/
extern uint8_t AppPort;

/*!
* User application data size
*/
extern uint8_t AppDataSize;

/*!
* User application data
*/
extern uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];//Ҫ���͵�����

/*!
* Indicates if the node is sending confirmed or unconfirmed messages
*/
extern uint8_t IsTxConfirmed;

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
extern LoRaMacCallback_t LoRaMacCallbacks;
extern MibRequestConfirm_t mibReq;
extern enum eDeviceState DeviceState;
extern struct ComplianceTest_s ComplianceTest;
void MlmeConfirm( MlmeConfirm_t *mlmeConfirm );
void McpsIndication( McpsIndication_t *mcpsIndication );
void McpsConfirm( McpsConfirm_t *mcpsConfirm );
void OnTxNextPacketTimerEvent( void );
bool SendFrame( void );
void PrepareTxFrame( uint8_t port );


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __LORA_COMMISSIONING_H__
