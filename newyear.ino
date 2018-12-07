

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
volatile int programm_max = 17;
volatile bool stop_now = false;

#define STEP 10 // задержка в миллиссекундах при повышении/понижении яркости ( полный
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

void LightUpAll(int step) {
  for(int i=0; i<=255; i++)
    {
    for(int j=0; j<=3; j++) {  
    analogWrite(pins[j],i);
    }
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

void LightDownAll(int step) {
  for(int i=255; i>=0; i--)
    {
    for(int j=0; j<=3; j++) {  
    analogWrite(pins[j],i);
    }
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

void Programm3() {
  /* программа 3 - все плавно зажечь, 
   все плавно погасить,
   задержка 3 секунды
*/

 if(StopNow()) return; 
 LightUpAll(STEP);
 if(StopNow()) return; 
 LightDownAll(STEP);

delay(3000);
}

void OneColor(int line) {
  if(StopNow()) return; 
  LightOn(pins[line]);
}

void ColorSet(bool blue, bool yellow, bool green, bool red) {
   LightOffAll();
   if(StopNow()) return;
   if(blue)   LightOn(pins[0]); 
   if(yellow) LightOn(pins[1]); 
   if(green)  LightOn(pins[2]); 
   if(red)    LightOn(pins[3]); 
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
            Programm3();
            break;    
        case 4:
            OneColor(0);
            break;    
        case 5:
            OneColor(1);
            break;    
        case 6:
            OneColor(2);
            break;
        case 7:
            OneColor(3);
            break;        
        case 8:
            ColorSet(0,0,1,1);
            break;   
        case 9:
            ColorSet(0,1,0,1);
            break;     
        case 10:
            ColorSet(0,1,1,0);
            break;     
        case 11:
            ColorSet(0,1,1,1);
            break;     
        case 12:
            ColorSet(1,0,0,1);
            break;         
        case 13:
            ColorSet(1,0,1,0);
            break; 
        case 14:
            ColorSet(1,0,1,1);
            break;         
        case 15:
            ColorSet(1,1,0,0);
            break;     
        case 16:
            ColorSet(1,1,1,0);
            break;         
        case 17:
            ColorSet(1,1,1,1);
            break;     
        default:
            LightOffAll();
            break;
    }
}
