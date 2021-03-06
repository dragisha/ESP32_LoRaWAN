#include "LoRaMac.h"
#include "Mcu.h"

#include "LoRa.h"

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


#endif


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
#define LORAWAN_DEVICE_EUI                          { IEEE_OUI, 0x00, 0x00, 0x88, 0x88, 0x02 }

/*!
 * Application IEEE EUI (big endian)
 */
#define LORAWAN_APPLICATION_EUI                     { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x9E, 0x63 }

/*!
 * AES encryption/decryption cipher application key
 */
#define LORAWAN_APPLICATION_KEY                     { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x66, 0x01 }

/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            2


#if defined( USE_BAND_433 ) || defined( USE_BAND_470 ) || defined( USE_BAND_470PREQUEL ) || defined( USE_BAND_780 ) || defined( USE_BAND_868 )

#define LORAWAN_APP_DATA_SIZE                       16

#elif defined( USE_BAND_915 ) || defined( USE_BAND_915_HYBRID )

#define LORAWAN_APP_DATA_SIZE                       11

#endif

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


RTC_DATA_ATTR enum eDeviceState DeviceState;
LoRaMacPrimitives_t LoRaMacPrimitives;
LoRaMacPrimitives_t applicationLevel;
LoRaMacCallback_t LoRaMacCallbacks;
MibRequestConfirm_t mibReq;
uint8_t isJioned=0;
uint8_t isAckReceived=0;
/*!
 * Defines the application data transmission duty cycle. 60s, value in [ms].
 */
uint8_t DevEui[] = LORAWAN_DEVICE_EUI;
uint8_t AppEui[] = LORAWAN_APPLICATION_EUI;
uint8_t AppKey[] = LORAWAN_APPLICATION_KEY;

uint8_t AppPort = LORAWAN_APP_PORT;
uint8_t AppDataSize = LORAWAN_APP_DATA_SIZE;
uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
uint8_t IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
uint32_t TxDutyCycleTime;
TimerEvent_t TxNextPacketTimer;
bool AppLedStateOn = false;
bool NextTx = true;


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


/*!
 * LoRaWAN compliance tests support data
 */

struct ComplianceTest_s ComplianceTest;


/*!
 * \brief   Prepares the payload of the frame
 */
void PrepareTxFrame( uint8_t port )
{
    switch( port )
    {
        case 2:
        {
#if defined( USE_BAND_433 ) || defined( USE_BAND_470 ) || defined( USE_BAND_470PREQUEL ) || defined( USE_BAND_780 ) || defined( USE_BAND_868 )

////             	AppData[0] =   TEM[0];
////              AppData[1] =   TEM[1];
////              AppData[2] =   TEM[2];
////              AppData[3] =   TEM[3];
////              AppData[4]  =  TEM[4];
////              AppData[5]  =  Temperature_H;
////              AppData[6]  =  Temperature_L;
////              AppData[7]  =   ' ';
////              AppData[8]  =  HUM[0];
////              AppData[9]  =  HUM[1];
////              AppData[10] =  HUM[2];
////              AppData[11] =  HUM[3];
////              AppData[12] =  Humidity_H;
////              AppData[13] =  Humidity_L;
////              AppData[14] =  '%';
////			      	AppData[15] =  '4';



#elif defined( USE_BAND_915 ) || defined( USE_BAND_915_HYBRID )
            AppData[0] =   '1';
            AppData[1] =   '2';
            AppData[2] =   '3';
            AppData[3] =   '4';
            AppData[4] =   '5';
            AppData[5] =   '6';
            AppData[6] =   '7';
            AppData[7] =   '8';
            AppData[8] =   '9';
            AppData[9] =   '0';
            AppData[10] =  'a';
#endif
        }
            break;
        case 224:
            if( ComplianceTest.LinkCheck == true )
            {
                ComplianceTest.LinkCheck = false;
                AppDataSize = 3;
                AppData[0] = 5;
                AppData[1] = ComplianceTest.DemodMargin;
                AppData[2] = ComplianceTest.NbGateways;
                ComplianceTest.State = 1;
            }
            else
            {
                switch( ComplianceTest.State )
                {
                    case 4:
                        ComplianceTest.State = 1;
                        break;
                    case 1:
                        AppDataSize = 2;
                        AppData[0] = ComplianceTest.DownLinkCounter >> 8;
                        AppData[1] = ComplianceTest.DownLinkCounter;
                        break;
                }
            }
            break;
        default:
            break;
    }
}

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
bool SendFrame( void )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    if( LoRaMacQueryTxPossible( AppDataSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
        if( IsTxConfirmed == false )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = AppPort;
            mcpsReq.Req.Unconfirmed.fBuffer = AppData;
            mcpsReq.Req.Unconfirmed.fBufferSize = AppDataSize;
            mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = AppPort;
            mcpsReq.Req.Confirmed.fBuffer = AppData;
            mcpsReq.Req.Confirmed.fBufferSize = AppDataSize;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
        }
    }

    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}

