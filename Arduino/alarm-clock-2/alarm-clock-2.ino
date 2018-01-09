void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char commandChar = Serial.read();
    switch(commandChar)
    {
      case 'f':
        Serial.print("success");
        break;
    }
  }

  delay(1000);
}
