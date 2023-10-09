/********************************************************************************************
*	SX1280.h - Library for SX1280
*	Copyright(C) 2018 NiceRF.All right reserved.  
*	@version 1.1  
*	This library is suit for LoRa1280 in loRa mode
*	please make sure the supply power of you board is UNDER 3.3V!! Otherwise, the module will be destory!!
*	The configuration of both the modules should be the same.
*********************************************************************************************/
#ifndef __SX1280_H__
#define __SX1280_H__

#include "Arduino.h"

typedef		boolean			bool;
typedef 	unsigned char	uint8_t;
typedef 	unsigned int	uint16_t;
typedef 	unsigned long	uint32_t;

typedef struct
{
	uint32_t rf_freq;
	 int8_t  tx_power;
	uint8_t  lora_sf;
	uint8_t  band_width;
	uint8_t  code_rate;
	uint8_t	 payload_size;
	
}loRa_Para_t;

#define    IRQ_RADIO_NONE                         0x0000
#define    IRQ_TX_DONE                            0x0001
#define    IRQ_RX_DONE                            0x0002
#define    IRQ_SYNCWORD_VALID                     0x0004
#define    IRQ_SYNCWORD_ERROR                     0x0008
#define    IRQ_HEADER_VALID                       0x0010
#define    IRQ_HEADER_ERROR                       0x0020
#define    IRQ_CRC_ERROR                          0x0040
#define    IRQ_RANGING_SLAVE_RESPONSE_DONE        0x0080
#define    IRQ_RANGING_SLAVE_REQUEST_DISCARDED    0x0100
#define    IRQ_RANGING_MASTER_RESULT_VALID        0x0200
#define    IRQ_RANGING_MASTER_RESULT_TIMEOUT      0x0400
#define    IRQ_RANGING_SLAVE_REQUEST_VALID        0x0800
#define    IRQ_CAD_DONE                           0x1000
#define    IRQ_CAD_ACTIVITY_DETECTED              0x2000
#define    IRQ_RX_TX_TIMEOUT                      0x4000
#define    IRQ_PREAMBLE_DETECTED                  0x8000
#define    IRQ_RADIO_ALL                          0xFFFF

#define LORA_SF5   			0x50
#define LORA_SF6   			0x60
#define LORA_SF7   			0x70
#define LORA_SF8   			0x80
#define LORA_SF9   			0x90
#define LORA_SF10  			0xA0
#define LORA_SF11  			0xB0
#define	LORA_SF12  			0xC0

#define LORA_BW_0200   		0x34
#define LORA_BW_0400   		0x26
#define LORA_BW_0800   		0x18
#define LORA_BW_1600   		0x0A

#define LORA_CR_4_5       	0x01
#define LORA_CR_4_6       	0x02
#define LORA_CR_4_7       	0x03
#define LORA_CR_4_8       	0x04
#define LORA_CR_LI_4_5    	0x05
#define LORA_CR_LI_4_6    	0x06
#define LORA_CR_LI_4_7    	0x07

/**********************************************/
#define RADIO_GET_STATUS                  0xC0
#define RADIO_WRITE_REGISTER              0x18
#define RADIO_READ_REGISTER               0x19
#define RADIO_WRITE_BUFFER                0x1A
#define RADIO_READ_BUFFER                 0x1B
#define RADIO_SET_SLEEP                   0x84
#define RADIO_SET_STANDBY                 0x80
#define RADIO_SET_FS                      0xC1
#define RADIO_SET_TX                      0x83
#define RADIO_SET_RX                      0x82
#define RADIO_SET_RXDUTYCYCLE             0x94
#define RADIO_SET_CAD                     0xC5
#define RADIO_SET_TXCONTINUOUSWAVE        0xD1
#define RADIO_SET_TXCONTINUOUSPREAMBLE    0xD2
#define RADIO_SET_PACKETTYPE              0x8A
#define RADIO_GET_PACKETTYPE              0x03
#define RADIO_SET_RFFREQUENCY             0x86
#define RADIO_SET_TXPARAMS                0x8E
#define RADIO_SET_CADPARAMS               0x88
#define RADIO_SET_BUFFERBASEADDRESS       0x8F
#define RADIO_SET_MODULATIONPARAMS        0x8B
#define RADIO_SET_PACKETPARAMS            0x8C
#define RADIO_GET_RXBUFFERSTATUS          0x17
#define RADIO_GET_PACKETSTATUS            0x1D
#define RADIO_GET_RSSIINST                0x1F
#define RADIO_SET_DIOIRQPARAMS            0x8D
#define RADIO_GET_IRQSTATUS               0x15
#define RADIO_CLR_IRQSTATUS               0x97
#define RADIO_CALIBRATE                   0x89
#define RADIO_SET_REGULATORMODE           0x96
#define RADIO_SET_SAVECONTEXT             0xD5
#define RADIO_SET_AUTOTX                  0x98
#define RADIO_SET_AUTOFS                  0x9E
#define RADIO_SET_LONGPREAMBLE            0x9B
#define RADIO_SET_UARTSPEED               0x9D
#define RADIO_SET_RANGING_ROLE            0xA3

