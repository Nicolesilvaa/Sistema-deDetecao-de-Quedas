//Nicole Silva version 1.0
#include <SPI.h>                                //the LoRa device is SPI based so load the SPI library                                         
#include <SX128XLT.h>                           //include the appropriate library  

SX128XLT LT;                                    //create a library class instance called LT

#define NSS 10                                  //select pin on LoRa device
#define NRESET 9                                //reset pin on LoRa device
#define RFBUSY 8                               //busy pin on LoRa device 
#define DIO1 3                                  //DIO1 pin on LoRa device, used for sensing RX and TX done 
#define LORA_DEVICE DEVICE_SX1280               //we need to define the device we are using
#define TXpower 13
#define LED1 7                             //LoRa transmit power in dBm

#define ACKtimeout 1000                         //Acknowledge timeout in mS                      
#define TXtimeout 1000                          //transmit timeout in mS. If 0 return from transmit function after send.  
#define TXattempts 10                           //number of times to attempt to TX and get an Ack before failing  

uint8_t buff[] = "Experimento Lora";                 //the payload to send
uint16_t PayloadCRC;
uint8_t TXPacketL;
uint32_t transmitterTime, startmS, endmS;

const uint16_t NetworkID = 0x3210;              //NetworkID identifies this connection, needs to match value in receiver

void setup(){

  pinMode(LED1, OUTPUT); 
  
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("209_Reliable_Transmitter_AutoACK Starting"));

  SPI.begin();

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, LORA_DEVICE)){

    Serial.println(F("LoRa Device found"));
    delay(1000);
    led_Flash(5,50);

  }
  else{

    Serial.println(F("No LoRa device responding"));
    while (1);
    led_Flash(10, 50); 
  }

  LT.setupLoRa(2445000000, 0, LORA_SF7, LORA_BW_0200, LORA_CR_4_5);

  Serial.println(F("Transmitter ready"));
  Serial.println();
}

void loop(){

  //keep transmitting the packet until an ACK is received
  uint8_t attempts = TXattempts;

  do{

    Serial.print(F("Transmit Payload > "));
    LT.printASCIIArray(buff, sizeof(buff));     //print the payload buffer as ASCII
    Serial.println();
    Serial.flush();

    Serial.print(F("Send attempt "));
    Serial.println(TXattempts - attempts + 1);

    startmS =  millis();
    TXPacketL = LT.transmitReliableAutoACK(buff, sizeof(buff), NetworkID, ACKtimeout, TXtimeout, TXpower, WAIT_TX);
    attempts--;

    if (TXPacketL > 0)
    {
      //if transmitReliable() returns > 0 then transmit and ack was OK
      PayloadCRC = LT.getTXPayloadCRC(TXPacketL);                        //read the actual transmitted CRC from the LoRa device buffer
      endmS = millis();
      packet_is_OK();
      Serial.println();
    }
    else
    {
      //if transmitReliableAutoACK() returns 0 there was an error, timeout etc
      packet_is_Error();
      Serial.println();
    }
    delay(5000);                                        //small delay between tranmission attampts
  }
  while ((TXPacketL == 0) && (attempts != 0));

  if (TXPacketL > 0)
  {
    Serial.println(F("Packet acknowledged"));
    endmS = millis();
  }

  if (attempts == 0)
  {
    Serial.print(F("No acknowledge after "));
    Serial.print(TXattempts);
    Serial.print(F(" attempts"));
  }

  transmitterTime = endmS - startmS;
  Serial.println();
  delay(5000);                                         //have a delay between packets
}

void packet_is_OK(){

  Serial.print(F("LocalNetworkID,0x"));
  Serial.print(NetworkID, HEX);
  Serial.print(F(",TransmittedPayloadCRC,0x"));        //print CRC of transmitted packet
  Serial.print(PayloadCRC, HEX);
  Serial.print(F(",Transmitter Time, ms")); 
  Serial.print(transmitterTime);
  led_Flash(2, 20);
}


void packet_is_Error(){

  Serial.print(F("No Packet acknowledge"));
  LT.printIrqStatus();                                 //prints the text of which IRQs set
  LT.printReliableStatus();                            //print the reliable status
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
