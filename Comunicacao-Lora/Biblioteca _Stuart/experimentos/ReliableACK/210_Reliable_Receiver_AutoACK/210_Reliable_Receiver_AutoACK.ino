//Nicole Silva version 1.0

#include <SPI.h>                                //the LoRa device is SPI based so load the SPI library
#include <SX128XLT.h>                           //include the appropriate library   

SX128XLT LT;                                    //create a library class instance called LT

#define NSS 10                                  //select pin on LoRa device
#define NRESET 9                                //reset pin on LoRa device
#define RFBUSY 8                               //busy pin on LoRa device 
#define DIO1 3                                  //DIO1 pin on LoRa device, used for sensing RX and TX done
#define LORA_DEVICE DEVICE_SX1280               //we need to define the device we are using
#define LED1 7

#define ACKdelay 100                            //delay in mS before sending acknowledge                    
#define RXtimeout 60000                         //receive timeout in mS.  
#define TXpower 13                           //dBm power to use for ACK   

const uint8_t RXBUFFER_SIZE = 251;              //RX buffer size, set to max payload length of 251, or maximum expected length
uint8_t RXBUFFER[RXBUFFER_SIZE];                //create the buffer that received packets are copied into

uint8_t RXPacketL;                              //stores length of packet received
uint8_t RXPayloadL;                             //stores length of payload received
uint8_t PacketOK;                               //set to > 0 if packetOK
int16_t PacketRSSI;                             //stores RSSI of received packet
uint16_t LocalPayloadCRC;                       //locally calculated CRC of payload
uint16_t RXPayloadCRC;                          //CRC of payload received in packet
uint16_t TransmitterNetworkID;                  //the NetworkID from the transmitted and received packet
uint32_t receiverTime, startmS, endmS;

const uint16_t NetworkID = 0x3210;              //NetworkID identifies this connection, needs to match value in transmitter

void setup(){

  pinMode(LED1, OUTPUT);  

  Serial.begin(115200);
  Serial.println();
  Serial.println(F("210_Reliable_Receiver_AutoACK Starting"));

  SPI.begin();

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, LORA_DEVICE))
  {
    Serial.println(F("LoRa Device found"));
    delay(1000);
  }
  else
  {
    Serial.println(F("No LoRa device responding"));
    while (1);
    led_Flash(50, 50); 
  }

  LT.setupLoRa(2445000000, 0, LORA_SF7, LORA_BW_0200, LORA_CR_4_5);

  Serial.println(F("Receiver ready"));
  Serial.println();
}

void loop(){
  startmS = millis();
  PacketOK = LT.receiveReliableAutoACK(RXBUFFER, RXBUFFER_SIZE, NetworkID, ACKdelay, TXpower, RXtimeout, WAIT_RX); //wait for a packet to arrive with 60seconds (60000mS) timeout

  RXPacketL = LT.readRXPacketL();               //get the received packet length
  RXPayloadL = RXPacketL - 4;                   //payload length is always 4 bytes less than packet length
  PacketRSSI = LT.readPacketRSSI();             //read the received packets RSSI value

  if (PacketOK > 0){

    //if the LT.receiveReliable() returns a value > 0 for PacketOK then packet was received OK
    endmS = millis();
    packet_is_OK();
  }
  else{

    //if the LT.receiveReliable() function detects an error PacketOK is 0
    packet_is_Error();
  }

  receiverTime = endmS - startmS;
  Serial.println();
}


void packet_is_OK(){

  Serial.print(F("Payload received OK > "));
  LT.printASCIIPacket(RXBUFFER, RXPayloadL);
  Serial.println();
  printPacketDetails();
  Serial.println();
  Serial.print(F(", Receiver Time, ms")); 
  Serial.print(receiverTime);
  led_Flash(2, 20);
}


void packet_is_Error(){

  uint16_t IRQStatus;

  IRQStatus = LT.readIrqStatus();                  //read the LoRa device IRQ status register
  Serial.print(F("Error "));

  if (IRQStatus & IRQ_RX_TIMEOUT)                  //check for an RX timeout
  {
    Serial.print(F(" RXTimeout "));
  }
  else
  {
    printPacketDetails();
  }
}


void printPacketDetails(){

  LocalPayloadCRC = LT.CRCCCITT(RXBUFFER, RXPayloadL, 0xFFFF);  //calculate payload crc from the received RXBUFFER
  TransmitterNetworkID = LT.getRXNetworkID(RXPacketL);
  RXPayloadCRC = LT.getRXPayloadCRC(RXPacketL);

  Serial.print(F("LocalNetworkID,0x"));
  Serial.print(NetworkID, HEX);
  Serial.print(F(",TransmitterNetworkID,0x"));
  Serial.print(TransmitterNetworkID, HEX);
  Serial.print(F(",LocalPayloadCRC,0x"));
  Serial.print(LocalPayloadCRC, HEX);
  Serial.print(F(",RXPayloadCRC,0x"));
  Serial.print(RXPayloadCRC, HEX);
  LT.printReliableStatus();
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
