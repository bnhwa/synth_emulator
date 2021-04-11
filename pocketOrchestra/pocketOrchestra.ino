#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
// beats per minute
#define BPM   (240)//120
#define BUZZER (2)
// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
//#include <AnalogSmooth.h>
#include <AoePlayer.h>
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
//             c0     cs1    d2     ds3    e4     f5  fs6   g7    gs8   a9   as10   b11 none12
//;//frequency in hertz for lowest octave
float narr[] = {
  16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87, 0
};//frequency in hertz for lowest octave
//byte lead_notes[] = {
//  // part 1
//  45, 114, 5, 25,
//  5, 114, 94, 94,
//  5, 45, 25, 5,
//  114, 114, 5, 25,
//  45, 5, 94, 94,
//  120,
//  25, 55, 95, 75,
//  55, 45, 5, 45,
//  25, 5, 114, 114,
//  5, 25, 45, 5,
//  94, 94, 120
//};
//byte lead_times[] = {
//  2, 1, 1, 2,
//  1, 1, 2, 1,
//  1, 2, 1, 1,
//  2, 1, 1, 2,
//  2, 2, 2, 2,
//  2,
//  3, 1, 2, 1,
//  1, 3, 1, 2,
//  1, 1, 2, 1,
//  1, 2, 2, 2,
//  2, 2, 2};
//             c0     cs1    d2     ds3    e4     f5  fs6   g7    gs8   a9   as10   b11 none12
byte lead_notes[] = {
  114,
  64,84,
  34,
  34,34,44,54,
  64,74,84,94,
  45,35,
  5,15,
  45,
  35,104,
  15,114,114,104,
  114,
  15,84,
  74,84,
  114,104,84,104,
  35,15,104,64,
  114,114,114,
  114,64,
  35,35,35,
  35,
  114,
  15,25,
  35,35,
  35,15,
  114,
  64,84,
  34,
  34,34,34,64,
  84,84,94,104,
  45,35,
  114,
  114,125,
  74,
  25,
  114,
  114,
  94,
  74,
  74,
  5,
  114,
  114,
  94,
  94,
  74,
  74,
  25,
  114,
  114,
  94,
  94,
  74,
  74,
  44,
  24,
114,
74,
74,
44,
74,
74,
74,
44,
74,
74,
74,
44,
114,
74,
74,
44,
74,
74,
74,
44,
74,
74,
74,
44,
114,
24,
4,
114,
94,
94,
74,
74,
74,
94,
74,
114,
//
  45, 114, 5, 25,
  5, 114, 94, 94,
  5, 45, 25, 5,
  114, 114, 5, 25,
  45, 5, 94, 94,
  120,
  25, 55, 95, 75,
  55, 45, 5, 45,
  25, 5, 114, 114,
  5, 25, 45, 5,
  94, 94, 120
};
byte lead_times[] = {
  6,
  3,3,
  6,
  2,1,2,1,
  2,1,2,1,
  3,3,
  6,6,
  6,
  3,3,
  2,1,2,1,
  6,
  3,3,
  3,3,
  2,1,2,1,
  2,1,2,1,
  2,1,3,
  3,3,
  2,1,3,
  6,
  6,
  3,3,
  3,3,
  3,3,
  6,
  3,3,
  6,
  2,1,2,1,
  2,1,2,1,
  3,3,
  6,
  3,3,
  4,2,2,2,4,2,2,4,
  2,2,2,2,4,
  2,2,2,2,2,2,2,2,4,8,
2,
4,
2,
2,
2,
4,
2,
2,
2,
4,
4,
4,
2,
4,
2,
2,
2,
4,
2,
2,
2,
4,
4,
4,
2,
2,
2,
2,
2,
2,
4,
2,
2,
2,
2,
2,
4,
4,
2,
8,
//
  2, 1, 1, 2,
  1, 1, 2, 1,
  1, 2, 1, 1,
  2, 1, 1, 2,
  2, 2, 2, 2,
  2,
  3, 1, 2, 1,
  1, 3, 1, 2,
  1, 1, 2, 1,
  1, 2, 2, 2,
  2, 2, 2
};
//trumpet main
//trombone
//allstar
//trombone main UNTIL MEASURE 9, 1st part trombone, last note is bas sax main, MEASURE 18 back to trombone
boolean intro = false;
int lead_note_count;// = sizeof(lead_notes);// / sizeof(float);
//AnalogSmooth a[2] = {AnalogSmooth(5),AnalogSmooth(5)};
AoePlayer p[2] = {AoePlayer(true, 0), AoePlayer(true, 1)};
byte gameState = 0;//
unsigned long curr_millis = 0;
byte shit = 0;
byte front = 0;
void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(6, OUTPUT);//powers player 1
  pinMode(7, OUTPUT);//powers player 2
  Serial.println(lead_note_count);//69
  tft.begin(HX8357D);
  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);

}
int which(int an) {
  int val = 5; int dist = 1000;
  //  int analogArr[27][2] = {
  //    {0,0},
  //    {75,5},{135,4},{184,45},{224,3},{259,35},
  //    {289,34},{314,345},{337,2},{357,25},{374,24},
  //    {390,245},{404,23},{417,235},{428,234},{449,1},
  //    {458,15},{466,14},{474,145},{481,13},{487,135},
  //    {494,134},{505,12},{510,125},{515,124},{523,123},
  //    {1023,0}};
  int analogArr[12][2] = {
    {0, 0},
//    {135, 4},
    {224, 4},
//    {289, 34},
    {337, 3},
//    {374, 24},
//    {404, 23},
    {507, 2},
//    {466, 14},
//    {481, 13},
//    {505, 12},
  {600, 1},
    {1023, 0}
  };
  for (int i = 0; i < sizeof(analogArr); i++) {
    int dst = abs(analogArr[i][0] - an);
    if (dst <= dist) {
      val = analogArr[i][1];
      dist = dst;
    } else {
      return val;
    }
  }
  return val;
}
void loop() {
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  Serial.println(shit);
  int curr_lead_note = 0;
  shit = which(analogRead(0));//a[0].analogReadSmooth(0)
  float curr_time_remaining;// = ((float)lead_times[curr_lead_note+front])*0.5;
  float lead_freq;
  boolean drawn = false;
  unsigned long duration;
  int colors[] = {HX8357_RED,HX8357_BLUE,HX8357_GREEN,HX8357_YELLOW};
  switch (gameState) {
    case 0:
      if (!intro){
        tft.setCursor(0,0);
        drawLines(HX8357_RED);
        tft.setTextColor(HX8357_GREEN);
        tft.setTextSize(5);
        //tft.setCursor(tft.width()/2, tft.height()/2);
        tft.println("POCKET BAND");
        tft.setTextColor(HX8357_GREEN);
        tft.setTextSize(3);
        tft.println("An original 2 player game by CTRL+Z");
        tft.setTextSize(2);
        tft.println("PLAYER 1 Press button 1 for random song!");
        //delay(1000);
        intro = true;
      } else if (shit >0){       
        switch(random(1,2)){
          case 1:
            lead_note_count = 69;
            front = 0;
          break;
          case 2:
            lead_note_count = 40;
            front = 128;
//            lead_note_count = 59;
//            front = 69;
          break;
          case 3:
            lead_note_count = 40;
            front = 128;
          break;
        }
        curr_time_remaining = ((float)lead_times[curr_lead_note+front])*0.5;
//      Serial.println(which(a[0].analogReadSmooth(0)));
        gameState=1;
      }
    break;
    case 1:
      tft.fillScreen(HX8357_BLACK);
      tft.fillRect(tft.width()*0.75, 0, tft.width()*0.75, tft.height(), HX8357_WHITE);
      //tft.fillRect(0, tft.height()*0.75, tft.width()*0.75, tft.height(), HX8357_WHITE);
      for (int i=0; i<sizeof(colors); i++){
        tft.fillCircle(tft.width()*0.875,(tft.height()/8)+(tft.height()/4)*(i),tft.height()/16,colors[i]);
        tft.fillCircle(tft.width()*0.875,(tft.height()/8)+(tft.height()/4)*(i),tft.height()/32,HX8357_WHITE);
      }
      delay(1000);
      lead_freq = get_freq(lead_notes[curr_lead_note+front ]);
      duration = curr_time_remaining*(1000 * (60.0 / BPM));
      curr_millis = millis();
      while (curr_lead_note < lead_note_count) {//&& curr_bass_note < bass_note_count
        duration = curr_time_remaining*( 1000 * (60.0 / BPM));
        byte val = 4 - ((lead_notes[curr_lead_note+front] + 10) / (40));
        if (!drawn && lead_freq>0){
          tft.fillRect(0, (tft.height()/16)+(tft.height()/4)*(val-1), tft.width()*0.75, tft.height()/8, colors[val-1]);
          drawn = true;
        }
        for (int i=0; i<2; i++){
          if(abs(millis()-curr_millis-duration)>350 && lead_freq>0 && val==which(analogRead(i))){
            p[i].addScore(abs(5));
          }
        }    
        if (millis()>curr_millis+duration){
//          Serial.println(val);
          drawn = false;
          tft.fillRect(0, (tft.height()/16)+(tft.height()/4)*(val-1), tft.width()*0.75, tft.height()/8, HX8357_BLACK);
          curr_millis = millis();//increment
          curr_lead_note++;      
          lead_freq = get_freq(lead_notes[curr_lead_note+front]);
          curr_time_remaining = ((float)lead_times[curr_lead_note+front])*0.5;
          noTone(BUZZER);
        }
        if (lead_freq>0){
          tone(BUZZER,lead_freq);//, duration);
        }else if (lead_freq==0){
          noTone(BUZZER);
        }
      }
      noTone(BUZZER);
      tft.fillScreen(HX8357_BLACK);
      tft.setTextSize(5);
      tft.setTextColor(HX8357_GREEN);
      if (abs(p[0].score)>abs(p[1].score)){
        tft.println("PLAYER 1 WINS!!!");
      }else if (abs(p[0].score)<abs(p[1].score)){
        tft.println("PLAYER 2 WINS!!!");
      }else{
        tft.println("TIE!!");
      }
      tft.setTextSize(2);
      tft.println("PLAYER 1 score: ");
      tft.println(p[0].score);
      tft.println("PLAYER 2 score: ");
      tft.println(p[1].score);
      tft.println("PLAYER 1 press any button to restart");
//      front = 0;
      gameState=2;
      shit=0;
      
      break;
    case 2:
    delay(2000);
        for (int i=0; i<sizeof(p); i++){//reset
          p[i] = AoePlayer(true, i);
        }
        intro = false;
        gameState=0;    
//    if (shit !=0){
//        for (int i=0; i<sizeof(p); i++){//reset
//          p[i] = AoePlayer(true, i);
//        }
//        intro = false;
//        gameState=0;
//      }
      break; 
  }

}
float get_freq(byte l_nt) {//get note frequency
  return narr[l_nt / 10] * pow(2, (l_nt % 10));
}
void drawLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(HX8357_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

}
