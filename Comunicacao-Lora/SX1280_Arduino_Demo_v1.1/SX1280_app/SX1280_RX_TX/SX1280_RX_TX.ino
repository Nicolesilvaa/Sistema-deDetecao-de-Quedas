
/********************************************************************************************
*	SX1280_app.pde - App demo for SX1280
*	@version 1.0  
*********************************************************************************************	
Caution: 
	Make sure the power supply is UNDER 3.3V. Or the module will be destory!!
	
Usage:
	1. connect the LoRa1280 module and Arduino NANO as below
	
		Arduino NANO				LoRa1280
		   D10(SS)		<------->    NSS
		   D13(SCK)		<------->    SCK
		   D11(MOSI)	<------->    MOSI
		   D12(MISO)	<------->    MISO
		   D9   	 	<------->    NRESET
		   D8   	 	<------->    BUSY
		   D7   	 	<------->    DIO1
		   3V3   	 	<------->    VCC
		   GND   	 	<------->    GND
	   
	2. compile the code for master and slave respectively.
	
*********************************************************************************************/
#define Versao 1.0 // Nicole Silva

#include <SPI.h>
#include <SoftwareSerial.h>
#include <SX1280.h>

#define RF_FREQUENCY                2410000000 // Hz
#define RADIO_GET_RSSIINST          0x1F //Retorna Intensidade sinal 
#define IRQ_RX_TX_TIMEOUT           0x4000 //Tempo total disponível para que uma requisição seja processada e respondida

/* if compile the code for slave, uncommand the below line*/
#define	MASTER //Master - Transmissor Slave - Receptor

SX1280 LoRa1280;	//define a object of class SX1280.
loRa_Para_t	lora_para;	//define a struct to hold lora parameters.


int8_t power_table[32]={-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13}; // Potência transmissão
uint8_t sf_table[8]={LORA_SF5,LORA_SF6,LORA_SF7,LORA_SF8,LORA_SF9,LORA_SF10,LORA_SF11,LORA_SF12}; // Espalhamento espectral
uint8_t bw_table[4]={LORA_BW_0200,LORA_BW_0400,LORA_BW_0800,LORA_BW_1600}; // Largura de banda
uint8_t cr_table[7]={LORA_CR_4_5,LORA_CR_4_6,LORA_CR_4_7,LORA_CR_4_8,LORA_CR_LI_4_5,LORA_CR_LI_4_6,LORA_CR_LI_4_7}; // Code rate 


uint8_t tx_buf[]={"Olá estou recebendo informações"}; // Informações que estão sendo printada no serial (Receptor)
uint16_t tx_cont = 0;

uint8_t  rx_buf[20]={}; // Dado recebido 
uint16_t rx_cont = 0;
uint16_t rx_size = 0;

uint8_t val;
uint8_t state;

// calculaPing(){
//   // Função que vai enviar e receber mensagem 1000x e medir o tempo médio delas


// }

void setup(void) 
{
	bool temp;
	Serial.begin(9600);	//UART Init
	
	lora_para.rf_freq    = RF_FREQUENCY;
	lora_para.tx_power   = 13;			//-18~13
	lora_para.lora_sf    = LORA_SF12; //Espalhamento Espectral
	lora_para.band_width = LORA_BW_0200;
	lora_para.code_rate  = LORA_CR_4_5;
	lora_para.payload_size = sizeof(tx_buf);

	temp = LoRa1280.Init(&lora_para);
	
	if(0 == temp)
	{
		Serial.println("Init fail!");
	}
	#ifdef SLAVE
		LoRa1280.RxBufferInit(rx_buf,&rx_size);
                LoRa1280.RxInit();    // wait for packet from master
		Serial.println("Comunicação iniciada Receptor");
	#else
		Serial.println("Comunicação Iniciada Transmissor!");
	#endif
}

void loop(void) 
{
#ifdef SLAVE

	state = LoRa1280.WaitForIRQ_RxDone();
	if(state)    // wait for RxDone interrupt
	{
		rx_cont++;
		Serial.print("rx_cont = ");
		Serial.print(rx_cont);
		Serial.print(" data:");
		Serial.write(rx_buf,rx_size);    // print out the receive data
		Serial.println();
	}
	
#else

	val= Serial.read();  // please make sure serial is OK befor runing this code
  
    
  LoRa1280.TxPacket(tx_buf,sizeof(tx_buf));
	state = LoRa1280.WaitForIRQ_TxDone();
	if(state)
	{
		tx_cont++;
		Serial.print("Contador \n");
		Serial.println(tx_cont);
    Serial.println();
  }


uint8_t  rx_buf[20]={}; // Dado recebido 
uint16_t rx_cont = 0;
uint16_t rx_size = 0;

uint8_t val;
uint8_t state;

// calculaPing(){
//   // Função que vai enviar e receber mensagem 1000x e medir o tempo médio delas

Não conectado. Selecione uma placa e uma porta para conectar automaticamente.
Nova linha


    Serial.println("Intensidade de sinal(RSSI) em dBm(decibéis para um miliwatt )\n");
    Serial.print(RADIO_GET_RSSIINST);
    Serial.println();

    Serial.println("Tempo médio da mensagem \n ");
    Serial.print(IRQ_RX_TX_TIMEOUT);
    Serial.println();

    Serial.println("Variação de tempo \n ");
    Serial.print(IRQ_RANGING_MASTER_RESULT_TIMEOUT);
    Serial.println();



 

    // for(int  i = 0; i < sizeof(tx_buf); i++){

    // Serial.print("tx_buf = ");
		// Serial.println(tx_buf[i]);
    // delay(1000);

    // }
    
	}
    delay(1000);
	
#endif
}
