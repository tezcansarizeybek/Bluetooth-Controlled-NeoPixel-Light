#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define DEBUG         //haberleşme mesajlarını göster
#define NUM_LEDS 16   //Neopixel led saysıı
#define PIN 4         //Neopixel in bağlı olduğu digital pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial btSerial(10, 11); //RX,TX

bool adaptive = true;
uint8_t rVal = 230, gVal = 230 , bVal = 230, lastBright = 0, brVal1 = 0, setBright = 0;
int effectIndex = 0;
int btnsyc = 0;

void setup() {
  pinMode(5,INPUT);
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  strip.begin();
  strip.show();
  btSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  parseData(&btSerial);
}

void parseData(SoftwareSerial *s)
{
  if (s->available())
  {
    char c = s->read();

#ifdef DEBUG
    Serial.print("Data: ");
    Serial.print(c);
    Serial.print("|");
#endif

    if (c == '#')           //Renk seçimi
    {
      rVal = s->parseInt();
      gVal = s->parseInt();
      bVal = s->parseInt();
      if(rVal == 255 && gVal == 255 && bVal == 255){
        rVal = 230;
        gVal = 230;
        bVal = 230;
      }
      switch (effectIndex)
      {
        case 0:
          normal(strip.Color(rVal,gVal,bVal));
          break;
        case 1:
          sirali(strip.Color(rVal, gVal, bVal), 50);
          break;
        case 2:
          gokKusagi(strip.Color(rVal, gVal, bVal), 50);
          break;
          case 3:
          boolean cikis = false;
          while(!cikis){
          boolean ses = digitalRead(5);
          if(ses){
            Serial.println("Işık yandı");
          }
            disco(ses);
            char c = s->read();
            if(c == '/' || c == '&' || c == '?'){
              cikis = true;
            }
          }
            break;
      }
#ifdef DEBUG
      Serial.print(rVal);
      Serial.print("|");
      Serial.print(gVal);
      Serial.print("|");
      Serial.println(bVal);
#endif
    }
    else if (c == '&')      //Parlaklık ayarı
    {
      int brVal = s->parseInt();
      strip.setBrightness(parlaklikDegistir(brVal));
      normal(strip.Color(rVal, gVal, bVal));
    } 
    else if (c == '/'){   //Işık açma
      for(int i = 0; i < strip.numPixels(); i++){
        strip.setPixelColor(i,strip.Color(rVal,gVal,bVal));
        strip.show();
      }
    }
    else if(c == '?'){    //Işık kapama
      for(int i = 0; i < strip.numPixels(); i++){
        strip.setPixelColor(i,strip.Color(0,0,0));
        strip.show();
      }
    }
    else if (c == '$')
    {
      int eIndex = s->parseInt();
      effectIndex = eIndex;
#ifdef DEBUG
      Serial.println(effectIndex);
#endif
    }
  }
}

int parlaklikDegistir(uint8_t val)
{
  int pval = 0;
  if (val == 0)
  {
    pval = 0;
  } 
  else if (val == 1)
  {
    pval = 25;
  }
  else if (val == 2){
    pval = 50;
  }
  else if (val == 3){
    pval = 75;
  }
  else{
    pval = 100;
  }
  delay(5);
  return pval;
}
void normal(uint32_t c){
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i,c);
    strip.show();
  }
}
void sirali(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void gokKusagi(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { 
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);
      }
      strip.show();
      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);
      }
      strip.show();
      delay(wait);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
    }
  }
}
void disco(boolean ses){
  if(ses){
    delay(2);
  rVal = random(0,255);
  gVal = random(0,255);
  bVal = random(0,255);
  if(rVal == 255 && gVal == 255 && bVal == 255){
        rVal = 230;
        gVal = 230;
        bVal = 230;
      }
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i,strip.Color(rVal,gVal,bVal));
    strip.show();
  }
  }
}
