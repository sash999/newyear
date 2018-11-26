
int pins[] = {6,9,10,11};
int ButtonPin = 4;
int ButtonState = 0;
int led_up;
int led_down;
int programm = 0;
int programm_max = 2;

#define STEP 5

void setup() {
  for(int i=0; i<=3; i++) {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(ButtonPin, INPUT);
} 

void LightUp(int pin, int step) {
  for(int i=0; i<=255; i++)
    {
    analogWrite(pin,i);
    delay(step);
    }
}

void LightDown(int pin, int step) {
  for(int i=255; i>=0; i--)
    {
    analogWrite(pin,i);
    delay(step);
    }
}    

void LightUpDown(int pin_up, int pin_down, int step) {
  for(int i=0; i<=255; i++) {
  analogWrite(pin_up,i);
  analogWrite(pin_down, 255-i);
  delay(step);
}
}

void LightOff(int pin) {
  digitalWrite(pin,0);
}

void LightOffAll() {
  for(int j=0; j<=3; j++) {
  LightOff(pins[j]);
 }
}

void LightOn(int pin) {
  digitalWrite(pin,1);
}

void LightOnAll() {
  for(int j=0; j<=3; j++) {
  LightOn(pins[j]);
 }
}

void loop() {

ButtonState = digitalRead(ButtonPin);
if(ButtonState == HIGH) {
 for(int j=0; j<5; j++) {
  LightOnAll();
  delay(20);
  LightOffAll();
  delay(20); 
 }
  
 programm++ ;
 if(programm > programm_max) { programm = 0 ; }
 delay(2000);
}

switch(programm) {
case 0:
for(int j =0; j<=3; j++) { 
 
 if(j == 0) {
  led_up = pins[0];
  led_down = pins[3];
  }
 else {
  led_up = pins[j];
  led_down = pins[j-1];
 }
  LightUpDown(led_up, led_down, STEP);
 
}
break;
case 1:
/* программа 1 - все по очереди плавно зажечь, 
   все по очереди плавно погасить,
   задержка 3 секунды
*/
for(int j=0; j<=3; j++) {
 LightUp(pins[j], STEP);
}

for(int j=0; j<=3; j++) {
 LightDown(pins[j], STEP);
}

delay(3000);

/* конец программы 1 */
break;
case 2:
/* программа 2 - бегущие огни  */
for(int j=0; j<=3; j++) {
  LightOn(pins[j]);
  delay(300);
  LightOffAll();
}

/* конец программы 2 */
break;
}
}
