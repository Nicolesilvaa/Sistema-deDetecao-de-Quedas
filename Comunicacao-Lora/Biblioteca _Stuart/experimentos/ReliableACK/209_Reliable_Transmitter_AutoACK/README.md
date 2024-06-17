/*******************************************************************************************************
  Programs for Arduino - Copyright of the author Stuart Robinson - 05/11/21

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

/*******************************************************************************************************
  Program Operation - This is a basic demonstration of the transmission and acknowledgement of a 'Reliable'
  packet.

  A reliable packet has 4 bytes automatically appended to the end of the buffer\array that is the data
  payload. The first two bytes appended are a 16bit 'NetworkID'. The receiver needs to have the same
  NetworkID as configured for the transmitter since the receiver program uses the NetworkID to check that
  the received packet is from a known source.  The third and fourth bytes appended are a 16 bit CRC of
  the payload. The receiver will carry out its own CRC check on the received payload and can then verify
  this against the CRC appended in the packet. The receiver is thus able to check if the payload is valid.

  For a packet to be accepted by the receiver, the networkID and payload CRC appended to the packet by the
  transmitter need to match those from the receiver which gives a high level of assurance that the packet
  is valid.

  If the received packet is valid then the networkID and payload CRC are returned in a 4 byte packet as an
  acknowledgement that the transmitter listens for. If the transmitter does not receive the acknowledgement
  within the ACKtimeout period, the original packet is re-transmitted until a valid acknowledgement is
  received. This program should be used with the matching receiver program, 210_Reliable_Receiver_AutoACK.

  The program will attempt to transmit the packet and have it acknowledged by the receiver a number of times
  as defined by constant TXattempts. If there is no acknowledge withing this time it will be reported.

  It is possible to use the 'NetworkID' to direct the packet to specific receivers.

  Serial monitor baud rate should be set at 115200.

  *******************************************************************************************************/
