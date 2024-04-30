//Settings //***************************************************************************************************
#define Program_Version "Teste_NicoleSilva"
#define NSS 10
#define RFBUSY 8
#define NRESET 9
#define LED1 7
#define DIO1 3
#define DIO2 -1                 //not used 
#define DIO3 -1                 //not used
#define RX_EN -1                //pin for RX enable, used on some SX1280 devices, set to -1 if not used
#define TX_EN -1                //pin for TX enable, used on some SX1280 devices, set to -1 if not used  
#define BUZZER -1               //pin for BUZZER, set to -1 if not used 

#define LORA_DEVICE DEVICE_SX1280               //we need to define the device we are using  

uint8_t SpreadingFactor[] = {LORA_SF5,LORA_SF6,LORA_SF7,LORA_SF8,LORA_SF9,LORA_SF10,LORA_SF11,LORA_SF12 };  //LoRa spreading factor

//LoRa Modem Parameters
const uint32_t Frequency = 2445000000;           //frequency of transmissions
const int32_t Offset = 0;                        //offset frequency for calibration purposes
const uint8_t Bandwidth = LORA_BW_0200;          //LoRa bandwidth
const uint8_t SF = SpreadingFactor[7];      //LoRa spreading factor
const uint8_t CodeRate = LORA_CR_4_5;            //LoRa coding rate

const int8_t TXpower = 13;                       //Power for transmissions in dBm

const uint16_t packet_delay = 1000;              //mS delay between packets

#define RXBUFFER_SIZE 255                        //RX buffer size  
 //**************************************************************************************************************
