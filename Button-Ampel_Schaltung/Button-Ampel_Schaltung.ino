int state = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(9, INPUT);

  pinMode(4, OUTPUT); //GRÜN
  pinMode(3, OUTPUT); //GELB
  pinMode(2, OUTPUT); //ROT
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(5) == HIGH){
    state = state + 1;
    delay(200);
    Serial.println(state);
  } 
  if(state > 3){
    state = 0;
    Serial.println(state);
  }

  switch(state)
  {
  case 0:
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    //ist ROT
    break;

  case 1:
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
    //ist GELB
    break;

  case 2:
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    //ist GRÜN
    break;
  }

    case 3:
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    //ist
    break;
  }

  default:
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
  
    break;
}
