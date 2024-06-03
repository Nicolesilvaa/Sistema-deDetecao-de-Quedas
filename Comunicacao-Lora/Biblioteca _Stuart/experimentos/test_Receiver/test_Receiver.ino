

    #include <SPI.h>                      //the SX128X device is SPI based so load the SPI library                                         
    #include <SX128XLT.h> 

    SX128XLT LT; 

    #define Program_Version "Teste_NicoleSilva"

    #define NSS 10
    #define RFBUSY 8
    #define NRESET 9
    #define LED1 7
    #define DIO1 3
    #define RX_EN -1                    //pin for RX enable, used on some SX1280 devices, set to -1 if not used
    #define TX_EN -1                    //pin for TX enable, used on some SX1280 devices, set to -1 if not used  
    #define BUZZER -1                   //pin for BUZZER, set to -1 if not used 
    #define LORA_DEVICE DEVICE_SX1280   //we need to define the device we are using  
    #define RXBUFFER_SIZE 255           //RX buffer size  


    //LoRa Modem Parameters and variables ***************************************************************************************************************************

    uint8_t SpreadingFactor = 0xC0;             //LoRa spreading factor values hexadecimal {LORA_SF5 = 0x50,LORA_SF6 = 0x60,LORA_SF7 = 0x70,LORA_SF8 = 0x80,LORA_SF9 = 0x90,LORA_SF10 = 0xA0, LORA_SF11 = 0xB0,LORA_SF12 = 0xC0}
    int8_t valuesTxpower[] = {-15, -12, -9, -6, -3, 1, 4, 7, 10, 13};
    int8_t TXpower = valuesTxpower[5];          //Power for transmissions in dBm

    const uint32_t Frequency = 2445000000;     //frequency of transmissions
    const int32_t Offset = 0;                  //offset frequency for calibration purposes
    const uint8_t Bandwidth = LORA_BW_0200;    //LoRa bandwidth
    const uint8_t CodeRate = LORA_CR_4_5;     
    const uint16_t packet_delay = 1000;       //mS delay between packets
    const uint8_t size = 200;                 //quantity of samples to carry out experiments

    uint32_t RXpacketCount,countPacket, errors, startmS, endmS;

    uint8_t RXBUFFER[RXBUFFER_SIZE];           //create the buffer that received packets are copied into
    uint8_t RXPacketL;                         //stores length of packet received
    int16_t PacketRSSI;                        //stores RSSI of received packet
    int8_t  PacketSNR;                         //stores signal to noise ratio of received packe
    //****************************************************************************************************************************************************************


    void setup(){

      pinMode(LED1, OUTPUT);                        //setup pin as output for indicator LED
      led_Flash(2, 125);                            //two quick LED flashes to indicate program start

      Serial.begin(9600);
      Serial.println();
      Serial.print(F(__TIME__));
      Serial.print(F(" "));
      Serial.println(F(__DATE__));
      Serial.println(F(Program_Version));
      Serial.println();
      Serial.println(F("Iniciando comunicação - Receptor"));
      Serial.println(F("Lasid UFBA"));
      Serial.println();

      if (BUZZER > 0){

        pinMode(BUZZER, OUTPUT);
        digitalWrite(BUZZER, HIGH);
        delay(50);
        digitalWrite(BUZZER, LOW);
      }

      SPI.begin();
      //SPI beginTranscation is normally part of library routines, but if it is disabled in the library
      //a single instance is needed here, so uncomment the program line below
      //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

      //setup hardware pins used by device, then check if device is found
      if (LT.begin(NSS,NRESET, RFBUSY,DIO1, RX_EN, TX_EN, LORA_DEVICE)){

        Serial.println(F("LoRa Device found"));
        //led_Flash(2, 1);
        delay(20);

      }else{

        Serial.println(F("No device responding"));
        while (1){

          led_Flash(50, 50);                       //long fast speed LED flash indicates device error
        }
      }

      
  
    //Setup LoRa device **********************************************************************************************************************************************
    LT.setMode(MODE_STDBY_RC);
    LT.setRegulatorMode(USE_LDO);
    LT.setPacketType(PACKET_TYPE_LORA);
    LT.setRfFrequency(Frequency, Offset);
    LT.setBufferBaseAddress(0, 0);
    LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate);
    LT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL, 0, 0);
    LT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);
    LT.setHighSensitivity();
    //****************************************************************************************************************************************************************

    Serial.print(F("Dados Recebdidos - RXBUFFER_SIZE "));
    Serial.println(RXBUFFER_SIZE);
    Serial.println();

   }

    void loop(){

      startmS =  millis(); 
      RXPacketL = LT.receive(RXBUFFER, RXBUFFER_SIZE, 60000, WAIT_RX); //wait for a packet to arrive with 60seconds (60000mS) timeout

      digitalWrite(LED1, HIGH);                         //something has happened

      if(BUZZER > 0){digitalWrite(BUZZER, HIGH);}    //turn buzzer on
        
      PacketRSSI = LT.readPacketRSSI();              //read the recived RSSI value
      PacketSNR = LT.readPacketSNR();                //read the received SNR value

      if(RXPacketL == 0){                           //if the LT.receive() function detects an error, RXpacketL == 0
        
        packet_is_Error();

      }else{
        
        endmS = millis(); 
        packet_is_OK();  
      }

      if(BUZZER > 0){digitalWrite(BUZZER, LOW);}   
      
      countPacket = RXpacketCount + errors;       // Count of all packages sent, including errors.
      medianTimeRX(receiverTime,countPacket);
      standardDeviationRSSI_SNR(countPacket,PacketRSSI,PacketSNR);


      //varying parameters

      digitalWrite(LED1, LOW);                                      
      Serial.println();  
    }

    void packet_is_OK(){

      int16_t IRQStatus, localCRC;
      uint32_t receiverTime;

      receiverTime = endmS - startmS;

      IRQStatus = LT.readIrqStatus();                  //read the LoRa device IRQ status register
      RXpacketCount++;

      Serial.print(F("TXpower,"));
      Serial.print(TXpower);
      Serial.print(F(",Spreading Factor,"));
      Serial.print(SpreadingFactor, HEX);
      Serial.print(F(",RSSI,"));
      Serial.print(PacketRSSI);
      Serial.print(F("dBm,SNR,"));
      Serial.print(PacketSNR);
      Serial.print(F("dB,Length,"));
      Serial.print(RXPacketL);
      Serial.print(F(",Packets,"));
      Serial.print(RXpacketCount);
      Serial.print(F(",Errors,"));
      Serial.print(errors);
      Serial.print(F(",IRQreg,"));   
      Serial.print(IRQStatus, HEX);
      Serial.print(F("  Receiver Time,"));
      Serial.print(receiverTime); 
      Serial.print(F("mS"));       

      led_Flash(2, 20);      
    }

    void packet_is_Error(){

      uint16_t IRQStatus;
      IRQStatus = LT.readIrqStatus();                   //read the LoRa device IRQ status register
      printElapsedTime();                               //print elapsed time to Serial Monitor

      if(IRQStatus & IRQ_RX_TIMEOUT){                 //check for an RX timeout
        Serial.print(F(" RXTimeout"));

      }else{
        
        errors++;
        Serial.print(F(" PacketError"));
        Serial.print(F(",RSSI,"));
        Serial.print(PacketRSSI);
        Serial.print(F("dBm,SNR,"));
        Serial.print(PacketSNR);
        Serial.print(F("dB,Length,"));
        Serial.print(LT.readRXPacketL());               //get the real packet length
        Serial.print(F(",Packets,"));
        Serial.print(RXpacketCount);
        Serial.print(F(",Errors,"));
        Serial.print(errors);
        Serial.print(F(",IRQreg,"));
        Serial.print(IRQStatus, HEX);
        LT.printIrqStatus();                            //print the names of the IRQ registers set
      }

      delay(250);                                       //gives a longer buzzer and LED flash for error

    }


    void standardDeviationRSSI_SNR(uint32_t &countPacket, int16_t  &PacketRSSI, int8_t &PacketSNR){ //calculates sample standard deviation SNR and RSSI

        // Declaring variables

        int16_t sumRSSI, mRSSI, varianceSSI, sdRSSI;
        int8_t  sumSNR, mSNR, varianceSNR, sdSNR;
       
        int16_t st_packetRSSI[size];
        int8_t st_packetSNR[size];

        // Storing data
        while(countPacket <= size){

            st_packetRSSI[size] = PacketRSSI;
            st_packetSNR[size] = PacketSNR;
            
            sumRSSI += PacketRSSI;
            sumSNR += PacketSNR;
        }

        // Calculating variance
        if(countPacket == size){
            
            // Calculating media 
            mRSSI = sumRSSI/countPacket;
            mSNR = sumSNR/countPacket;
            
            for(int i = 0; i < size; i++){

                varianceRSSI += (st_packetRSSI[i] - mRSSI)^2;
                varianceSNR += (st_packetSNR[i] - mSNR)^2;
            }
            
            // Calculating sample standard deviation
            sdRSSI = sqrt(varianceRSSI / (size - 1));
            sdSNR = sqrt(varianceSNR / (size - 1));
            
            //Output
            Serial.print("Standard deviation RSSI: ");
            Serial.print(sdRSSI);
            Serial.print("Standard deviation SNR: ")
            Serial.print(sdSNR);
            Serial.println();
        }
    }


    void medianTimeRX(uint32_t &receiverTime, uint32_t &countPacket){

     uint32_t sumTime;
     sumTime += receiverTime;

      if(countPacket == size){
      
        uint32_t  median = sumTime/countPacket;

        Serial.print(" Median time de reception = ");
        Serial.print(median); 
        Serial.print("ms");
        Serial.println();
      }
    }

    void printElapsedTime(){ 

      float seconds;
      seconds = millis() / 1000;
      Serial.print(seconds, 0);
      Serial.print(F("s"));
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

