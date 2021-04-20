#include <apuEmulator.h>
#include <notes.h>

//set DAC voltage out pin
APU apu= APU(25);



void setup(){

  Serial.begin(9600);
//  //note,octave,note_len,note_pos,waveform within measure

    apu.add_note_queue(C,2,1,  1, 0);
    apu.add_note_queue(DS,2,1,   2, 0);
    apu.add_note_queue(GS,2,1,  3,0);
    apu.add_note_queue(C,3,1,    4,0);
    apu.add_note_queue(GS,2,1,  5,0);
    apu.add_note_queue(DS,2,1,   6, 0);
    apu.add_note_queue(C,2,1,  6+1, 0);
    apu.add_note_queue(DS,2,1,   6+2, 0);
    apu.add_note_queue(GS,2,1,  6+3,0);
    apu.add_note_queue(C,3,1,    6+4,0);
    apu.add_note_queue(GS,2,1,  6+5,0);
    apu.add_note_queue(DS,2,1,   6+6, 0);

    apu.add_note_queue(C,2,1,  12+1, 0);
    apu.add_note_queue(E,2,1,   12+2, 0);
    apu.add_note_queue(AS,2,1,  12+3,0);
    apu.add_note_queue(C,3,1,    12+4,0);
    apu.add_note_queue(AS,2,1,  12+5,0);
    apu.add_note_queue(E,2,1,   12+6, 0);
    
    apu.add_note_queue(C,2,1,  18+1, 0);
    apu.add_note_queue(E,2,1,   18+2, 0);
    apu.add_note_queue(AS,2,1,  18+3,0);
    apu.add_note_queue(C,3,1,    18+4,0);
    apu.add_note_queue(AS,2,1,  18+5,0);
    apu.add_note_queue(E,2,1,   18+6, 0);


//    apu.add_note_queue(C,3,0.5,  3+0, 1);
//    apu.add_note_queue(DS,3,0.5,   3+0.5, 1);
//    apu.add_note_queue(GS,3,0.5,  3+1,1);
//    apu.add_note_queue(C,4,0.5,    3+1.5,1);
//    apu.add_note_queue(DS,3,0.5,   3+2, 1);
//    apu.add_note_queue(GS,3,0.5,  3+2.5,1);
    
//  apu.remove(0);
//  Serial.println(l.data[0].waveform);
  
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
