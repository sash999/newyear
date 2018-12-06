

int pins[] = {6,9,10,11};
/*
 * выводы ардуинки с pwm для управления
 * нитями гирлянды.
 * в нашем случае 6 - синий,
 * 9 -желтый
 * 10-зеленый
 * 11 -красный
 */
int led_up;
int led_down;

volatile int programm = 0;
volatile int programm_max = 7;
volatile bool stop_now = false;

#define STEP 5 // задержка в миллиссекундах при повышении/понижении яркости ( полный
               // цикл - 255 итераций, то есть при 5мс - 1.275 сек)

void setup() {
  for(int i=0; i<=3; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // Устанавливаем прерывание
  // Функция changeProgramm вызовется тогда, когда
  // на 2 пине (прерываниие 0 связано с пином 2)
  // изменится сигнал с логического 0 на 1
  attachInterrupt(0, changeProgramm, RISING); 

} 

void changeProgramm() {
 static unsigned long millis_prev;
      if(millis()-200 > millis_prev) { // программная борьба с дребезгом контактов
                                       // проверяем, что с момента прошлого прерывания
                                       // прошло больше 200 мс
        stop_now = true;
        programm++ ;
        if(programm > programm_max) { programm = 0 ; }
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

void ColorSet(bool colorset[4]) {
   if(StopNow()) return;
   if(colorset[0]) LightOn(pins[0]); 
   if(colorset[1]) LightOn(pins[1]); 
   if(colorset[2]) LightOn(pins[2]); 
   if(colorset[3]) LightOn(pins[3]); 
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
        case 7:
            bool colors[] = {0,0,1,1} ;
            ColorSet(colors);
            break;    
      
        default:
            LightOffAll();
            break;
    }
}
