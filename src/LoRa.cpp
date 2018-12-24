#include "LoRa.h"

void LoRaClass::DeviceStateInit()
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

void LoRaClass::DeviceStateJoinABP(uint32_t &devAddr, uint8_t *nwkSKey, uint8_t *appSKey) {
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

void LoRaClass::DeviceStateSend( uint8_t *frameData, uint8_t frameSize, uint8_t appPort, bool isTxConfirmed) {
	if( NextTx)
	{
#if DebugLevel > 0
		lora_printf("In sending...\r\n");
#endif
		DelayMs(100);
		NextTx = SendAppFrame(frameData, frameSize, appPort, isTxConfirmed);
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