/*!
 *
 * @param frameData to be copied to AppData, data to be sent
 * @param frameSize its size
 * @param appPort fPort parameter
 * @param isTxConfirmed do we require an ACK?
 * @return true on success
 */

/*
 * Trebam provjeriti da li uopšte trebam datarate argument ili su defaulti ili neka globalna var rješenje...
 */
bool SendAppFrame( uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed, int8_t datarate) {
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    if (LoRaMacQueryTxPossible( frameSize, &txInfo ) != LORAMAC_STATUS_OK) {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
    } else {
        // dd: ovo je bilo premaleno, drzao sam 64 a gurao po 232 mozda vise
        memcpy(AppData, frameData, LORAWAN_APP_DATA_MAX_SIZE);
        if (datarate < 0)
            datarate = LORAMAC_DEFAULT_DATARATE;
        if (isTxConfirmed) {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.Datarate = datarate;
            mcpsReq.Req.Confirmed.NbTrials = 8;
        } else {
            mcpsReq.Req.Unconfirmed.Datarate = datarate;
            mcpsReq.Type = MCPS_UNCONFIRMED;
        }
        lora_printf("[Wish %d]", datarate);
        mcpsReq.Req.Unconfirmed.fPort = appPort;
        mcpsReq.Req.Unconfirmed.fBuffer = AppData;
        mcpsReq.Req.Unconfirmed.fBufferSize = frameSize;
    }

    return LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK;
}

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
void OnTxNextPacketTimerEvent( void )
{
#if (DebugLevel >= 1)
    lora_printf("{OnTxNextPacketTimerEvent}");
#endif

    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    TimerStop( &TxNextPacketTimer );

    mibReq.Type = MIB_NETWORK_JOINED;
    status = LoRaMacMibGetRequestConfirm( &mibReq );

    if( status == LORAMAC_STATUS_OK )
    {
        if( mibReq.Param.IsNetworkJoined == true )
        {
            DeviceState = DEVICE_STATE_SEND;
            NextTx = true;
        }
        else
        {
            DeviceState = DEVICE_STATE_JOIN;
        }
    }
}

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
void McpsConfirm( McpsConfirm_t *mcpsConfirm ) {
    if (applicationLevel.MacMcpsConfirm) {
        applicationLevel.MacMcpsConfirm(mcpsConfirm);
    }
    if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK ) {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                break;
            }
            case MCPS_PROPRIETARY:
            {
                break;
            }
            default:
                break;
        }
    }

    NextTx = true;
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
void McpsIndication( McpsIndication_t *mcpsIndication )
{
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            break;
        }
        case MCPS_CONFIRMED:
        {
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        case MCPS_MULTICAST:
        {
            //lora_printf("Got a multicast to McpsIndication\n");
            break;
        }
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot

    if( mcpsIndication->AckReceived == true ) {
//    	lora_printf("UpLinkCounter:%d  DownLinkCounter:%d \n",UpLinkCounter,DownLinkCounter);
        //lora_printf("DRV: +SEND:DONE %llx\n\n", mcpsIndication);
        isAckReceived++;
        //delay(100);
    }

    if( ComplianceTest.Running == true )
    {
        ComplianceTest.DownLinkCounter++;
    }

    if (applicationLevel.MacMcpsIndication) {
        applicationLevel.MacMcpsIndication(mcpsIndication);
    }

    if( mcpsIndication->RxData == true ) {
        switch( mcpsIndication->Port )
        {
            case 1: // The application LED can be controlled on port 1 or 2
            case 2:
                if( mcpsIndication->BufferSize == 1 )
                {
                    AppLedStateOn = mcpsIndication->Buffer[0] & 0x01;
                    //  AppLedStateOn = AppLedStateOn;
                }
                break;
            case 224:
                if( ComplianceTest.Running == false )
                {
                    // Check compliance test enable command (i)
                    if( ( mcpsIndication->BufferSize == 4 ) &&
                        ( mcpsIndication->Buffer[0] == 0x01 ) &&
                        ( mcpsIndication->Buffer[1] == 0x01 ) &&
                        ( mcpsIndication->Buffer[2] == 0x01 ) &&
                        ( mcpsIndication->Buffer[3] == 0x01 ) )
                    {
                        IsTxConfirmed = false;
                        AppPort = 224;
                        AppDataSize = 2;
                        ComplianceTest.DownLinkCounter = 0;
                        ComplianceTest.LinkCheck = false;
                        ComplianceTest.DemodMargin = 0;
                        ComplianceTest.NbGateways = 0;
                        ComplianceTest.Running = true;
                        ComplianceTest.State = 1;

                        MibRequestConfirm_t mibReq;
                        mibReq.Type = MIB_ADR;
                        mibReq.Param.AdrEnable = true;
                        LoRaMacMibSetRequestConfirm( &mibReq );

#if defined( USE_BAND_868 )
                        LoRaMacTestSetDutyCycleOn( false );
#endif

                    }
                }
                else
                {
                    ComplianceTest.State = mcpsIndication->Buffer[0];
                    switch( ComplianceTest.State )
                    {
                        case 0: // Check compliance test disable command (ii)
                            IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                            AppPort = LORAWAN_APP_PORT;
                            AppDataSize = LORAWAN_APP_DATA_SIZE;
                            ComplianceTest.DownLinkCounter = 0;
                            ComplianceTest.Running = false;

                            MibRequestConfirm_t mibReq;
                            mibReq.Type = MIB_ADR;
                            mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                            LoRaMacMibSetRequestConfirm( &mibReq );
#if defined( USE_BAND_868 )
                            LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif
                            break;
                        case 1: // (iii, iv)
                            AppDataSize = 2;
                            break;
                        case 2: // Enable confirmed messages (v)
                            IsTxConfirmed = true;
                            ComplianceTest.State = 1;
                            break;
                        case 3:  // Disable confirmed messages (vi)
                            IsTxConfirmed = false;
                            ComplianceTest.State = 1;
                            break;
                        case 4: // (vii)
                            AppDataSize = mcpsIndication->BufferSize;

                            AppData[0] = 4;
                            for( uint8_t i = 1; i < AppDataSize; i++ )
                            {
                                AppData[i] = mcpsIndication->Buffer[i] + 1;
                            }
                            break;
                        case 5: // (viii)
                        {
                            MlmeReq_t mlmeReq;
                            mlmeReq.Type = MLME_LINK_CHECK;
                            LoRaMacMlmeRequest( &mlmeReq );
                        }
                            break;
                        case 6: // (ix)
                        {
                            MlmeReq_t mlmeReq;

                            // Disable TestMode and revert back to normal operation
                            IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                            AppPort = LORAWAN_APP_PORT;
                            AppDataSize = LORAWAN_APP_DATA_SIZE;
                            ComplianceTest.DownLinkCounter = 0;
                            ComplianceTest.Running = false;

                            MibRequestConfirm_t mibReq;
                            mibReq.Type = MIB_ADR;
                            mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                            LoRaMacMibSetRequestConfirm( &mibReq );
#if defined( USE_BAND_868 )
                            LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
#endif

                            mlmeReq.Type = MLME_JOIN;

                            mlmeReq.Req.Join.DevEui = DevEui;
                            mlmeReq.Req.Join.AppEui = AppEui;
                            mlmeReq.Req.Join.AppKey = AppKey;
                            mlmeReq.Req.Join.NbTrials = 3;

                            LoRaMacMlmeRequest( &mlmeReq );
                            DeviceState = DEVICE_STATE_SLEEP;
                        }
                            break;
                        case 7: // (x)
                        {
                            if( mcpsIndication->BufferSize == 3 )
                            {
                                MlmeReq_t mlmeReq;
                                mlmeReq.Type = MLME_TXCW;
                                mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                                LoRaMacMlmeRequest( &mlmeReq );
                            }
                            else if( mcpsIndication->BufferSize == 7 )
                            {
                                MlmeReq_t mlmeReq;
                                mlmeReq.Type = MLME_TXCW_1;
                                mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                                mlmeReq.Req.TxCw.Frequency = ( uint32_t )( ( mcpsIndication->Buffer[3] << 16 ) | ( mcpsIndication->Buffer[4] << 8 ) | mcpsIndication->Buffer[5] ) * 100;
                                mlmeReq.Req.TxCw.Power = mcpsIndication->Buffer[6];
                                LoRaMacMlmeRequest( &mlmeReq );
                            }
                            ComplianceTest.State = 1;
                        }
                            break;
                        default:
                            break;
                    }
                }
                break;
            default:
                break;
        }
    }

//	uint64_t lightsleep=nextAlarm-TimerGetTimerValue();
//	lora_printf("sleep:%d\n",(uint32_t)lightsleep);
//	esp_sleep_enable_timer_wakeup(191600/1000*lightsleep);
//	esp_light_sleep_start();
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */

void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                if (applicationLevel.MacMlmeConfirm) applicationLevel.MacMlmeConfirm(mlmeConfirm);
                isJioned++;
                // Status is OK, node has joined the network
                DeviceState = DEVICE_STATE_SEND;
            }
            else
            {
                // Join was not successful. Try to join again
                DeviceState = DEVICE_STATE_JOIN;
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                if (applicationLevel.MacMlmeConfirm) applicationLevel.MacMlmeConfirm(mlmeConfirm);
                // Check DemodMargin
                // Check NbGateways
                if( ComplianceTest.Running == true )
                {
                    ComplianceTest.LinkCheck = true;
                    ComplianceTest.DemodMargin = mlmeConfirm->DemodMargin;
                    ComplianceTest.NbGateways = mlmeConfirm->NbGateways;
                }
            }
            break;
        }
        default:
            break;
    }
    NextTx = true;
}

