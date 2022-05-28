#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <IRremote.h>

const int RECV_PIN = 16;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
#define Motor1 23
#define Motor2 22
#define Motor3 21
#define Module 19
#define Button1 27
#define Button2 13
#define pwrBtn 14
#define IR_REC 16
#define LED_1 32 // inner LEDs
#define LED_2 33 // inner LEDs

  Serial.begin(115200);
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(Motor3, OUTPUT);
  pinMode(Module, OUTPUT);
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(pwrBtn, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

int variable;
double i=0, j=0;
int count = 0;
int btn1State = 0, btn2State = 0, pwrBtnState = 0;
static byte togglePwrBtn = 0;
int count1_value = 0, count2_value = 0;
int prestate1 = 0, prestate2 = 0;
static uint32_t previousMillis = 0;

int NUMPIXELS_1 = 10; // no. of neopixels of inner LEDs
int NUMPIXELS_2 = 10; // no. of neopixels of outer LEDs
Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMPIXELS_1, LED_1, NEO_GRB + NEO_KHZ800); // defining inner LEDs function
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMPIXELS_2, LED_2, NEO_GRB + NEO_KHZ800); // defining inner LEDs function

void IR()
{
  if (irrecv.decode(&results))
  {
    variable = results.value, HEX;
    Serial.println(variable, HEX);
    Serial.println(results.value, HEX);
    switch (results.value)
    {
      case 0xFFA25D:  Serial.println("Power "); if(pwrBtnState!=HIGH){pwrBtnState= HIGH;}else{pwrBtnState=LOW;}   break; // Power
      case 0xFF9867:  Serial.println("LED ++"); count1_value++; Serial.println(count1_value); break; // LED ++
      case 0xFF6897:  Serial.println("LED --"); count1_value--; Serial.println(count1_value); break; // LED --
      case 0xFF906F:  Serial.println("Motor ++"); count2_value++; Serial.println(count2_value); break; // Motor ++
      case 0xFFE01F:  Serial.println("Motor --"); count2_value--; Serial.println(count2_value); break; // Motor --
    }
    irrecv.resume();
  }
}

void btn1Logic()
{
  if (pwrBtnState == HIGH)
  {
    if (btn1State == HIGH && prestate1 == 0)
    {
      count1_value++;
      prestate1 = 1;
      if (i==0){count2_value=0;i++;}else{}
    }
    else if (btn1State == LOW)
    {
      prestate1 = 0;
    }
  }
  else
  {
    count1_value = 0;
    digitalWrite(Motor1, LOW);
    digitalWrite(Motor2, LOW);
    digitalWrite(Motor3, LOW);
    digitalWrite(Module, LOW);
  }
}

void btn2Logic()
{
  if (pwrBtnState == HIGH)
  {
    if (btn2State == HIGH && prestate2 == 0)
    {
      count2_value++;
      prestate2 = 1;
      if (j==0){count2_value=0;j++;}else{}
    }
    else if (btn2State == LOW)
    {
      prestate2 = 0;
    }
  }
  else
  {
    count2_value = 0;
  }
}

void motors()
{
  if (count1_value == 1)
  {
    digitalWrite(Motor1, HIGH);
    digitalWrite(Motor2, LOW);
    digitalWrite(Motor3, LOW);
    Serial.println("here 1");
    delay(10);

  }
  else if (count1_value == 2)
  {
    digitalWrite(Motor1, LOW);
    digitalWrite(Motor2, HIGH);
    digitalWrite(Motor3, LOW);
  }
  else if (count1_value == 3)
  {
    digitalWrite(Motor1, LOW);
    digitalWrite(Motor2, LOW);
    digitalWrite(Motor3, HIGH);
  }
  else if (count1_value == 4)
  {
    count1_value = 0;
    digitalWrite(Motor1, LOW);
    digitalWrite(Motor2, LOW);
    digitalWrite(Motor3, LOW);
    digitalWrite(Module, LOW);
    Serial.println("here");
  }
  else if (count1_value == -1){count1_value = 3;}
  else if (count1_value == 0){digitalWrite(Motor1, LOW);}
}


void pwrBtnRead()
{
  if (!digitalRead(pwrBtn))
  {
    togglePwrBtn = !togglePwrBtn;
    if (togglePwrBtn)
    {pwrBtnState = 1;}
    else
    {pwrBtnState = 0;}
    while(!digitalRead(pwrBtn));
  }
}

void btn1Read()
{
  if (btn1State == 0 && digitalRead(Button1) == HIGH)
  {
    btn1State = 1;
  }
  if (btn1State == 1 && digitalRead(Button1) == LOW)
  {
    btn1State = 0;
  }
}

void btn2Read()
{
  if (btn2State == 0 && digitalRead(Button2) == HIGH)
  {
    btn2State = 1;
  }
  if (btn2State == 1 && digitalRead(Button2) == LOW)
  {
    btn2State = 0;
  }
}

void timecounter()
{
  if (count == 0)
  {
    for (int i = 0; i < NUMPIXELS_1; i++)
    {
      pixels_1.setPixelColor(i, pixels_1.Color(255, 0, 0)); // Red
      pixels_1.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if (count > 0 && count <= 4)
  { digitalWrite(Module, HIGH);
    for (int i = 0; i < NUMPIXELS_1; i++) {
      pixels_1.setPixelColor(i, pixels_1.Color(128, 0, 128)); // purple
      pixels_1.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if ( count <= 16)
  {
    digitalWrite(Module, LOW);
    for (int i = 0; i < NUMPIXELS_1; i++) {
      pixels_1.setPixelColor(i, pixels_1.Color(0, 255, 0)); // GREEN
      pixels_1.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
}

void countcolour()
{
  if (count2_value == 0) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(0, 0, 0)); // off
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if (count2_value == 1) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(0, 0, 255)); // blue
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if (count2_value == 2) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(0, 255, 0)); // green
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }

  else if (count2_value == 3) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(255, 255, 0)); // yellow
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if (count2_value == 4) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(255, 0, 0)); // red
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds).
    }
  }
  else if (count2_value == 5) {
    for (int i = 0; i < NUMPIXELS_2; i++) {
      pixels_2.setPixelColor(i, pixels_2.Color(128, 0, 128)); // purple
      pixels_2.show(); // This sends the updated pixel color to the hardware.
      delay(10); // Delay for a period of time (in milliseconds)
    }
  }
  else if (count2_value == 6){count2_value = 0;}
  else if (count2_value == -1){count2_value = 5;}
}

void loop()
{
  pwrBtnRead();
  btn1Read();
  btn2Read();
  
  if (pwrBtnState == LOW){btn1State = LOW; btn2State = LOW;}
  
  if (millis() - previousMillis >= 1000)
  {
    previousMillis += 1000;
    Serial.print("Power: ");
    Serial.print(pwrBtnState);
    Serial.print("        ");
    Serial.print("Btn1: ");
    Serial.print(btn1State);
    Serial.print("        ");
    Serial.print("Btn2: ");
    Serial.print(btn2State);
    Serial.print("        ");
    Serial.print("counter1: ");
    Serial.print(count1_value);
    Serial.print("        ");
    Serial.print("counter2: ");
    Serial.print(count2_value);
    Serial.print("        ");
    Serial.print("count: ");
    Serial.println(count);
    count++;
    if (count > 16 || pwrBtnState == LOW)
    {
      count = 0;
    }
  }

  IR();
  timecounter();
  countcolour();
  btn1Logic();
  btn2Logic();

  motors();
}
