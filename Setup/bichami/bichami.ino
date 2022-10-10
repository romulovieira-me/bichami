/* Bichami 
Capturando movimentos na Bichami com acelerômetro e sensor capacitivo de toque, que envia os dados via rede para o Pure Data
Código por: Rômulo Vieira
*/

// Declarando bibliotecas 
#include<Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// Declarando constantes 
#define touch 14 // Declarando pino que irá receber sensor de toque (pino D5 no ESP8266)
const int MPU=0x68; // Endereço na placa NOdeMCU v3 para o acelorômetro MPU6050
const IPAddress outIp(255,255,255,255); // IP do computador que irá receber o pacote OSC 
const unsigned int outPort = 9999; // Porta remota para receber dados OSC
const unsigned int outPort2 = 7777; // Porta 2 remota para enviar dados OSC
const unsigned int localPort = 2390; // Porta local para ouvir pacotes OSC

// Declarando variáveis
char ssid[] = "Romulo"; // EDITAR: Nome da rede
char pass[] = "romulo182"; // EDITAR: senha da rede
WiFiUDP Udp; // Instância que permite enviar e receber pacotes usando o pacote UDP
int GyX, GyY, GyZ; // Variáveis inteiras que captam mudanças no eixo X, Y e Z do giroscópio
int minVal=0; // Valor mínimo do MPU6050
int maxVal=65536;

void setup() {
  // Declarando baud rate
  Serial.begin(9600); 

  // Conectando a rede WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
  // Declarando pino para receber toque como sinal de entrada
  pinMode(touch, INPUT);

  // Configurando acelerômetro MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU); // Começando transmissão com endereço do MPU
  Wire.write(0X6B);

  // Iniciando leitura no MPU6050
  Wire.write(0);
  Wire.endTransmission(true);
  
}

// Declarando variável com estado do sensor de toque
int value = 0;

void loop() {
  
  // Iniciando lógica do sensor de toque capacitivo
  value = digitalRead(touch); // Lendo o sensor de toque
  Serial.println(value); // Imprimindo valor do sensor de toque no monitor serial
  delay(50); // Leitura realizada a cada 50 ms

  // Iniciando lógica do MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0X3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true); // Pegando dados do MPU6050 (14 bytes consecutivos)
  // Lendo dados do giroscópio
  GyX=Wire.read()<<8|Wire.read();  //0x3B (GYRO_XOUT_H) & 0x3C (GYRO_XOUT_L)     
  GyY=Wire.read()<<8|Wire.read();  //0x3D (GYRO_YOUT_H) & 0x3E (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x3F (GYRO_ZOUT_H) & 0x40 (GYRO_ZOUT_L)

  //Adaptando range do giroscópio de -180º até 180º
  int xAng = map(GyX,minVal,maxVal,0,180);
  int yAng = map(GyY,minVal,maxVal,0,180);
  int zAng = map(GyZ,minVal,maxVal,0,180);

   // Enviando eixo X do giroscópio para o monitor serial
  Serial.print(" | GyX = "); Serial.print(xAng);
  // Enviando eixo Y do giroscópio para o monitor serial
  Serial.print(" | GyY = "); Serial.print(yAng);
  // Enviando eixo Z do giroscópio para o monitor serial
  Serial.print(" | GyZ = "); Serial.println(zAng);
  // Estabelecendo delay nestas informações
  delay(300);

 // Enviando mensagem OSC do sensor de toque
  OSCMessage botao("/value");
  botao.add((int32_t)int(value));
  Udp.beginPacket(outIp, outPort);
  botao.send(Udp);
  Udp.endPacket();
  botao.empty();
  delay(500);

  // Enviando mensagem do eixo X do acelerômetro para o Pure Data
  OSCMessage axleX("/gyx");
  axleX.add((int32_t)int(xAng));
  Udp.beginPacket(outIp, outPort);
  axleX.send(Udp);
  Udp.endPacket();
  axleX.empty();
  delay(10);

  // Enviando mensagem do eixo Y do acelerômetro para o Processing
  OSCMessage processingaxleX("/gyx");
  processingaxleX.add((int32_t)int(xAng));
  Udp.beginPacket(outIp, outPort2);
  processingaxleX.send(Udp);
  Udp.endPacket();
  processingaxleX.empty();
  delay(10);

  // Enviando mensagem do eixo Y do acelerômetro para o Pure Data
  OSCMessage axleY("/gyy");
  axleY.add((int32_t)int(yAng));
  Udp.beginPacket(outIp, outPort);
  axleY.send(Udp);
  Udp.endPacket();
  axleY.empty();
  delay(10);

  // Enviando mensagem do eixo Y do acelerômetro para o Processing
  OSCMessage processingaxleY("/gyy");
  processingaxleY.add((int32_t)int(yAng));
  Udp.beginPacket(outIp, outPort2);
  processingaxleY.send(Udp);
  Udp.endPacket();
  processingaxleY.empty();
  delay(10);

  // Enviando mensagem do eixo Z do acelerômetro
  OSCMessage axleZ("/gyz");
  axleZ.add((int32_t)int(zAng));
  Udp.beginPacket(outIp, outPort);
  axleZ.send(Udp);
  Udp.endPacket();
  axleZ.empty();
  delay(100);

}
