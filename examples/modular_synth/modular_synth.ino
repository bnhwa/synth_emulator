#include <apuEmulator.h>
#include <notes.h>


APU apu= APU(25);

void setup(){
  //500 frames
//  sigmaDeltaSetup(0, 88200);
//  sigmaDeltaAttachPin(25, 0);
//  sigmaDeltaWrite(0, 0);
//l= APU(25);
//  oscillator test = {
//    .waveform = 1,
//    .pitch = apu.get_freq(C,1),
//    .ctr=4,
//    .pos = 0
//  }; 
//  oscillator eli = {
//    .waveform = 1,
//    .pitch = apu.get_freq(E,1),
//    .ctr=2,
//    .pos = 0
//  }; 
//  oscillator gre = {
//    .waveform = 1,
//    .pitch = apu.get_freq(G,1),
//    .ctr=1,//1 beat
//    .pos = 0
//  }; 
  oscillator test = {
    .waveform = 1,
    .pitch = apu.get_freq(C,1),
    .ctr=1,
    .pos = 1
  }; 
  oscillator eli = {
    .waveform = 1,
    .pitch = apu.get_freq(E,1),
    .ctr=1,
    .pos = 2
  }; 
  oscillator gre = {
    .waveform = 1,
    .pitch = apu.get_freq(G,1),
    .ctr=1,//1 beat
    .pos = 3
  }; 
  Serial.begin(9600);
  //waveform,note,octave,note_len,note_pos within measure
    apu.add_oscillator_queue(C,1,1,0,1);
    apu.add_oscillator_queue(E,1,1,1,1);
    apu.add_oscillator_queue(G,1,1,2,1);
    apu.add_oscillator_queue(C,1,2,3,1);
    apu.add_oscillator_queue(E,1,2,3,1);
    apu.add_oscillator_queue(G,1,2,3,1);
    apu.add_oscillator_queue(C,2,2,3,1);
//  Serial.println(l.data[0].waveform);
//  apu.remove(0);
//  Serial.println(l.data[0].waveform);
  
//  initWaves();
//  apu.setSpeed(0.5);
  apu.setSpeed(8);
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
  Serial.println("end");
//  String ok = "3.5";
//  Serial.println(ok.toFloat());
  delay(1000);
}