void LoRaClass::DeviceStateInit(uint32_t myRandSeed)
{
    LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;//
    LoRaMacPrimitives.MacMcpsIndication = McpsIndication;//
    LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;//
    LoRaMacCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks );

    TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );//
    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
    LoRaMacMibSetRequestConfirm( &mibReq );//global variable AdrCtrlon=1

    mibReq.Type = MIB_PUBLIC_NETWORK;
    mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
    LoRaMacMibSetRequestConfirm( &mibReq );//PublicNetwork=true; //SX1276SetModem(MODEM_LORA);//1276.settings.lora.publicnetwork=true;//  public SYNCWORD;

#if defined( USE_BAND_868 )
    LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
    if(IsLoRaMacNetworkJoined==false)
    {
#if( USE_SEMTECH_DEFAULT_CHANNEL_LINEUP == 1 )
    LoRaMacChannelAdd( 3, ( ChannelParams_t )LC4 );
    LoRaMacChannelAdd( 4, ( ChannelParams_t )LC5 );
    LoRaMacChannelAdd( 5, ( ChannelParams_t )LC6 );
    LoRaMacChannelAdd( 6, ( ChannelParams_t )LC7 );
    LoRaMacChannelAdd( 7, ( ChannelParams_t )LC8 );
    LoRaMacChannelAdd( 8, ( ChannelParams_t )LC9 );
    LoRaMacChannelAdd( 9, ( ChannelParams_t )LC10 );

    mibReq.Type = MIB_RX2_DEFAULT_CHANNEL;
    mibReq.Param.Rx2DefaultChannel = ( Rx2ChannelParams_t ){ 869525000, DR_3 };
    LoRaMacMibSetRequestConfirm( &mibReq );

    mibReq.Type = MIB_RX2_CHANNEL;
    mibReq.Param.Rx2Channel = ( Rx2ChannelParams_t ){ 869525000, DR_3 };
    LoRaMacMibSetRequestConfirm( &mibReq );
#endif
    }
