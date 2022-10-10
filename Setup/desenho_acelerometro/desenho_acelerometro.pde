// Importando bibliotecas
import oscP5.*;
import netP5.*;

OscP5 oscP5;
int x, y, z;

// Definindo as propriedades da tela 
void setup() {
  size(400, 400);
  background(255);
  oscP5 = new OscP5(this, 7777);
}

// Recebendo dados OSC
void oscEvent(OscMessage theOscMessage) {
  
  if (theOscMessage.addrPattern().equals("/gyx")) {
    x = theOscMessage.get(0).intValue();
  }
  
  if (theOscMessage.addrPattern().equals("/gyy")) {
    y = theOscMessage.get(0).intValue();
  }

  //OscMessage m = new OscMessage("/gyx");
  //m.add(1);

}

// Definindo a lógica do ambiente 
void draw() {
  stroke(0);
  line(120, 80, x, y);
  strokeWeight(5);
  
  stroke(255, 0, 0);
  line(340, 80, x, y);

   
}  
  