#define STDBY_RC     0x00
#define STDBY_XOSC   0x01

#define REG_LR_PACKETPARAMS                         0x903
#define REG_LR_PAYLOADLENGTH                        0x901

#define PERIOBASE_15_US    0x00
#define PERIOBASE_62_US    0x01
#define PERIOBASE_01_MS    0x02
#define PERIOBASE_04_MS    0x03

#define PACKET_TYPE_GFSK    0x00                  
#define	PACKET_TYPE_LORA	0x01
#define PACKET_TYPE_RANGING	0x02
#define PACKET_TYPE_FLRC	0x03
#define PACKET_TYPE_BLE		0x04

#define RADIO_RAMP_02_US    0x00
#define RADIO_RAMP_04_US    0x20
#define RADIO_RAMP_06_US    0x40
#define RADIO_RAMP_08_US    0x60
#define RADIO_RAMP_10_US    0x80
#define RADIO_RAMP_12_US    0xA0
#define RADIO_RAMP_16_US	0xC0
#define RADIO_RAMP_20_US	0xE0

#define LORA_CAD_01_SYMBOL	0x00
#define LORA_CAD_02_SYMBOL	0x20
#define LORA_CAD_04_SYMBOL	0x40
#define LORA_CAD_08_SYMBOL	0x60
#define LORA_CAD_16_SYMBOL	0x80

#define    LORA_PACKET_VARIABLE_LENGTH            0x00    //!< The packet is on variable size, header included
#define    LORA_PACKET_FIXED_LENGTH               0x80    //!< The packet is known on both sides, no header included in the packet
#define    LORA_PACKET_EXPLICIT                   LORA_PACKET_VARIABLE_LENGTH
#define    LORA_PACKET_IMPLICIT                   LORA_PACKET_FIXED_LENGTH

#define    LORA_CRC_ON                            0x20        //!< CRC activated
#define    LORA_CRC_OFF                           0x00         //!< CRC not used

#define    LORA_IQ_NORMAL                         0x40
#define    LORA_IQ_INVERTED                       0x00

#define    USE_LDO     0x00           //! Use LDO (default value)
#define    USE_DCDC    0x01           //! Use DCDC

#define REG_LR_DEVICERANGINGADDR                    0x0916
#define REG_LR_REQUESTRANGINGADDR                   0x0912
#define REG_LR_RANGINGIDCHECKLENGTH                 0x0931

#define RANGING_IDCHECK_LENGTH_08_BITS    	0x00
#define RANGING_IDCHECK_LENGTH_16_BITS		0x01
#define RANGING_IDCHECK_LENGTH_24_BITS		0x02
#define RANGING_IDCHECK_LENGTH_32_BITS		0x03

#define  RANGING_RESULT_RAW           0x00
#define  RANGING_RESULT_AVERAGED      0x01
#define  RANGING_RESULT_DEBIASED      0x02
#define  RANGING_RESULT_FILTERED      0x03

#define REG_LR_RANGINGRESULTCONFIG                  0x0924
#define MASK_RANGINGMUXSEL                          0xCF
#define REG_LR_RANGINGRESULTBASEADDR                0x0961
#define REG_LR_RANGINGRERXTXDELAYCAL                0x092C

