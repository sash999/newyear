
int pins[] = {6,9,10,11};
int ButtonPin = 4;
int ButtonState = 0;
int led_up;
int led_down;
volatile int programm = 0;
volatile int programm_max = 6;
volatile bool stop_now = false;

#define STEP 5

void setup() {
  Serial.begin(9600);
  for(int i=0; i<=3; i++) {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(ButtonPin, INPUT);

  // Устанавливаем прерывание
  // Функция changeProgramm вызовется тогда, когда
  // на 2 пине (прерываниие 0 связано с пином 2)
  // изменится сигнал с логического 0 на 1
  attachInterrupt(0, changeProgramm, RISING); 

} 

void changeProgramm() {
 static unsigned long millis_prev;
      if(millis()-100 > millis_prev) {
        stop_now = true;
        programm++ ;
        if(programm > programm_max) { programm = 0 ; }
        Serial.println(programm, millis_prev);
        millis_prev = millis();
     }
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

bool StopNow() {
  /* срочное завершение текущей программы
  проверкяем стоп-флаг - если выставлен,
  то всё погасить, флаг сбросить и вернуть true */
  if(stop_now) {
    stop_now = false;
    LightOffAll();
    return true;
  }
  return false;
  }


void Programm0() { 
/* программа 0 - плавные бегущие огни */
for(int j =0; j<=3; j++) { 
if(StopNow()) return; 
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
}

void Programm1() {
  /* программа 1 - все по очереди плавно зажечь, 
   все по очереди плавно погасить,
   задержка 3 секунды
*/
for(int j=0; j<=3; j++) {
 if(StopNow()) return; 
 LightUp(pins[j], STEP);
}

for(int j=0; j<=3; j++) {
 if(StopNow()) return; 
 LightDown(pins[j], STEP);
}

delay(3000);
}

void Programm2() {
  /* программа 2 - бегущие огни  */
for(int j=0; j<=3; j++) {
  if(StopNow()) return; 
  LightOn(pins[j]);
  delay(300);
  LightOffAll();
}
}

void OneColor(int line) {
  if(StopNow()) return; 
  LightOn(pins[line]);
}
 

void loop() {
    switch(programm) {
        case 0:
            Programm0();
            break;
        case 1:
            Programm1();
            break;
        case 2:
            Programm2();
            break;
        case 3:
            OneColor(0);
            break;    
        case 4:
            OneColor(1);
            break;    
        case 5:
            OneColor(2);
            break;    
        case 6:
            OneColor(3);
            break;    
        default:
            LightOffAll();
            break;
    }
}
