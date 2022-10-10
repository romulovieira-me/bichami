#define touch 14 // pino D5

void setup () {
  pinMode(touch, INPUT);
  Serial.begin(9600);
  Serial.println("Serial Started");
}

int value = 0;

void loop ()
{
  value = digitalRead(touch);
  Serial.println(value);
  delay(50);
}
