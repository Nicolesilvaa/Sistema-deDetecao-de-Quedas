// Receptor

#include <SPI.h>
#include <LoRa.h>


//Declarando variaveis

String dadoRecebido = ""; // String de leitura

void setup(){

	Serial.begin(9600); // Inicia a blibioteca com a frequência especificada

 // Testando se a comunicação está sendo feita
	while(!Serial);
  Serial.println("Receptor LoRa");

	if(!LoRa.begin(433E6)){  //Frequencia de Operacao (ou 915E6)
    Serial.println("Falha ao Iniciar o Lora!");
    while(1);
	}

	  
}

void loop(){
	

	int packetSize = LoRa.parsePacket(); // Verifica se um pacote foi recebido

	if(packetSize != 0){ // Retorna o tamanho pacote em bytes ou em 0 se nenhum pacote foi recebido
	  //Lê pacote
		while(LoRa.available()){ // 
			dadoRecebido = LoRa.read(); // LoRa.read() - > Retorna o próximo byte do pacote ou -1 se nenhum byte estiver disponível
		}

    		Serial.println("Alerta Recebido");
	  	Serial.println(dadoRecebido);
	  	//Piscar ou tocar um alarme uma led para emitir o alerta
	  	delay(100);

	}

	
}
