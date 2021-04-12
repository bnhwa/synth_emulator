#include <apuEmulator.h>
#include <notes.h>


APU apu= APU(25);

void setup(){

  Serial.begin(9600);
  //waveform,note,octave,note_len,note_pos within measure
   
    apu.add_oscillator_queue(C,3,2,0,1);//apu.add_oscillator_queue(C,3,4,0,0);
    apu.add_oscillator_queue(E,3,2,0,0);
    apu.add_oscillator_queue(G,3,2,0,0);
//    apu.add_oscillator_queue(C,4,2,0,1);
//    apu.add_oscillator_queue(C,3,2,0,1);
    apu.add_oscillator_queue(D,3,1,2,1);
    apu.add_oscillator_queue(E,3,1,3,1);
    apu.add_oscillator_queue(F,3,2,4,1);apu.add_oscillator_queue(C,3,2,4,1);apu.add_oscillator_queue(GS,3,2,4,1);
    apu.add_oscillator_queue(G,3,1,5,1);
    apu.add_oscillator_queue(F,3,1,6,1);
    apu.add_oscillator_queue(A,3,2,7,1);
    
    apu.add_oscillator_queue(GS,3,3,8,1);//apu.add_oscillator_queue(CS,3,3,8,0);
    apu.add_oscillator_queue(E,3,3,9,1);
    apu.add_oscillator_queue(CS,3,3,10,1);
    apu.add_oscillator_queue(A,2,3,11,1);
    apu.add_oscillator_queue(E,2,3,12,1);
    apu.add_oscillator_queue(CS,2,3,13,1);
    apu.add_oscillator_queue(A,2,18,14,1);
    apu.add_oscillator_queue(GS,2,18,16,1);
    apu.add_oscillator_queue(CS,2,18,18,1);

//  apu.remove(0);
//  Serial.println(l.data[0].waveform);
  
//  initWaves();
//  apu.setSpeed(10);
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
