#include <apuEmulator.h>
#include <notes.h>


APU apu= APU(25);

void setup(){
  //500 frames
//  sigmaDeltaSetup(0, 88200);
//  sigmaDeltaAttachPin(25, 0);
//  sigmaDeltaWrite(0, 0);
//l= APU(25);
  oscillator test = {
    .waveform = 1,
    .pitch = apu.get_freq(C,1),
    .ctr=24,
    .pos = 0
  }; 
  oscillator eli = {
    .waveform = 1,
    .pitch = apu.get_freq(E,1),
    .ctr=12,
    .pos = 8
  }; 
  oscillator gre = {
    .waveform = 1,
    .pitch = apu.get_freq(G,1),
    .ctr=6,//1 beat
    .pos = 0
  }; 
  Serial.begin(9600);
//  Serial.print(0);
    apu.append(test);
  apu.append(eli);
  apu.append(gre);
//  Serial.println(l.data[0].waveform);
//  apu.remove(0);
//  Serial.println(l.data[0].waveform);
  
//  initWaves();
  apu.iterateAll();
}

void loop(){
  //1 is base
//  for(float i=0;i<256;i+=1){//double is pitch//16.35
//    dacWrite(25,SineValues[int(i)]);//gain is 15
////   dacWrite(25,SawValues[int(i)]);
////    Serial.println(SineValues[i]);
//  }
//  delay(1);

//  l.iterateAll();
  Serial.println(0);
  delay(1000);
}
