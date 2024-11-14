#ifndef DETECTOR_DE_QUEDA_H
#define DETECTOR_DE_QUEDA_H

#include <Wire.h>
#include <string.h>

// Endereço do sensor MPU6050
const int MPU_addr = 0x68;

// Variáveis para valores lidos do sensor
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Variáveis de aceleração e giroscópio
float AcnX, AcnY, AcnZ;
float AcVertical, AcLateral, AcHorizontal;
float gyVertical, gyLateral, gyHorizontal;

// Variável de detecção de queda
bool queda = false;

// Funções auxiliares para conversões
float converteGraus(int16_t valor, int16_t escala);
float converteGravidade(int16_t valor, int16_t escala);
double gravidadeParaAceleracao(float valor);

// Inicializa o sensor MPU6050
void inicializaMPU() {
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1C);
    Wire.write(0x08);  // Configuração para ±4g
    Wire.endTransmission(true);
}

// Função de detecção de queda
bool detectarQueda(char* buff) {
    // Ler dados do sensor
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true);
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
    Tmp = Wire.read() << 8 | Wire.read();
    GyX = Wire.read() << 8 | Wire.read();
    GyY = Wire.read() << 8 | Wire.read();
    GyZ = Wire.read() << 8 | Wire.read();

    AcnX = converteGravidade(AcX, 4);
    AcnY = converteGravidade(AcY, 4);
    AcnZ = converteGravidade(AcZ, 4);

    gyLateral = converteGraus(GyX, 1000);
    gyVertical = converteGraus(GyY, 1000);
    gyHorizontal = converteGraus(GyZ, 1000);

    AcLateral = gravidadeParaAceleracao(AcnX);
    AcVertical = gravidadeParaAceleracao(AcnY);
    AcHorizontal = gravidadeParaAceleracao(AcnZ);

    // Verifica as condições de queda para frente e para trás
    if ((AcHorizontal <= 3) && (AcVertical <= -1) && (gyLateral <= -500)) {
        queda = true;
        strcpy(buff, "Queda para frente detectada");
    } else if ((AcHorizontal <= 0) && (AcVertical <= -1) && (gyLateral >= 500)) {
        queda = true;
        strcpy(buff, "Queda para trás detectada");
    } else {
        queda = false;
    }

    return queda;
}

// Implementação das funções auxiliares
float converteGraus(int16_t valor, int16_t escala) {
    return (float)(valor * escala) / 32768.0;
}

float converteGravidade(int16_t valor, int16_t escala) {
    return (float)(valor * escala) / 32768.0;
}

double gravidadeParaAceleracao(float valor) {
    return valor * 9.8;
}

#endif
