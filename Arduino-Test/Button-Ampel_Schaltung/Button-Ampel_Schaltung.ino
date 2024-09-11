int state = 0;
int value;
const int potPin = A0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

//  pinMode(potPin, INPUT);
  pinMode(13, INPUT);

  pinMode(2, OUTPUT); //e
  pinMode(3, OUTPUT); //d
  pinMode(4, OUTPUT); //c
  pinMode(5, OUTPUT); //DP
  pinMode(6, OUTPUT); //g
  pinMode(7, OUTPUT); //f
  pinMode(8, OUTPUT); //a
  pinMode(9, OUTPUT); //b

}

void loop() {
// put your main code here, to run repeatedly:

//  value = analogRead(potPin);          //Read and save analog value from potentiometer
//  value = map(value, 0, 1023, 0, 255); //Map value 0-1023 to 0-255 (PWM)
//  analogWrite(ledPin, value);          //Send PWM value to led
//  delay(100);
//  Serial.println(value)

  if(digitalRead(13) == HIGH){
    state = state + 1;
    delay(200);
    Serial.println(state);
  } 
  if(state > 10){
    state = 0;
    Serial.println(state);
  }

  switch(state)
  {
  case 0:
    digitalWrite(2, HIGH); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, HIGH); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //ist All on
    break;

  case 1:
    digitalWrite(2, LOW); //e
    digitalWrite(3, LOW); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, LOW); //g
    digitalWrite(7, LOW); //f
    digitalWrite(8, LOW); //a
    digitalWrite(9, HIGH); //b
    //1
    break;

  case 2:
    digitalWrite(2, HIGH); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, LOW); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, LOW); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //2
    break;

  case 3:
    digitalWrite(2, LOW); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, LOW); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //3
    break;

  case 4:
    digitalWrite(2, LOW); //e
    digitalWrite(3, LOW); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, LOW); //a
    digitalWrite(9, HIGH); //b
    //4
    break;

  case 5:
    digitalWrite(2, LOW); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, LOW); //b
    //5
    break;

  case 6:
    digitalWrite(2, HIGH); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, LOW); //a
    digitalWrite(9, LOW); //b
    //6
    break;

  case 7:
    digitalWrite(2, LOW); //e
    digitalWrite(3, LOW); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, LOW); //g
    digitalWrite(7, LOW); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //7
    break;

  case 8:
    digitalWrite(2, HIGH); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //8
    break;

  case 9:
    digitalWrite(2, LOW); //e
    digitalWrite(3, LOW); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, HIGH); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //9
    break;

  case 10:
    digitalWrite(2, HIGH); //e
    digitalWrite(3, HIGH); //d
    digitalWrite(4, HIGH); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, LOW); //g
    digitalWrite(7, HIGH); //f
    digitalWrite(8, HIGH); //a
    digitalWrite(9, HIGH); //b
    //0
    break;

  default:
    digitalWrite(2, LOW); //e
    digitalWrite(3, LOW); //d
    digitalWrite(4, LOW); //c
    digitalWrite(5, LOW); //DP
    digitalWrite(6, LOW); //g
    digitalWrite(7, LOW); //f
    digitalWrite(8, LOW); //a
    digitalWrite(9, LOW); //b
    //ist All off
    break;
  }
}
