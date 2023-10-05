// Transmissor

#include <SPI.h>
#include <Lora.h>


// Definição de pinos
#define pinRX 7
#define pintTX 8

// Definições gerais
#define alerta "Detectamos uma queda"
#define confirmacao "Alerta gerado"


void setup(){

	Serial.begin(9600)

    // Testando se a comunicação está sendo feita
	while(!Serial);
	Serial.println("Transmissor LoRa");

	if(!LoRa.begin(433E6)){  //Frequencia de Operacao (ou 915E6)
		Serial.println("Falha ao Iniciar o Lora!");
		while(1)
	
	}
}

void loop(){

	LoRa.beginPacket(); // Inicia transmissão do pacote

	if(){  //Condição que verifica se a queda ocorreu
 
		LoRa.print(confirmacao);
		Serial.print("Enviando") ;
		
	}

	if(LoRa.available()){
		
		string recebido = LoRa.read();
		Serial.print(recebido);

		if(recebido == confirmacao){
			LoRa.endPacket();
		}
	}

}