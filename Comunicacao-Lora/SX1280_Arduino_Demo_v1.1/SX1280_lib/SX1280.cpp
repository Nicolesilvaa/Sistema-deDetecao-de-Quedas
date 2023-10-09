/********************************************************************************************
*	SX1280.cpp - Library for SX1280
*	Copyright(C) 2018 NiceRF.All right reserved.  
*	@version 1.1  
*	This library is suit for LoRa1280 in loRa mode
*	please make sure the supply of you board is UNDER 3.3V!! Otherwise, the module will be destory!!
*	The configuration of both the modules should be the same.
*********************************************************************************************/
#include <SPI.h>
#include <SoftwareSerial.h>
#include <SX1280.h>
//#include "string.h"

#define SPI_NSS_LOW()  digitalWrite(SPI_NSS, LOW)
#define SPI_NSS_HIGH() digitalWrite(SPI_NSS, HIGH)

/*
The TCXO is 32MHz, so the frequency step should be FREQ_STEP = 52e6 / (2^18) Hz
*/
#define FREQ_STEP    198.364

static loRa_Para_t *lora_para_pt;	//pointer to hold lora parameter from app layer

SX1280::SX1280(uint8_t NSS_Pin, uint8_t NRESET_Pin,uint8_t BUSY_Pin,uint8_t DIO1_Pin)
{
	SPI_NSS = NSS_Pin;
	RF_NRESET = NRESET_Pin;
	RF_BUSY = BUSY_Pin;
	RF_DIO1 = DIO1_Pin;
	
	lora_para_pt->rf_freq;
	lora_para_pt->tx_power;
	lora_para_pt->lora_sf;
	lora_para_pt->band_width;
	lora_para_pt->code_rate;
	lora_para_pt->payload_size;
}

void SX1280::SPI_Init(void)
{
	SPI.begin();
	
	// init slave select pin
	pinMode(SPI_NSS, OUTPUT);
	digitalWrite(SPI_NSS, HIGH);

	// depends on LORA spi timing
	SPI.setBitOrder(MSBFIRST);
	
	// too fast may cause error
	SPI.setClockDivider(SPI_CLOCK_DIV16);
	SPI.setDataMode(SPI_MODE0);
}

void SX1280::Pin_Init(void)
{
	pinMode(RF_NRESET, OUTPUT);
	digitalWrite(RF_NRESET, LOW);
	
	pinMode(RF_BUSY, INPUT);
	pinMode(RF_DIO1, INPUT);
}

bool SX1280::Init(loRa_Para_t *lp_pt)
{
	lora_para_pt = lp_pt;
	
	Pin_Init();
	SPI_Init();
	
	Reset_SX1280();	// reset LoRa chip
	SX1280_Config();// Set RF parameter,like frequency,data rate etc

	return true;
}

void SX1280::Reset_SX1280(void)
{
	delay(20);
	digitalWrite(RF_NRESET, LOW);	
	delay(50);		//more thena 100us, delay 2ms
	digitalWrite(RF_NRESET, HIGH);	
	delay(20);		
}

uint8_t SX1280::spi_rw(uint8_t value_w) 
{
	uint8_t value_r;
	
	value_r = SPI.transfer(value_w);	
	
	return (value_r);	
}

/***************sx1262*****************/
void SX1280::CheckBusy(void)
{
	uint8_t busy_timeout_cnt;								
		
	busy_timeout_cnt = 0;
	while(digitalRead(RF_BUSY))
	{
		delay(1);
		busy_timeout_cnt++;
		
		if(busy_timeout_cnt>2)	//TODO		
		{
			//TODO 
            //add a flag here, and add some process to reset the module in the main().
			break;		
		}
	}
}

void SX1280::WriteCommand(uint8_t Opcode, uint8_t *buffer, uint16_t size )
{
	uint8_t i;
	
	CheckBusy();
	
	SPI_NSS_LOW();
	spi_rw(Opcode);
	for(i=0;i<size;i++)
	{
		spi_rw(buffer[i]);
	}
	SPI_NSS_HIGH();
	
	if( Opcode != RADIO_SET_SLEEP )
    {
        CheckBusy();
    }
}
void SX1280::ReadCommand( uint8_t Opcode, uint8_t *buffer, uint16_t size )
{
    uint8_t i;
	
	CheckBusy();
    SPI_NSS_LOW();
	
	spi_rw(Opcode);
	spi_rw(0xFF);
	
	for( i = 0; i < size; i++ )
    {
		*(buffer+i) = spi_rw(0xFF);
    }
	
    SPI_NSS_HIGH();
    CheckBusy();
}

