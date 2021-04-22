#include <apuEmulator.h>
#include <notes.h>


APU apu= APU(25);



void setup(){

  Serial.begin(9600);
//  //note,octave,note_len,note_pos,waveform within measure
apu.add_note_queue(A,4,1.0,0.0,0);
apu.add_note_queue(GS,4,1.0,1.0,0);
apu.add_note_queue(E,4,1.0,2.0,0);
apu.add_note_queue(CS,4,1.0,3.0,0);
apu.add_note_queue(B,3,1.0,4.0,0);
apu.add_note_queue(CS,4,3.0,5.0,0);
apu.add_note_queue(FS,4,1.0,8.0,0);
apu.add_note_queue(E,4,1.0,9.0,0);
apu.add_note_queue(D,4,1.0,10.0,0);
apu.add_note_queue(A,3,1.0,11.0,0);
apu.add_note_queue(G,3,1.0,12.0,0);
apu.add_note_queue(A,3,3.0,13.0,0);
apu.add_note_queue(F,4,1.0,16.0,0);
apu.add_note_queue(E,4,1.0,17.0,0);
apu.add_note_queue(D,4,1.0,18.0,0);
apu.add_note_queue(AS,3,1.0,19.0,0);
apu.add_note_queue(C,4,1.0,20.0,0);
apu.add_note_queue(G,4,3.0,21.0,0);
apu.add_note_queue(FS,4,0.5,24.0,0);
apu.add_note_queue(F,4,0.5,24.5,0);
apu.add_note_queue(E,4,5.0,25.0,0);
//  initWaves();
//  apu.setSpeed(0.25);
//  apu.setSpeed(8);
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
