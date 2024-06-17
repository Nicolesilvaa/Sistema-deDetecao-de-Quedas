
***

  Programs for Arduino - Copyright of the author Stuart Robinson - 06/02/20

  This program is supplied as is, it is up to the user of the program to decide if the program is

***
  
  Program Operation - This is a simple LoRa test transmitter. A packet containing ASCII text is sent  according to the frequency and LoRa settings specified in the 'Settings.h' file. The pins to access
  the lora device need to be defined in the 'Settings.h' file also.

  The details of the packet sent and any errors are shown on the Serial Monitor, together with the transmit
  power used, the packet length and the CRC of the packet. The matching receive program, '4_LoRa_Receive'
  can be used to check the packets are being sent correctly, the frequency and LoRa settings (in Settings.h)
  must be the same for the Transmit and Receive program. Sample Serial Monitor output;

  10dBm Packet> {packet contents*}  BytesSent,23  CRC,DAAB  TransmitTime,54mS  PacketsSent,1

  Serial monitor baud rate is set at 9600
  
***