void SX1280::WriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    uint8_t addr_l,addr_h;
	uint8_t i;
	
	addr_l = address&0xff;
	addr_h = address>>8;

    CheckBusy();
    SPI_NSS_LOW();
	
	spi_rw(RADIO_WRITE_REGISTER);
	spi_rw(addr_h);//MSB
	spi_rw(addr_l);//LSB
	for(i=0;i<size;i++)
	{
		spi_rw(buffer[i]);
	}
	
    SPI_NSS_HIGH();
    CheckBusy();
}

void SX1280::WriteRegister( uint16_t address, uint8_t value )
{
    WriteRegisters( address, &value, 1 );
}

void SX1280::ReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    uint16_t i;
	uint8_t addr_l,addr_h;
	
	addr_h = address >> 8;
	addr_l = address & 0x00FF;
	
    CheckBusy();
    SPI_NSS_LOW();
	spi_rw(RADIO_READ_REGISTER);
	spi_rw(addr_h);//MSB
	spi_rw(addr_l);//LSB
    spi_rw(0xFF);
	for( i = 0; i < size; i++ )
    {
		*(buffer+i) = spi_rw(0xFF);
    }
	
    SPI_NSS_HIGH();
    CheckBusy();
}

uint8_t SX1280::ReadRegister( uint16_t address )
{
    uint8_t data;

    ReadRegisters( address, &data, 1 );

    return data;
}

void SX1280::WriteBuffer(uint8_t offset, uint8_t *data, uint8_t length)
{
    uint16_t i;
	
	CheckBusy();
    SPI_NSS_LOW();

	spi_rw(RADIO_WRITE_BUFFER);
	spi_rw(offset);
	
	for(i=0;i<length;i++)
	{
		spi_rw(data[i]);
	}
	
    SPI_NSS_HIGH();
    CheckBusy();
}
void SX1280::ReadBuffer(uint8_t offset, uint8_t *data, uint8_t length)
{
    uint16_t i;
	
	CheckBusy();
    SPI_NSS_LOW();

	spi_rw(RADIO_READ_BUFFER);
	spi_rw(offset);
	spi_rw(0xFF);
	for(i=0;i<length;i++)
	{
		data[i]=spi_rw(0xFF);
	}
	
    SPI_NSS_HIGH();
    CheckBusy();
}

void SX1280::SetSleep(void)
{
	uint8_t Opcode,sleepConfig;
	
	Opcode = 0x84;
	sleepConfig = 0x07;	//bit2: 1: ; bit0: 0: 
	
	CheckBusy();
	SPI_NSS_LOW();
	spi_rw(Opcode);
	spi_rw(sleepConfig);
	SPI_NSS_HIGH();
}

//0:STDBY_RC; 1:STDBY_XOSC
void SX1280::SetStandby(uint8_t StdbyConfig)
{
	uint8_t Opcode;
	
	Opcode = 0x80;	//
	
	CheckBusy();
	SPI_NSS_LOW();
	spi_rw(Opcode);
	spi_rw(StdbyConfig);
	SPI_NSS_HIGH();
}

/************************************************************************************
periodBase  Time-out step
  0x00        15.625 us
  0x01        62.5 us
  0x02        1 ms
  0x03        4 ms
  
periodBaseCount
   0x0000         No time-out, Tx Single mode
   Others 		  Time-out active,Time-out duration = periodBase * periodBaseCount
***********************************************************************************/
void SX1280::SetTx(uint8_t periodBase, uint16_t periodBaseCount)
{
	uint8_t buf[3];
	
    buf[0] = periodBase;
    buf[1] = ( uint8_t )( ( periodBaseCount >> 8 ) & 0x00FF );
    buf[2] = ( uint8_t )( periodBaseCount & 0x00FF );
	
	WriteCommand( RADIO_SET_TX, buf, 3 );
}