#define     RADIO_RANGING_ROLE_SLAVE     0x00
#define     RADIO_RANGING_ROLE_MASTER    0x01

/*******************FOR GFSK***************************/
#define    GFS_BLE_BR_2_000_BW_2_4    0x04
#define    GFS_BLE_BR_1_600_BW_2_4    0x28
#define    GFS_BLE_BR_1_000_BW_2_4    0x4C
#define    GFS_BLE_BR_1_000_BW_1_2    0x45
#define    GFS_BLE_BR_0_800_BW_2_4    0x70
#define    GFS_BLE_BR_0_800_BW_1_2    0x69
#define    GFS_BLE_BR_0_500_BW_1_2    0x8D
#define    GFS_BLE_BR_0_500_BW_0_6    0x86
#define    GFS_BLE_BR_0_400_BW_1_2    0xB1
#define    GFS_BLE_BR_0_400_BW_0_6    0xAA
#define    GFS_BLE_BR_0_250_BW_0_6    0xCE
#define    GFS_BLE_BR_0_250_BW_0_3    0xC7
#define    GFS_BLE_BR_0_125_BW_0_3    0xEF
	
#define    GFS_BLE_MOD_IND_0_35   0
#define    GFS_BLE_MOD_IND_0_50   1
#define    GFS_BLE_MOD_IND_0_75   2
#define    GFS_BLE_MOD_IND_1_00   3
#define    GFS_BLE_MOD_IND_1_25   4
#define    GFS_BLE_MOD_IND_1_50   5
#define    GFS_BLE_MOD_IND_1_75   6
#define    GFS_BLE_MOD_IND_2_00   7
#define    GFS_BLE_MOD_IND_2_25   8
#define    GFS_BLE_MOD_IND_2_50   9
#define    GFS_BLE_MOD_IND_2_75   10
#define    GFS_BLE_MOD_IND_3_00   11
#define    GFS_BLE_MOD_IND_3_25   12
#define    GFS_BLE_MOD_IND_3_50   13
#define    GFS_BLE_MOD_IND_3_75   14
#define    GFS_BLE_MOD_IND_4_00   15	
	
#define    RADIO_MOD_SHAPING_BT_OFF                0x00        
#define    RADIO_MOD_SHAPING_BT_1_0                0x10
#define    RADIO_MOD_SHAPING_BT_0_5         0x20	
	

#define    PREAMBLE_LENGTH_04_BITS          0x00         //!< Preamble length: 04 bits
#define    PREAMBLE_LENGTH_08_BITS          0x10         //!< Preamble length: 08 bits
#define    PREAMBLE_LENGTH_12_BITS          0x20         //!< Preamble length: 12 bits
#define    PREAMBLE_LENGTH_16_BITS          0x30         //!< Preamble length: 16 bits
#define    PREAMBLE_LENGTH_20_BITS          0x40         //!< Preamble length: 20 bits
#define    PREAMBLE_LENGTH_24_BITS          0x50         //!< Preamble length: 24 bits
#define    PREAMBLE_LENGTH_28_BITS          0x60         //!< Preamble length: 28 bits
#define    PREAMBLE_LENGTH_32_BITS          0x70         //!< Preamble length: 32 bits
	
#define    GFS_SYNCWORD_LENGTH_1_BYTE       0x00        //!< Sync word length: 1 byte
#define    GFS_SYNCWORD_LENGTH_2_BYTE       0x02        //!< Sync word length: 2 bytes
#define    GFS_SYNCWORD_LENGTH_3_BYTE       0x04        //!< Sync word length: 3 bytes
#define    GFS_SYNCWORD_LENGTH_4_BYTE       0x06        //!< Sync word length: 4 bytes
#define    GFS_SYNCWORD_LENGTH_5_BYTE       0x08        //!< Sync word length: 5 bytes	
	
#define    RADIO_RX_MATCH_SYNCWORD_OFF      0x00 //!<do not search for SyncWord
#define    RADIO_RX_MATCH_SYNCWORD_1        0x10
#define    RADIO_RX_MATCH_SYNCWORD_2        0x20
#define    RADIO_RX_MATCH_SYNCWORD_1_2      0x30
#define    RADIO_RX_MATCH_SYNCWORD_3        0x40
#define    RADIO_RX_MATCH_SYNCWORD_1_3      0x50
#define    RADIO_RX_MATCH_SYNCWORD_2_3      0x60
#define    RADIO_RX_MATCH_SYNCWORD_1_2_3    0x70	
	