#endif

}

void LoRaClass::DeviceStateJoinOTAA() {
    MlmeReq_t mlmeReq;

    mlmeReq.Type = MLME_JOIN;

    mlmeReq.Req.Join.DevEui = DevEui;
    mlmeReq.Req.Join.AppEui = AppEui;
    mlmeReq.Req.Join.AppKey = AppKey;
    mlmeReq.Req.Join.NbTrials = 3;

    if (NextTx == true) {
        lora_printf("+JOIN OTAA:STARTING...\n");
        LoRaMacMlmeRequest(&mlmeReq);
    }
    DeviceState = DEVICE_STATE_SLEEP;
}

void LoRaClass::DeviceStateJoinABP(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey, uint32_t downlinkCounter) {
    mibReq.Type = MIB_NET_ID;
    mibReq.Param.NetID = LORAWAN_NETWORK_ID;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_DEV_ADDR;
    mibReq.Param.DevAddr = devAddr;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_NWK_SKEY;
    mibReq.Param.NwkSKey = nwkSKey;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_APP_SKEY;
    mibReq.Param.AppSKey = appSKey;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_DOWNLINK_COUNTER;
    mibReq.Param.DownLinkCounter = downlinkCounter;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_NETWORK_JOINED;
    mibReq.Param.IsNetworkJoined = true;
    LoRaMacMibSetRequestConfirm(&mibReq);

    mibReq.Type = MIB_ADR;
    mibReq.Param.AdrEnable = true;
    LoRaMacMibSetRequestConfirm(&mibReq);

    MlmeReq_t mlmeReq;

    mlmeReq.Type = MLME_LINK_CHECK;

    if (NextTx == true) {
        lora_printf("+JOIN ABP:STARTING...\n");
        LoRaMacMlmeRequest(&mlmeReq);
    }

    DeviceState = DEVICE_STATE_SLEEP;
}