/************************************************************************************
periodBase  Time-out step
  0x00        15.625 us
  0x01        62.5 us
  0x02        1 ms
  0x03        4 ms
  
periodBaseCount
   0x0000         No time-out. Rx Single mode
   0xFFFF		  Rx Continuous mode
   Others 		  Time-out active,Time-out duration = periodBase * periodBaseCount
***********************************************************************************/
void SX1280::SetRx(uint8_t periodBase, uint16_t periodBaseCount)
{
	uint8_t buf[3];
	
	buf[0] = periodBase;
    buf[1] = ( uint8_t )( ( periodBaseCount >> 8 ) & 0x00FF );
    buf[2] = ( uint8_t )( periodBaseCount & 0x00FF );
	
	WriteCommand( RADIO_SET_RX, buf, 3 );
}

void SX1280::SetRxDutyCycle(uint8_t PeriodBase, uint16_t NbStepRx, uint16_t RxNbStepSleep )
{
	uint8_t buf[5];

    buf[0] = PeriodBase;
    buf[1] = ( uint8_t )( ( NbStepRx >> 8 ) & 0x00FF );
    buf[2] = ( uint8_t )( NbStepRx & 0x00FF );
    buf[3] = ( uint8_t )( ( RxNbStepSleep >> 8 ) & 0x00FF );
    buf[4] = ( uint8_t )( RxNbStepSleep & 0x00FF );
	
	WriteCommand( RADIO_SET_RXDUTYCYCLE, buf, 5 );
}

void SX1280::SetLongPreamble( uint8_t enable )
{
    WriteCommand( RADIO_SET_LONGPREAMBLE, &enable, 1 );
}

void SX1280::SetCad( void )
{
    WriteCommand( RADIO_SET_CAD, 0, 0 );
}
void SX1280::SetTxContinuousWave( void )
{
    WriteCommand( RADIO_SET_TXCONTINUOUSWAVE, 0, 0 );
}
void SX1280::SetTxContinuousPreamble( void )
{
    WriteCommand( RADIO_SET_TXCONTINUOUSPREAMBLE, 0, 0 );
}
/******************************************************
	packetType			Value			Modem mode of operation
 PACKET_TYPE_GFSK 		0x00[default] 		GFSK mode
 PACKET_TYPE_LORA 		0x01 				LoRa? mode
 PACKET_TYPE_RANGING 	0x02 				Ranging Engine mode
 PACKET_TYPE_FLRC 		0x03 				FLRC mode
 PACKET_TYPE_BLE 		0x04 				BLE mode
******************************************************/

void SX1280::SetPacketType( uint8_t packetType )
{
    WriteCommand( RADIO_SET_PACKETTYPE,&packetType, 1 );
}

uint8_t SX1280::GetPacketType(void)
{
	uint8_t Opcode;
	uint8_t Status;
	uint8_t packetType;
	
	CheckBusy();
	Opcode = RADIO_GET_PACKETTYPE;	
	
	SPI_NSS_LOW();
	spi_rw(Opcode);
	Status = spi_rw(0xFF);
	packetType = spi_rw(0xFF);
	SPI_NSS_HIGH();
	
	return packetType;
}

void SX1280::SetRfFrequency( uint32_t frequency )
{
    uint8_t buf[3];
    uint32_t freq = 0;

    freq = ( uint32_t )( ( double )frequency / ( double )FREQ_STEP );
    buf[0] = ( uint8_t )( ( freq >> 16 ) & 0xFF );
    buf[1] = ( uint8_t )( ( freq >> 8 ) & 0xFF );
    buf[2] = ( uint8_t )( freq & 0xFF );
    WriteCommand( RADIO_SET_RFFREQUENCY, buf, 3 );
}

void SX1280::SetTxParams( int8_t power, uint8_t rampTime )
{
    uint8_t buf[2];

    // The power value to send on SPI/UART is in the range [0..31] and the
    // physical output power is in the range [-18..13]dBm
    buf[0] = power + 18;
    buf[1] = ( uint8_t )rampTime;
    WriteCommand( RADIO_SET_TXPARAMS, buf, 2 );
}

void SX1280::SetCadParams( uint8_t cadSymbolNum )
{
    WriteCommand( RADIO_SET_CADPARAMS,&cadSymbolNum, 1 );
}

