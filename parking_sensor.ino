#define Buzzer  3
#define Start   A0
#define End     A1
#define trigPin 2
#define echoPin 4

//----Left 7-Segment pins
#define a1 9
#define b1 12
#define c1 11
#define d1 10

//---Right 7-Segment pins
#define a2 5
#define b2 8
#define c2 7
#define d2 6

//Variables
bool state, Buzzer_level;
uint8_t cm, Buzzer_value[2]={0, 120};
uint16_t Delay_time, Duration;
unsigned long timer1, timer2, ultrasonic_read_timer1, ultrasonic_read_timer2;

//Functions
void ultra_sonic();
void segleft(uint8_t number);
void segright(uint8_t number);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(Start, INPUT_PULLUP);
  pinMode(End, INPUT_PULLUP);
}

void loop() {

  segright('-');  //for turinng off 7-segments
  segleft('-');

  if (digitalRead(Start) == 0)
    state = 1;

  while (state) {
    
    //Measuring Distance Value
    ultrasonic_read_timer1 = millis();
    if (ultrasonic_read_timer1 - ultrasonic_read_timer2 >= 300) {
      ultra_sonic();
      ultrasonic_read_timer2 = ultrasonic_read_timer1;
    }

    //Toggling Buzzer
    timer1 = millis();
    if (timer1 - timer2 >= Delay_time and cm > 4) {
      Buzzer_level = !Buzzer_level;
      analogWrite(Buzzer, Buzzer_value[Buzzer_level]);
      timer2 = timer1;
    }

    //displaying Measured Value on 7-segments
    segleft(cm / 10);
    segright(cm % 10);

    //Setting Buzzer Frequency
    if (cm <= 4) {
      analogWrite(Buzzer, Buzzer_value[1]);
    }
    else if (cm > 4  and cm <= 8 ) {Delay_time = 100;}
    else if (cm > 8  and cm <= 12) {Delay_time = 200;}
    else if (cm > 12 and cm <= 18) {Delay_time = 350;}
    else if (cm > 18 and cm <= 25) {Delay_time = 550;}
    else {Delay_time = 800;}

    //Reading value of End button to stop measuring and turn off the Buzzer
    if (digitalRead(End) == 0)
    {
      state = 0;
      segleft(0);
      segright(0);
      digitalWrite(Buzzer, LOW);
    }
  }
}


//Ultra Sonic Driver
void ultra_sonic() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Duration = pulseIn(echoPin, HIGH);
  cm = (Duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
}

//------Left 7-Segment display
void segleft(uint8_t number) {
  if (number != '-') {
    digitalWrite(a1, (number >> 0) & 0x01);
    digitalWrite(b1, (number >> 1) & 0x01);
    digitalWrite(c1, (number >> 2) & 0x01);
    digitalWrite(d1, (number >> 3) & 0x01);
  }
  else {
    digitalWrite(a1, HIGH);
    digitalWrite(b1, HIGH);
    digitalWrite(c1, HIGH);
    digitalWrite(d1, HIGH);
  }
}

//------Right 7-Segment display
void segright(uint8_t number) {
  if (number != '-') {
    digitalWrite(a2, (number >> 0) & 0x01);
    digitalWrite(b2, (number >> 1) & 0x01);
    digitalWrite(c2, (number >> 2) & 0x01);
    digitalWrite(d2, (number >> 3) & 0x01);
  }
  else {
    digitalWrite(a2, HIGH);
    digitalWrite(b2, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(d2, HIGH);
  }
}
