
#include <SPI.h>                                               //the SX128X device is SPI based so load the SPI library                                         
#include <SX128XLT.h>                                          //include the appropriate library  
#include "Settings.h"                                         //include the setiings file, frequencies, LoRa settings etc   
                                      
SX128XLT LT;                                                   //create a library class instance called LT

uint8_t TXPacketL;
uint32_t TXPacketCount, startmS, endmS;

uint8_t buff[] = "Experimento  Lora";


void setup(){

  pinMode(LED1, OUTPUT);                                   //setup pin as output for indicator LED
  led_Flash(2, 125);                                       //two quick LED flashes to indicate program start

  Serial.begin(9600);
  Serial.println();
  Serial.print(F(__TIME__));
  Serial.print(F(" "));
  Serial.println(F(__DATE__));
  Serial.println(F(Program_Version));
  Serial.println();
  Serial.println(F("Iniciando comunicacão - Transmissor"));
  Serial.println(F("Lasid UFBA"));

  SPI.begin();

  //SPI beginTranscation is normally part of library routines, but if it is disabled in library
  //a single instance is needed here, so uncomment the program line below
  //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  //setup hardware pins used by device, then check if device is found

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE)){

    Serial.println(F("LoRa Device found"));
    //led_Flash(2, 125);                                   //two further quick LED flashes to indicate device found
    delay(20);

  }
  else{

    Serial.println(F("No device responding"));
    while (1)
    {
      led_Flash(50, 50);                                 //long fast speed LED flash indicates device error
    }
  }

  //The function call list below shows the complete setup for the LoRa device using the information defined in the
  //Settings.h file.
  //The 'Setup LoRa device' list below can be replaced with a single function call;
  //LT.setupLoRa(Frequency, Offset, SF, Bandwidth, CodeRate);


  //***************************************************************************************************
  //Setup LoRa device
  //***************************************************************************************************
  LT.setMode(MODE_STDBY_RC);
  LT.setRegulatorMode(USE_LDO);
  LT.setPacketType(PACKET_TYPE_LORA);
  LT.setRfFrequency(Frequency, Offset);
  LT.setBufferBaseAddress(0, 0);
  LT.setModulationParams(SF, Bandwidth, CodeRate);
  LT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL, 0, 0);
  LT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);
  LT.setHighSensitivity();

  //LT.setLowPowerTX();
  //***************************************************************************************************
  Serial.println();
  Serial.print(F("Dados Transmitidos - Settings "));
  Serial.println();
  LT.printModemSettings();                               //reads and prints the configured LoRa settings, useful check
  Serial.println();

}

void loop(){

  Serial.print(TXpower);                                       //print the transmit power defined
  Serial.print(F("dBm "));
  Serial.print(F("Packet> "));
  Serial.flush();

  TXPacketL = sizeof(buff);                                    //set TXPacketL to length of array
  buff[TXPacketL - 1] = '*';                                   //replace null character at buffer end so its visible on reciver

  LT.printASCIIPacket(buff, TXPacketL);                        //print the buffer (the sent packet) as ASCII

  digitalWrite(LED1, HIGH);
  startmS =  millis();                                           //start transmit timer

  if (LT.transmit(buff, TXPacketL, 10000, TXpower, WAIT_TX)){  //will return packet length sent if OK, otherwise 0 if transmit, timeout 10 seconds{
    endmS = millis();                                          //packet sent, note end time
    TXPacketCount++;
    packet_is_OK();
  }else{
    packet_is_Error();
  
  }                              //transmit packet returned 0, there was an error

  
  digitalWrite(LED1, LOW);
  Serial.println();
  delay(40);   //have a delay between packets
}


void packet_is_OK(){
//if here packet has been sent OK

  uint16_t localCRC;
  uint32_t transmitTime;

  transmitTime = endmS - startmS;

  //print total of packets sent OK (1000 packets)
  Serial.print(F("  BytesSent,"));
  Serial.print(TXPacketL);                             //print transmitted packet length
  localCRC = LT.CRCCCITT(buff, TXPacketL, 0xFFFF);
  Serial.print(F("  CRC,"));
  Serial.print(localCRC, HEX);                              //print CRC of sent packet
  Serial.print(F("  TransmitTime,"));
  Serial.print(transmitTime);                       //print transmit time of packet
  Serial.print(F("mS"));
  Serial.print(F("  PacketsSent,"));
  Serial.println(TXPacketCount);  
    
  led_Flash(2, 20);
  tempoMedio(TXPacketCount,transmitTime);
    
   
}


void packet_is_Error(){

  //if here there was an error transmitting packet
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                  //read the the interrupt register
  Serial.print(F(" SendError,"));
  Serial.print(F("Length,"));
  Serial.print(TXPacketL);                         //print transmitted packet length
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);                    //print IRQ status
  LT.printIrqStatus();                             //prints the text of which IRQs set
}

void tempoMedio(uint32_t &TXPacketCount, uint32_t &transmitTime){

  uint32_t somaTime;
  somaTime += transmitTime;

  //Calcula o tempo médio de envio de pacotes para cada intervalo de 200 envios.
  if(TXPacketCount % 200 == 0){
    
    uint32_t  tempoMedio = somaTime/200;

    Serial.println();
    Serial.print("Tempo total da transmissão de 200 mensagens = ");
    Serial.print(somaTime);
    Serial.println(" mS");
    Serial.print("Tempo médio de transmissão de 200 mensagens = ");
    Serial.print(tempoMedio/60000); //Convertendo para minutos
    Serial.print("m ");
    Serial.print(tempoMedio/1000); //Convertendo para segundos
    Serial.print("s");
    Serial.println();

  }
} 

void variaSF_TXPower((uint32_t &TXPacketCount, uint8_t &SpreadingFactor[], uint8_t &SF){

  int sizeSF = sizeof(SpreadingFactor);
  int i = 0;

  if(TXPacketCount % 200 == 0 && i < sizeSF){ 

    SF = SpreadingFactor[i++];

  }

  return SF;
}

void led_Flash(uint16_t flashes, uint16_t delaymS){

  uint16_t index;
  for (index = 1; index <= flashes; index++){
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }

}