void SX1280::SetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress )
{
    uint8_t buf[2];

    buf[0] = txBaseAddress;
    buf[1] = rxBaseAddress;
    WriteCommand( RADIO_SET_BUFFERBASEADDRESS, buf, 2 );
}

void SX1280::SetModulationParams(uint8_t sf, uint8_t bw, uint8_t cr)
{
    uint8_t buf[3];

    //below parameter used in PACKET_TYPE_LORA or PACKET_TYPE_RANGING		
    buf[0] = sf;
    buf[1] = bw;
    buf[2] = cr;
    WriteCommand( RADIO_SET_MODULATIONPARAMS, buf, 3 );
}

void SX1280::SetPacketParams(uint8_t payload_len)
{
    uint8_t buf[7];

    //below parameter used in PACKET_TYPE_LORA or PACKET_TYPE_RANGING	
	buf[0] = 12;	//PreambleLength;
	buf[1] = LORA_PACKET_VARIABLE_LENGTH;	//HeaderType;
	buf[2] = payload_len;	//PayloadLength;
	buf[3] = LORA_CRC_ON;	//CrcMode;
	buf[4] = LORA_IQ_NORMAL;	//InvertIQ;
	buf[5] = NULL;
	buf[6] = NULL;

    WriteCommand( RADIO_SET_PACKETPARAMS, buf, 7 );
}

void SX1280::GetRxBufferStatus( uint8_t *payloadLength, uint8_t *rxStartBufferPointer )
{
    uint8_t status[2];

    ReadCommand( RADIO_GET_RXBUFFERSTATUS, status, 2 );

    // In case of LORA fixed header, the payloadLength is obtained by reading
    // the register REG_LR_PAYLOADLENGTH
    if( ReadRegister( REG_LR_PACKETPARAMS ) >> 7 == 1 )
    {
        *payloadLength = ReadRegister( REG_LR_PAYLOADLENGTH );
    }
	else
	{
		*payloadLength = status[0];
	}
    *rxStartBufferPointer = status[1];
}

void SX1280::GetPacketStatus(int8_t RssiPkt,int8_t SnrPkt)
{  
    uint8_t status[5];

    ReadCommand( RADIO_GET_PACKETSTATUS, status, 5 );

    //for PACKET_TYPE_LORA or PACKET_TYPE_RANGING
    
	RssiPkt = -status[0] / 2;
    if( status[1] < 128 ) 
		SnrPkt = status[1] / 4 ; 
	else
		SnrPkt = ( status[1] - 256 ) / 4;
}
int8_t SX1280::GetRssiInst( void )
{
    uint8_t raw = 0;

    ReadCommand( RADIO_GET_RSSIINST, &raw, 1 );

    return ( int8_t )( -raw / 2 );
}

void SX1280::SetDioIrqParams(uint16_t irq)
{
    uint8_t buf[8];
	uint16_t irqMask;
	uint16_t dio1Mask;
	uint16_t dio2Mask;
	uint16_t dio3Mask;
	
	irqMask = irq;
	dio1Mask = irq;
	dio2Mask = 0;
	dio3Mask = 0;
	
    buf[0] = ( uint8_t )( irqMask >> 8 );
    buf[1] = ( uint8_t )( irqMask & 0xFF );
    buf[2] = ( uint8_t )( dio1Mask >> 8 );
    buf[3] = ( uint8_t )( dio1Mask & 0xFF );
    buf[4] = ( uint8_t )( dio2Mask >> 8 );
    buf[5] = ( uint8_t )( dio2Mask & 0xFF );
    buf[6] = ( uint8_t )( dio3Mask >> 8 );
    buf[7] = ( uint8_t )( dio3Mask & 0xFF );
	
    WriteCommand( RADIO_SET_DIOIRQPARAMS, buf, 8 );
}

uint16_t SX1280::GetIrqStatus( void )
{
    uint8_t irqStatus[2];
	uint16_t IrqStatus;
	
    ReadCommand( RADIO_GET_IRQSTATUS, irqStatus, 2 );
	
	IrqStatus = irqStatus[0];
	IrqStatus = IrqStatus<<8;
	IrqStatus = IrqStatus|irqStatus[1];
	
    return IrqStatus;
}

void SX1280::ClearIrqStatus( uint16_t irq )
{
    uint8_t buf[2];

    buf[0] = ( uint8_t )( irq >> 8 );
    buf[1] = ( uint8_t )( irq & 0xFF );
    WriteCommand( RADIO_CLR_IRQSTATUS, buf, 2 );
}

