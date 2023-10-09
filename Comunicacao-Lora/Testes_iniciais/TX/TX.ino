// Transmissor

#include <SPI.h>
#include <LoRa.h>


// Definição de pinos
#define pinRX 7
#define pintTX 8

// Definições gerais
#define alerta "Detectamos uma queda"
#define confirmacao "Alerta gerado"


void setup(){

	Serial.begin(9600); // Serial.begin() -> Inicia a blibioteca com a frequência especificada

  // Testando se a comunicação está sendo feita
	while(!Serial);

  Serial.println("Transmissor LoRa");

	if(!LoRa.begin(433E6)){  //Frequencia de Operacao (ou 915E6)

		Serial.println("Falha ao Iniciar o Lora!");
		while(1);
	
	}

  
	  
}

void loop(){

	LoRa.beginPacket(); // Inicia transmissão do pacote

	if(true){  //Condição que verifica se a queda ocorreu
 
		LoRa.print(confirmacao);
		Serial.println("Enviando");
		
	}

	if(LoRa.available()){ // LoRa.available -> Retorna o número de bytes disponíveis para leitura.
		
		String recebido = LoRa.read();  // LoRa.read() -> Retorna o próximo byte do pacote ou -1 se nenhum byte estiver disponível
		Serial.println(recebido);

		if(recebido == confirmacao)
			LoRa.endPacket(); // Finaliza o envio do pacote
		
	}

}