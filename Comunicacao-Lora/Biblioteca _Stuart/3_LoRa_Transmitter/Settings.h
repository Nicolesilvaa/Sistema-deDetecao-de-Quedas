//Settings//**************************************************************************************************************
#define Program_Version "Teste_NicoleSilva"
#define NSS 10
#define RFBUSY 8
#define NRESET 9
#define LED1 7
#define DIO1 3
#define LORA_DEVICE DEVICE_SX1280  //we need to define the device we are using

uint8_t SpreadingFactor[] = {LORA_SF5,LORA_SF6,LORA_SF7,LORA_SF8,LORA_SF9,LORA_SF10,LORA_SF11,LORA_SF12};  //LoRa spreading factor

//LoRa Modem Parameters
const uint32_t Frequency = 2445000000;     //frequency of transmissions
const int32_t Offset = 0;                  //offset frequency for calibration purposes
const uint8_t Bandwidth = LORA_BW_0200;    //LoRa bandwidth
const uint8_t SF = SpreadingFactor[0];
const uint8_t CodeRate = LORA_CR_4_5;     

const int8_t TXpower = 13;  //Power for transmissions in dBm

const uint16_t packet_delay = 1000;  //mS delay between packets
//**************************************************************************************************************
  