void SX1280::SetRegulatorMode( uint8_t mode )
{
    WriteCommand( RADIO_SET_REGULATORMODE, &mode, 1 );
}

void SX1280::SetSaveContext( void )
{
    WriteCommand( RADIO_SET_SAVECONTEXT, 0, 0 );
}

void SX1280::TxPacket(uint8_t *payload,uint8_t size)
{
	SetStandby(STDBY_RC);//0:STDBY_RC; 1:STDBY_XOSC
	SetBufferBaseAddress(0,0);//(TX_base_addr,RX_base_addr)

	WriteBuffer(0,payload,size);//(offset,*data,length)
	SetPacketParams(size);//PreambleLength;HeaderType;PayloadLength;CRCType;InvertIQ

	SetDioIrqParams(IRQ_TX_DONE);//TxDone IRQ
	
	SetTx(PERIOBASE_15_US,0);//timeout = 0
	
	//Wait for the IRQ TxDone or Timeout(implement in another function)
}

uint8_t SX1280::WaitForIRQ_TxDone(void)
{
	uint8_t time_out;
	
	time_out = 0;
	while(!digitalRead(RF_DIO1))
	{
		time_out++;
		delay(10);
		if(time_out > 200)	//if timeout , reset the the chip
		{
			ClearIrqStatus(IRQ_TX_DONE);//Clear the IRQ_TX_DONE flag
			SetStandby(STDBY_RC);//0:STDBY_RC; 1:STDBY_XOSC
			Reset_SX1280();		//reset RF
			SX1280_Config();
			Serial.println("WaitFor IRQ_TxDone time out\n");
			return 0;		
		}
	}
	
	//Irq_Status = GetIrqStatus();
	ClearIrqStatus(IRQ_TX_DONE);//Clear the IRQ_TX_DONE flag
	return 1;
}

void SX1280::RxBufferInit(uint8_t *rxpayload,uint16_t *rx_size)
{
	rxbuf_pt = rxpayload;
	rxcnt_pt = rx_size;
}

void SX1280::RxInit(void)
{
	SetBufferBaseAddress(0,0);//(TX_base_addr,RX_base_addr)
	SetDioIrqParams(IRQ_RX_DONE);//RxDone IRQ
	SetRx(PERIOBASE_15_US,0);//timeout = 0
}
uint8_t SX1280::WaitForIRQ_RxDone(void)
{
	uint16_t Irq_Status;
	uint8_t packet_size;
	uint8_t buf_offset;

	if(digitalRead(RF_DIO1))//if IRQ check
	{
		Irq_Status = GetIrqStatus();//read Irq Status
		if((Irq_Status&0x0002) == IRQ_RX_DONE)
		{
			GetRxBufferStatus(&packet_size, &buf_offset);
			ReadBuffer(buf_offset, rxbuf_pt, packet_size+1);
			*rxcnt_pt = packet_size;
			ClearIrqStatus(IRQ_RX_DONE);//Clear the IRQ RxDone flag
			RxInit();
			return 1;
		}
	}
	return 0;
}


void SX1280::SX1280_Config(void)
{
	uint32_t rf_freq_temp;
	int8_t power_temp;
	uint8_t sf_temp;
	uint8_t bw_temp;
	uint8_t cr_temp;
	uint8_t size_temp;
	
	rf_freq_temp = lora_para_pt->rf_freq;
	power_temp = lora_para_pt->tx_power;
	sf_temp = lora_para_pt->lora_sf;
	bw_temp = lora_para_pt->band_width;
	cr_temp = lora_para_pt->code_rate;
	size_temp = lora_para_pt->payload_size;
	
	SetRegulatorMode(USE_LDO);
	SetStandby(STDBY_RC);//0:STDBY_RC; 1:STDBY_XOSC
	SetPacketType(PACKET_TYPE_LORA);
	
	SetModulationParams(sf_temp,bw_temp,cr_temp);

    SetRfFrequency( rf_freq_temp );
    SetTxParams(power_temp, RADIO_RAMP_02_US );
	
	SetPacketParams(size_temp);
	SetBufferBaseAddress( 0x00, 0x00 );
}





