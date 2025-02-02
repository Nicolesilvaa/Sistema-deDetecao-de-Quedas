/*******************************************************************************************************
  Programs for Arduino - Copyright of the author Stuart Robinson - 21/09/21

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/


/*******************************************************************************************************
  Program Operation - This is a minimum setup LoRa test transmitter. A packet containing the ASCII text
  "Hello World 1234567890" is sent using the frequency and LoRa settings specified in the LT.setupLoRa()
  command. The pins to access the lora device need to be defined at the top of the program also.

  This program does not need the DIO1 pin on the LoRa device connected.

  The details of the packet sent and any errors are shown on the Arduino IDE Serial Monitor, together with
  the transmit power used and the packet length. The matching receiver program, '4_LoRa_Receiver' can be used
  to check the packets are being sent correctly, the frequency and LoRa settings (in the LT.setupLoRa()
  commands) must be the same for the transmitter and receiver programs. Sample Serial Monitor output;

  10dBm Packet> Hello World 1234567890*  BytesSent,23  PacketsSent,6

  This is a version of example 3_LoRa_Transmitter.ino that does not require the use of the DIO1 pin to
  check for transmit done. In addition no NRESET pin is needed either, so its a program for use with a
  minimum pin count Arduino. Leave the DIO1 and NRESET pins on the LoRa device not connected.

  For an example of a more detailed configuration for a transmitter, see program 103_LoRa_Transmitter.

  Serial monitor baud rate is set at 9600
*******************************************************************************************************/

#include <SPI.h>                                //the lora device is SPI based so load the SPI library                                         
#include <SX128XLT.h>                           //include the appropriate library  
#include<Wire.h>                                //Biblioteca para comunicação I2C
#include<math.h>                                //Biblioteca de Calculos matematicos

SX128XLT LT;                                    //create a library class instance called LT

#define NSS 10                                  //select pin on LoRa device
#define NRESET 9                                //reset pin on LoRa device 
#define RFBUSY 8                                //busy pin on LoRa device
#define LORA_DEVICE DEVICE_SX1280               //we need to define the device we are using
#define TXpower 40                              //LoRa transmit power in dBm

uint8_t TXPacketL;
uint32_t TXPacketCount;

uint8_t buff[] = "";      //the message to send



// Variaveis da queda

bool queda = false;

const int MPU_addr=0x68; //Endereço do sensor

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; //Variaveis para pegar os valores medidos

float AcnX, AcnY, AcnZ;

float AcVertical, AcLateral, AcHorizontal;
float gyVertical, gyLateral, gyHorizontal;

// Fim das variaveis da queda




// Funções de Queda


float converteGraus(int16_t valor, int16_t escala) {
    float newValor = (int)valor;
    float newEscala = (int)escala;
    float resultado = (newValor * newEscala )/ 32768.00;
    return resultado;
}

float converteGravidade(int16_t valor, int16_t escala) {
    float newValor = (int)valor;
    float newEscala = (int)escala;
    float resultado = (newValor * newEscala )/ 32768.00;
    return resultado;
}

double gravidadeParaAceleracao(float valor){
  return valor * 9.8;
}

// Fim das Funções de Queda





void loop()
{

  // PARTE DA QUEDA

  Wire.beginTransmission(MPU_addr); //Começa a transmissao de dados para o sensor
  Wire.write(0x3B); // registrador dos dados medidos (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); // faz um "pedido" para ler 14 registradores, que serão os registrados com os dados medidos
  AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  AcnX = converteGravidade(AcX, 4); // Valor de X em g
  AcnY = converteGravidade(AcY, 4); // Valor de Y em g
  AcnZ = converteGravidade(AcZ, 4); // Valor de Z em g

  gyLateral     = converteGraus(GyX, 1000);
  gyVertical    = converteGraus(GyY, 1000);
  gyHorizontal = converteGraus(GyZ, 1000);

  AcLateral = gravidadeParaAceleracao(AcnX); // Valor de X em m/s²
  AcVertical = gravidadeParaAceleracao(AcnY); // Valor de Y em m/s²
  AcHorizontal = gravidadeParaAceleracao(AcnZ); // Valor de Z em m/s²

  
  // QUEDA PARA FRENTE
  if ((AcHorizontal <= 3) && (AcVertical <= -1) && (gyLatWire.write(0x3B); // registrador dos dados medidos (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wireral <= -500)){
    queda = true;
    strcpy(buff, "Queda para frente detectada");
  }

  // QUEDA PARA TRÁS
   if ((AcHorizontal <= 0) && (AcVertical <= -1) && (gyLateral >= 500)){
    queda = true;
    strcpy(buff, "Queda para trás detectada");
  }
  
  // FIM DA PARTE DA QUEDA

  if (queda){
  
    TXPacketL = sizeof(buff);                                    //set TXPacketL to length of array
    buff[TXPacketL - 1] = '*';                                   //replace null character at buffer end so its visible on receiver

    LT.printASCIIPacket(buff, TXPacketL);                           //print the buffer (the sent packet) as ASCII

    if (LT.transmitIRQ(buff, TXPacketL, 10000, TXpower, WAIT_TX))   //will return packet length sent if OK, otherwise 0 if transmit error
    {
      TXPacketCount++;
      packet_is_OK();
      queda = false;
    }
    else
    {
      packet_is_Error();                                         //transmit packet returned 0, there was an error
      queda = false;
    }

    Serial.println();
    delay(1000);                                                 //have a delay between packets
  }
}


void packet_is_OK()
{
  Serial.println("dados enviados com sucesso");                   //print total of packets sent OK
}


void packet_is_Error()
{
  //if here there was an error transmitting packet
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                      //read the the interrupt register
  Serial.print(F(" SendError,"));
  Serial.print(F("Length,"));
  Serial.print(TXPacketL);                             //print transmitted packet length
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);                        //print IRQ status
  LT.printIrqStatus();                                 //prints the text of which IRQs set
}


void setup()
{
  // Parte da Queda

  Wire.begin(); //Inicia a comunicação I2C
  Wire.beginTransmission(MPU_addr); //Começa a transmissao de dados para o sensor
  Wire.write(0x6B); // registrador PWR_MGMT_1
  Wire.write(0); // Manda 0 e "acorda" o MPU 6050
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C); // Endereço do registrador de configuração do acelerômetro
  Wire.write(0x08); // Configurar para ±4g (veja o datasheet para outras opções)
  Wire.endTransmission(true);

  // Fim da parte da Queda


  Serial.begin(9600);
  Serial.println();
  Serial.println(F("LaSiD - UFBA - Detector de quedas"));

  SPI.begin();

  if (LT.begin(NSS, NRESET, RFBUSY, LORA_DEVICE))
  {
    Serial.println(F("Dispositivo LoRa Conectado"));
    delay(1000);
  }
  else
  {
    Serial.println(F("No LoRa device responding"));
    while (1);
  }

  LT.setupLoRa(2445000000, 0, LORA_SF7, LORA_BW_0400, LORA_CR_4_5);      //configure frequency and LoRa settings

  Serial.print(F("Transmissor Pronto"));
  Serial.println();
}