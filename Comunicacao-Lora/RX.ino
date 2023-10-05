// Receptor

#include <SPI.h>
#include <Lora.h>


//Declarando variaveis

String dadoRecebido = ""; // String de leitura



void setup(){

	Serial.begin(9600)

 // Testando se a comunicação está sendo feita
	while(!Serial);
	Serial.println("Receptor LoRa");

	if(!LoRa.begin(433E6)){  //Frequencia de Operacao (ou 915E6)
		Serial.println("Falha ao Iniciar o Lora!");
		while(1)
	}
}

void loop(){
	
	// Tenta receber pacotes de dados
	int packetSize = LoRa.parsePacket();

	if(packetSize){
	    //Lê pacote
		while(LoRa.available()){
			dadoRecebido = LoRa.read();
		}
	}

	
	Serial.print("Alerta Recebido");
	Serial.print(dadoRecebido)
	//Piscar uma led para emitir o alerta
	delay(100);

}