#define    RADIO_PACKET_FIXED_LENGTH    0x00//!< The packet is known on both sides, no header included in the packet
#define    RADIO_PACKET_VARIABLE_LENGTH 0x20//!< The packet is on variable size, header included
    
#define		RADIO_CRC_OFF        0x00
#define		RADIO_CRC_1_BYTES    0x10
#define    	RADIO_CRC_2_BYTES    0x20
#define    	RADIO_CRC_3_BYTES    0x30
	
#define		RADIO_WHITENING_ON    0x00
#define		RADIO_WHITENING_OFF   0x08
/*******************END FOR GFSK***************************/


class SX1280
{
	
public:
	// Constructor.
	SX1280(uint8_t NSS_Pin = 10, uint8_t NRESET_Pin = 9,uint8_t BUSY_Pin = 8,uint8_t DIO1_Pin = 7);
	bool Init(loRa_Para_t *lp_pt);
	
	void TxPacket(uint8_t *payload,uint8_t size);
	uint8_t WaitForIRQ_TxDone(void);
	void RxBufferInit(uint8_t *rxpayload,uint16_t *rx_size);
	void RxInit();
	uint8_t WaitForIRQ_RxDone(void);
	void Reset_SX1280(void);
	void SetSleep(void);
	void SetStandby(uint8_t StdbyConfig);
	
	uint8_t *rxbuf_pt;
	uint16_t *rxcnt_pt;
	
protected:
	void SPI_Init(void);	// Initialise SPI. @note Use standard Arduino SPI interface
	void Pin_Init(void);	//Initialise other pin.
	uint8_t spi_rw(uint8_t value_w);
	void CheckBusy(void);
	void WriteCommand(uint8_t Opcode, uint8_t *buffer, uint16_t size );
	void ReadCommand( uint8_t Opcode, uint8_t *buffer, uint16_t size );
	void WriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size );
	void WriteRegister( uint16_t address, uint8_t value );
	void ReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size );
	uint8_t ReadRegister( uint16_t address );
	void WriteBuffer(uint8_t offset, uint8_t *data, uint8_t length);
	void ReadBuffer(uint8_t offset, uint8_t *data, uint8_t length);
	
	void SetTx(uint8_t periodBase, uint16_t periodBaseCount);
	void SetRx(uint8_t periodBase, uint16_t periodBaseCount);
	void SetRxDutyCycle(uint8_t PeriodBase, uint16_t NbStepRx, uint16_t RxNbStepSleep );
	void SetLongPreamble( uint8_t enable );
	void SetCad( void );
	void SetTxContinuousWave( void );
	void SetTxContinuousPreamble( void );
	void SetPacketType( uint8_t packetType );
	uint8_t GetPacketType(void);
	void SetRfFrequency( uint32_t frequency );
	void SetTxParams( int8_t power, uint8_t rampTime );
	void SetCadParams( uint8_t cadSymbolNum );
	void SetBufferBaseAddress( uint8_t txBaseAddress, uint8_t rxBaseAddress );
	void SetModulationParams(uint8_t sf, uint8_t bw, uint8_t cr);
	void SetPacketParams(uint8_t payload_len);
	void GetRxBufferStatus( uint8_t *payloadLength, uint8_t *rxStartBufferPointer );
	void GetPacketStatus(int8_t RssiPkt,int8_t SnrPkt);
	int8_t GetRssiInst( void );
	void SetDioIrqParams(uint16_t irq);
	uint16_t GetIrqStatus( void );
	void ClearIrqStatus( uint16_t irq );
	void SetRegulatorMode( uint8_t mode );
	void SetSaveContext( void );
	void SX1280_Config(void);
	
private:
	uint8_t SPI_NSS;	//output,SPI slave select pin
	uint8_t RF_NRESET;	//output,hard reset of rf chip
	uint8_t RF_BUSY;	//input
	uint8_t RF_DIO1;	//input
	
};

#endif