bool LoRaClass::DeviceGetLoRaCreds(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey) {
    mibReq.Type = MIB_NETWORK_JOINED;
    LoRaMacMibGetRequestConfirm(&mibReq);
    if (!mibReq.Param.IsNetworkJoined)
        return false;

    mibReq.Type = MIB_DEV_ADDR;
    LoRaMacMibGetRequestConfirm(&mibReq);
    devAddr = mibReq.Param.DevAddr;

    mibReq.Type = MIB_NWK_SKEY;
    LoRaMacMibGetRequestConfirm(&mibReq);
    memcpy(nwkSKey, mibReq.Param.NwkSKey, 16);

    mibReq.Type = MIB_APP_SKEY;
    LoRaMacMibGetRequestConfirm(&mibReq);
    memcpy(appSKey, mibReq.Param.AppSKey, 16);

    return true;
}

/*!
 *
 * @param frameData to be copied to AppData, data to be sent
 * @param frameSize its size
 * @param appPort fPort parameter
 * @param isTxConfirmed do we require an ACK?
 * @return true on success
 */

void LoRaClass::DeviceStateSend( uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed, int8_t datarate) {
	if( NextTx)
	{
#if DebugLevel > 0
		lora_printf("In sending...\r\n");
#endif
		DelayMs(100);
		NextTx = SendAppFrame(frameData, frameSize, appPort, isTxConfirmed, datarate);
	} else {
	    // TODO What if not? We must maintain a queue here or in LoRaMac. FIXME
	}
	if( ComplianceTest.Running == true )
	{
			// Schedule next packet transmission
			TxDutyCycleTime = 5000; // 5000 ms
	}
	else
	{
			// Schedule next packet transmission
			TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
	}
}
void LoRaClass::DeviceSleep(uint8_t isLowPowerOn,uint8_t debuglevel)
{
	Mcu.sleep(isLowPowerOn,debuglevel);
}
LoRaClass LoRa;

